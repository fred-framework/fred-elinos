/*
 * Fred for Linux. Experimental support.
 *
 * Copyright (C) 2018-2021, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#ifndef FRED_LIB_H_
#define FRED_LIB_H_

// required to call from a C++ program
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

//---------------------------------------------------------------------------------------------

#define DBG_VERBOSE

//---------------------------------------------------------------------------------------------

enum fred_errors {
	FRED_ERR_REQ	= 200,
	FRED_ERR_ARGS	= 100,
	FRED_ERR_MEM	= -100,
	FRED_ERR_COMM	= -200,
	FRED_ERR_MAP	= -400,
	FRED_ERR_INT	= -500
};

//---------------------------------------------------------------------------------------------

struct fred_data;
struct fred_hw_task;

//---------------------------------------------------------------------------------------------

int fred_init(struct fred_data **self);

int fred_bind(struct fred_data *self, struct fred_hw_task **hw_task, uint32_t hw_task_id);

int fred_accel(struct fred_data *self, const struct fred_hw_task *hw_task);

void fred_free(struct fred_data *self);

//---------------------------------------------------------------------------------------------

int fred_async_accel(struct fred_data *self, const struct fred_hw_task *hw_task);

int fred_async_wait_for_accel(struct fred_data *self);

//---------------------------------------------------------------------------------------------

int fred_get_buffs_count(const struct fred_data *self, struct fred_hw_task *hw_task);

ssize_t fred_get_buff_size(const struct fred_data *self, struct fred_hw_task *hw_task,
							int buff_idx);

//---------------------------------------------------------------------------------------------

void *fred_map_buff(const struct fred_data *self, struct fred_hw_task *hw_task, int buff_idx);

void fred_unmap_buff(const struct fred_data *self, struct fred_hw_task *hw_task, int buff_idx);

//---------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* FRED_LIB_H_ */
