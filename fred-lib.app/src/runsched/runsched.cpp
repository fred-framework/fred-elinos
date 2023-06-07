#include <errno.h>
#include <sched.h>
#include <sched.h>       /* Definition of SCHED_* constants */
#include <sys/syscall.h> /* Definition of SYS_* constants */
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <limits>
#include <span>
#include <string>
#include <vector>

// Bad practice
using namespace std;

struct sched_attr {
    uint32_t size = sizeof(sched_attr);
    uint32_t sched_policy = 0;   /* Policy (SCHED_*) */
    uint64_t sched_flags = 0;    /* Flags */
    int32_t sched_nice = 0;      /* Nice value (SCHED_OTHER, SCHED_BATCH) */
    uint32_t sched_priority = 0; /* Static priority (SCHED_FIFO, SCHED_RR) */

    /* Remaining fields are for SCHED_DEADLINE */
    uint64_t sched_runtime = 0;
    uint64_t sched_deadline = 0;
    uint64_t sched_period = 0;
};

int sched_setattr(pid_t pid, sched_attr *attr, unsigned int flags) {
    return syscall(SYS_sched_setattr, pid, attr, flags);
}

int sched_getattr(pid_t pid, sched_attr *attr, unsigned int flags) {
    return syscall(SYS_sched_getattr, pid, attr, flags);
}

void usage(FILE *file) {
    auto help = R"DELIM(Run command with the given policy and priority

USAGE: runsched POLICY OPTIONS command

Options depend on the POLICY:
    - SCHED_FIFO       <PRIO>
    - SCHED_RR         <PRIO>
    - SCHED_DEADLINE   <RUNTIME> <DEADLINE> <PERIOD>
)DELIM";

    fprintf(file, "%s", help);
}

struct action {
    sched_attr sched;
    char *command_file;
    vector<char *> command_args;
};

template <typename Number>
Number parse_number_range(const string &snumber, Number min, Number max) {
    try {
        Number number = stoi(snumber);
        if (number < min || number > max) {
            throw -1;
        }
        return number;
    } catch (...) {
        fprintf(stderr, "ERROR: invalid number %s\n", snumber.c_str());
        exit(EXIT_FAILURE);
    }
}

uint32_t parse_fifo_rr_number(const string &snumber) {
    return parse_number_range<uint32_t>(snumber, 1, 99);
}

uint64_t parse_deadline_number(const string &snumber) {
    return parse_number_range<uint64_t>(snumber, 1,
                                        std::numeric_limits<uint64_t>::max());
}

void parse_fifo_rr_prio(action &request, const string &sprio) {
    request.sched.sched_priority = parse_fifo_rr_number(sprio);
}

void parse_deadline_prio(action &request, const string &sruntime,
                         const string &sdeadline, const string &speriod) {
    request.sched.sched_runtime = parse_deadline_number(sruntime);
    request.sched.sched_deadline = parse_deadline_number(sdeadline);
    request.sched.sched_period = parse_deadline_number(speriod);
}

vector<char *> construct_args(span<char *> argv) {
    vector<char *> args(argv.size() + 1);
    args.assign(argv.begin(), argv.end());
    args[argv.size()] = nullptr;
    return args;
}

action parse_args(span<char *> args) {
    if (args.size() < 3) {
        fprintf(stderr, "ERROR: missing arguments!\n");
        usage(stderr);
        exit(EXIT_FAILURE);
    }

    action request;

    const string spolicy = args[0];
    if (spolicy == "SCHED_FIFO") {
        request.sched.sched_policy = SCHED_FIFO;
    } else if (spolicy == "SCHED_RR") {
        request.sched.sched_policy = SCHED_RR;
    } else if (spolicy == "SCHED_DEADLINE") {
        request.sched.sched_policy = SCHED_DEADLINE;
    } else {
        fprintf(stderr, "ERROR, Invalid policy %s", spolicy.c_str());
        usage(stderr);
        exit(EXIT_FAILURE);
    }

    // Consume first argument
    args = args.subspan(1);

    switch (request.sched.sched_policy) {
    case SCHED_FIFO: // [[fallthrough]] //
    case SCHED_RR:
        parse_fifo_rr_prio(request, args[0]);
        args = args.subspan(1);
        break;
    case SCHED_DEADLINE:
        if (args.size() < 4) {
            // There are not enough arguments left
            fprintf(stderr, "ERROR: missing arguments!\n");
            usage(stderr);
            exit(EXIT_FAILURE);
        }

        parse_deadline_prio(request, args[0], args[1], args[2]);
        args = args.subspan(3);
        break;
    }

    // args now contains the command name and its arguments, but it may not
    // contain the NULL termination that we need (even if conventionally it
    // should be there)

    request.command_file = args[0];
    request.command_args = construct_args(args);
    // The 0-argument is contained also in here

    return request;
}

int main(int argc, char *argv[]) {
    // Parse and validate arguments
    auto request = parse_args(span(argv + 1, argc - 1));

    // Arguments were valid, attempt to set scheduling params
    if (sched_setattr(0, &request.sched, 0)) {
        fprintf(stderr, "ERROR: sched_setattr failed: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("Attempting to start %s with args\n", request.command_file);

    // Scheduling params set successfully, run requested action
    execvp(request.command_file, request.command_args.data());
    fprintf(stderr, "ERROR: execvp failed: %s", strerror(errno));
    return EXIT_FAILURE;
}
