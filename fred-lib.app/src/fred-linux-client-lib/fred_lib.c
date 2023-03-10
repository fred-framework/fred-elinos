/*
 * Fred for Linux. Experimental support.
 *
 * Copyright (C) 2018-2021, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2.
*/

#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "fred_lib.h"
#include "dbg_print.h"
#include "fred_parameters.h"
#include "fred_msg.h"
#include "user_buff.h"

//---------------------------------------------------------------------------------------------

struct fred_hw_task {
	uint32_t id;
	struct user_buff buffers[MAX_DATA_BUFFS];
	int buffers_count;
};

//---------------------------------------------------------------------------------------------

struct fred_data {
	enum client_state {
		CLI_ERROR,
		CLI_INIT,
		CLI_IDLE,
		CLI_ACCEL
	} state;

	int fd;

	struct fred_hw_task *hw_tasks[MAX_HW_TASKS];
	int hw_tasks_count;
};

//---------------------------------------------------------------------------------------------

static
int connect_to_server_()
{
	int retval;
	int sockfd;
	struct sockaddr_un serv_addr;
	size_t addr_len;

    // Create a socket
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sockfd < 0) {
		DBG_PRINT("fred_lib: could not open socket: %s\n", strerror(errno));
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strncpy(serv_addr.sun_path, LIST_SOCK_PATH, sizeof(serv_addr.sun_path) - 1);

    // Connect to the server
	addr_len = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
	retval = connect(sockfd, (struct sockaddr *)&serv_addr, addr_len);
	if (retval < 0) {
		DBG_PRINT("fred_lib: could not connect: %s\n", strerror(errno));
		return -1;
	}

	return sockfd;
}

static inline
int send_to_server_(int socket, const void *data, int data_len)
{
	int retval;

	retval = write(socket, data, data_len);
	if (retval != data_len) {
		DBG_PRINT("fred_lib: send: unable to reach server. Error: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

static inline
int receive_from_server_(int socket, void *data, int data_len)
{
	int retval;

	retval = read(socket, data, data_len);
	if (retval != data_len) {
		DBG_PRINT("fred_lib: receive: unable to reach server. Error: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

//---------------------------------------------------------------------------------------------

int fred_init(struct fred_data **self)
{
	int retval;
	struct fred_msg msg;

	*self = calloc(1, sizeof(**self));
	if (!(*self)) {
		DBG_PRINT("fred_lib: could not allocate memory\n");
		return FRED_ERR_MEM;
	}

	(*self)->fd = connect_to_server_();
	if ((*self)->fd <= 0) {
		(*self)->state = CLI_ERROR;
		return FRED_ERR_COMM;
	}

	// Create message for the server
	fred_msg_set_head(&msg, FRED_MSG_INIT);
	// args are set just to avoid sending uninitialised data
	// see this valgrind report
	// ==1860== Syscall param write(buf) points to uninitialised byte(s)
	// ==1860==    at 0x49024D6: write (syscall-template.S:84)
	// ==1860==    by 0x4859CFB: send_to_server_ (fred_lib.c:91)
	// ==1860==    by 0x4859E37: fred_init (fred_lib.c:137)	
	fred_msg_set_arg(&msg, 0);

	// Send
	retval = send_to_server_((*self)->fd, &msg, sizeof(msg));
	if (retval) {
		(*self)->state = CLI_ERROR;
		return FRED_ERR_COMM;
	}

	// Receive and check for an acknowledge
	retval = receive_from_server_((*self)->fd, &msg, sizeof(msg));
	if (retval) {
		(*self)->state = CLI_ERROR;
		return FRED_ERR_COMM;
	}

	if (fred_msg_get_head(&msg) != FRED_MSG_ACK) {
		DBG_PRINT("fred_lib: warning! initialization request"
				"rejected by the fred server\n");
		(*self)->state = CLI_ERROR;
		return FRED_ERR_INT;
	}

	DBG_PRINT("fred_lib: connected to fred server!\n");
	(*self)->state = CLI_INIT;
	return 0;
}

int fred_bind(struct fred_data *self, struct fred_hw_task **hw_task, uint32_t hw_task_id)
{
	int retval;
	int idx;
	struct fred_msg msg;

	if (!self)
		return FRED_ERR_ARGS;

	assert(self->hw_tasks_count < MAX_HW_TASKS);

	if (!(self->state == CLI_INIT || self->state == CLI_IDLE)) {
		DBG_PRINT("fred_lib: warning! can bind only if idle\n");
		return FRED_ERR_REQ;
	}

	// Insert the new hw-task as the last hw-task
	idx = self->hw_tasks_count;

	// Create message for the server
	fred_msg_set_head(&msg, FRED_MSG_BIND);
	fred_msg_set_arg(&msg, hw_task_id);

	// Send
	retval = send_to_server_(self->fd, &msg, sizeof(msg));
	if (retval) {
		self->state = CLI_ERROR;
		return FRED_ERR_COMM;
	}

	// Receive the number of buffers
	retval = receive_from_server_(self->fd, &msg, sizeof(msg));
	if (retval) {
		DBG_PRINT("fred_lib: could not receive data buffers size\n");
		self->state = CLI_ERROR;
		return FRED_ERR_COMM;
	}

	if (fred_msg_get_head(&msg) != FRED_MSG_BUFFS) {
		DBG_PRINT("fred_lib: unable to bind, check hw-task id\n");
		return FRED_ERR_REQ;
	}

	// Allocate a new hw-task
	self->hw_tasks[idx] = calloc(1, sizeof(*(self->hw_tasks[idx])));
	if (!(self->hw_tasks[idx])) {
		DBG_PRINT("fred_lib: could not allocate memory\n");
		self->state = CLI_ERROR;
		return FRED_ERR_MEM;
	}

	// Set id and buffers count
	self->hw_tasks[idx]->id = hw_task_id;
	self->hw_tasks[idx]->buffers_count = (int)fred_msg_get_arg(&msg);

	assert(self->hw_tasks[idx]->buffers_count <= MAX_DATA_BUFFS);

	// Receive buffers
	retval = receive_from_server_(self->fd, self->hw_tasks[idx]->buffers,
				self->hw_tasks[idx]->buffers_count * sizeof(self->hw_tasks[idx]->buffers[0]));
	if (retval) {
		DBG_PRINT("fred_lib: could not receive data buffers\n");
		self->state = CLI_ERROR;
		return FRED_ERR_COMM;
	}

	// Update hw-tasks count and client state
	++self->hw_tasks_count;
	self->state = CLI_IDLE;

	// Set parameter
	*hw_task = self->hw_tasks[idx];

	return 0;
}

int fred_async_accel(struct fred_data *self, const struct fred_hw_task *hw_task)
{
	int retval;
	struct fred_msg msg;

	if (!self)
		return FRED_ERR_ARGS;

	if (self->state != CLI_IDLE) {
		DBG_PRINT("fred_lib: warning! cannot request an acceleration right now\n");
		return FRED_ERR_REQ;
	}

	// Create message for the server
	fred_msg_set_head(&msg, FRED_MSG_RUN);
	fred_msg_set_arg(&msg, hw_task->id);

	// Send acceleration request
	retval = send_to_server_(self->fd, &msg, sizeof(msg));
	if (retval) {
		self->state = CLI_ERROR;
		return FRED_ERR_COMM;
	}

	self->state = CLI_ACCEL;
	return 0;
}

int fred_async_wait_for_accel(struct fred_data *self)
{
	int retval;
	struct fred_msg msg;

	if (!self)
		return FRED_ERR_ARGS;

	if (self->state != CLI_ACCEL) {
		DBG_PRINT("fred_lib: warning! no acceleration request has been issued\n");
		return FRED_ERR_REQ;
	}

	// Block until acceleration request arrive and check for an acknowledge
	retval = receive_from_server_(self->fd, &msg, sizeof(msg));
	if (retval) {
		self->state = CLI_ERROR;
		return FRED_ERR_COMM;
	}

	if (fred_msg_get_head(&msg) != FRED_MSG_DONE) {
		DBG_PRINT("fred_lib: warning! internal server error\n");
		self->state = CLI_ERROR;
		return FRED_ERR_INT;
	}

	self->state = CLI_IDLE;
	return 0;
}

// Wrapper
int fred_accel(struct fred_data *self, const struct fred_hw_task *hw_task)
{
	int retval;

	if (!self || !hw_task)
		return FRED_ERR_ARGS;

	retval = fred_async_accel(self, hw_task);

	if (retval)
		return retval;

	retval = fred_async_wait_for_accel(self);

	return retval;
}

void fred_free(struct fred_data *self)
{
	if (!self)
		return;

	if (self->state == CLI_ACCEL) {
		DBG_PRINT("fred_lib: warning! must wait for acceleration request\n");
		return;
	}

	// For each hw-task
	for (int i = 0; i < self->hw_tasks_count; ++i) {
		if (self->hw_tasks[i]) {
			// Unmap buffers
			for (int j = 0; j < self->hw_tasks[i]->buffers_count; ++j) {
				user_buff_unmap(&self->hw_tasks[i]->buffers[j]);
			}
			// free hw-task
			free(self->hw_tasks[i]);
		}
	}

	if (self->fd > 0)
		close(self->fd);
}

//---------------------------------------------------------------------------------------------

void *fred_map_buff(const struct fred_data *self, struct fred_hw_task *hw_task, int buff_idx)
{
	if (!self || !hw_task)
		return NULL;

	if (self->state != CLI_IDLE) {
		DBG_PRINT("fred_lib: warning! cannot map buffers right now\n");
		return NULL;
	}

	if (buff_idx >= hw_task->buffers_count) {
		DBG_PRINT("fred_lib: warning! requested buffer does not exist\n");
		return NULL;
	}

	return user_buff_map(&hw_task->buffers[buff_idx]);
}

void fred_unmap_buff(const struct fred_data *self, struct fred_hw_task *hw_task, int buff_idx)
{
	if (!self || !hw_task)
		return;

	if (self->state != CLI_IDLE) {
		DBG_PRINT("fred_lib: warning! cannot unmap buffers right now\n");
		return;
	}

	if (buff_idx >= hw_task->buffers_count) {
		DBG_PRINT("fred_lib: warning! requested buffer does not exist\n");
		return;
	}

	user_buff_unmap(&hw_task->buffers[buff_idx]);
}

int fred_get_buffs_count(const struct fred_data *self, struct fred_hw_task *hw_task)
{
	if (!self || !hw_task)
		return FRED_ERR_ARGS;

	if (self->state != CLI_IDLE) {
		DBG_PRINT("fred_lib: warning! cannot get buffers count right now\n");
		return FRED_ERR_REQ;
	}

	return hw_task->buffers_count;
}

ssize_t fred_get_buff_size(const struct fred_data *self, struct fred_hw_task *hw_task, int buff_idx)
{
	if (!self || !hw_task)
		return FRED_ERR_ARGS;

	if (self->state != CLI_IDLE) {
		DBG_PRINT("fred_lib: warning! cannot get buffer size right now\n");
		return FRED_ERR_REQ;
	}

	if (buff_idx >= hw_task->buffers_count) {
		DBG_PRINT("fred_lib: warning! requested buffer does not exist\n");
		return FRED_ERR_ARGS;
	}

	return (ssize_t)user_buff_get_size(&hw_task->buffers[buff_idx]);
}

//---------------------------------------------------------------------------------------------
