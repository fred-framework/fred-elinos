#include "fredcpp.h"

#include <algorithm>
#include <iostream>

using itype = int8_t;

template <int size>
using fred_vec = std::array<itype, size>;

template <int size>
using fred_mat = std::array<std::array<itype, size>, size>;

using v1024 = fred_vec<1024>;
using m64 = fred_mat<64>;
using m128 = fred_mat<128>;

using sum_vec_t = fredcpp_function<100, v1024, v1024, v1024>;
using xor_vec_t = fredcpp_function<101, v1024, v1024, v1024>;
using mat_mul128_t = fredcpp_function<102, m128, m128, m128>;
using mat_mul64_t = fredcpp_function<103, m64, m64, m64>;

template <int size, itype value>
inline auto fill_vec(fred_vec<size> &v) {
    return std::fill(std::begin(v), std::end(v), value);
}

template <int size, itype value>
inline auto fill_mat(fred_mat<size> &v) {
    return std::for_each(std::begin(v), std::end(v), fill_vec<size, value>);
}

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

template <typename fred_fun_t, typename Fill0, typename Fill1,
          typename Expected, typename Dump>
std::pair<struct timespec, bool> fred_speedtest(fredcpp &handle, Fill0 fill0,
                                                Fill1 fill1, Expected expected,
                                                Dump dump) {
    fred_fun_t fun(handle);

    // Fill arguments
    fill0(fun.template get<1>());
    fill1(fun.template get<2>());
    // Third is output

    struct timespec before = {0, 0};
    struct timespec after = {0, 0};
    struct timespec difference;

    // Execute fred call
    clock_gettime(CLOCK_MONOTONIC_RAW, &before);
    fun();
    clock_gettime(CLOCK_MONOTONIC_RAW, &after);

    // Calculate elapsed time
    difference = diff_timespec(&after, &before);
    std::printf("That took: %lld.%.9ld seconds\n", (long long)difference.tv_sec,
                difference.tv_nsec);

    // Check that output is correct
    bool check = expected(fun.template get<0>());
    if (check) {
        std::printf("Output matches expected value!\n");
    } else {
        std::printf("Output DOES NOT MATCH expected value!\n");
    }

    dump(fun.template get<1>());
    dump(fun.template get<2>());
    std::printf("--------------\n");
    dump(fun.template get<0>());

    return std::make_pair(difference, check);
}

template <int size, itype value>
bool check_vec(const fred_vec<size> &v) {
    return std::all_of(std::begin(v), std::end(v),
                       [](const itype &vv) { return vv == value; });
}

template <int size, int max = size>
void dump_vec(const fred_vec<size> &v) {
    static_assert(max <= size, "Cannot print past the array size!");

    std::printf("[ ");
    std::for_each(std::begin(v), std::begin(v) + max,
                  [](const itype &vv) { std::printf("%d ", vv); });

    if constexpr (max < size) {
        std::printf("...");
    }
    std::printf("]\n");
}

int main(int argc, char *argv[]) {
    fredcpp handle;

    if (argc != 2) {
        std::fprintf(stderr, "Must provide ONE of the following arguments: "
                             "sum_vec, xor_vec, mul64, mul128\n");
        return EXIT_FAILURE;
    }

    std::string invoke = argv[1];

    if (invoke == "sum_vec") {
        // [2, 2...] + [3, 3...] = [5, 5...]
        (void)fred_speedtest<sum_vec_t>(handle, fill_vec<1024, 2>,
                                        fill_vec<1024, 3>, check_vec<1024, 5>,
                                        dump_vec<1024, 10>);
    } else if (invoke == "xor_vec") {
        // [2, 2...] ^ [3, 3...] = [1, 1...]
        (void)fred_speedtest<xor_vec_t>(handle, fill_vec<1024, 2>,
                                        fill_vec<1024, 3>, check_vec<1024, 1>,
                                        dump_vec<1024, 10>);
    } else if (invoke == "mat64") {
        printf("TBD\n");
    } else if (invoke == "mat128") {
        printf("TBD\n");
    }

    return EXIT_SUCCESS;
}
