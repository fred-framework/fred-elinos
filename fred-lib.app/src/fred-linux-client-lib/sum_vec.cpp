#include "fredcpp.h"

#include <ctime>
#include <algorithm>
#include <iostream>

// TODO: a way to specify argument types and sizes

struct timespec diff_timespec(const struct timespec *time1,
                              const struct timespec *time0) {
    assert(time1);
    assert(time0);
    struct timespec diff = {.tv_sec = time1->tv_sec - time0->tv_sec,
                            .tv_nsec = time1->tv_nsec - time0->tv_nsec};
    if (diff.tv_nsec < 0) {
        diff.tv_nsec += 1000000000; // nsec/sec
        diff.tv_sec--;
    }
    return diff;
}

int main() {
    using vec = std::array<int8_t, 1024>;

    fredcpp handle;
    fredcpp_function<vec, vec, vec> sum_vec(handle, 100);

    // NOTE: destination is the 0th argument!!!!
    vec &A = sum_vec.get<1>();
    vec &B = sum_vec.get<2>();
    vec &C = sum_vec.get<0>();

    auto dump_vec = [](std::ostream &os, const vec &mvector) -> std::ostream & {
        os << "[ ";
        std::for_each(std::begin(mvector), std::begin(mvector) + 10,
                      [&os](int8_t v) { os << int(v) << " "; });
        os << "]";
        return os;
    };

    std::fill(std::begin(A), std::end(A), 2);
    std::fill(std::begin(B), std::end(B), 3);

    struct timespec before = {0};
    struct timespec after = {0};
    struct timespec difference;

    // Do the thing!!
    clock_gettime(CLOCK_MONOTONIC_RAW, &before);
    sum_vec();
    clock_gettime(CLOCK_MONOTONIC_RAW, &after);

    difference = diff_timespec(&after, &before);
    printf("That took: %lld.%.9ld\n", (long long)difference.tv_sec,
           difference.tv_nsec);

    auto test = [](const int8_t &v) { return v == int8_t(2 + 3); };
    if (std::all_of(C.cbegin(), C.cend(), test)) {
        std::cout << "   MATCH!!\n";
    } else {
        std::cout << "NO MATCH!!\n";
    }

    dump_vec(std::cout, A) << " +\n";
    dump_vec(std::cout, B) << " =\n";
    dump_vec(std::cout, C) << "\n";
}
