#include "fredcpp.h"

#include <algorithm>
#include <iostream>

// From the original implementation of the IPs, all data is 64 bits wide
using itype = int64_t;

template <int size>
using fred_vec = std::array<itype, size>;

template <int size>
using fred_mat = std::array<std::array<itype, size>, size>;

template <int FredID, int size>
class vec_fun {
    using Fun = fredcpp_function<FredID, fred_vec<size>, fred_vec<size>,
                                 fred_vec<size>>;

    Fun fun;

public:
    vec_fun(fredcpp & handle) : fun(handle) {}

    auto &in1() {
        return fun.template get<1>();
    }

    auto &in2() {
        return fun.template get<2>();
    }

    auto &out() {
        return fun.template get<0>();
    }

    void operator()() {
        return fun();
    }
};

template <int FredID, int size>
class mat_fun {
    using Fun = fredcpp_function<FredID, fred_mat<size>, fred_mat<size>,
                                 fred_mat<size>>;

    Fun fun;

public:
    mat_fun(fredcpp &handle) : fun(handle) {}

    auto &in1() {
        return fun.template get<0>();
    }

    auto &in2() {
        return fun.template get<1>();
    }

    auto &out() {
        return fun.template get<2>();
    }

    void operator()() {
        return fun();
    }
};

using vec_sum_t = vec_fun<100, 1024>;
using vec_xor_t = vec_fun<101, 1024>;
using mat_mul128_t = mat_fun<102, 128>;
using mat_mul64_t = mat_fun<103, 64>;

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

template <typename fred_fun_t, typename Fill1, typename Fill2,
          typename Expected, typename Dump>
std::pair<struct timespec, bool> fred_speedtest(fredcpp &handle, Fill1 fill1,
                                                Fill2 fill2, Expected expected,
                                                Dump dump) {
    fred_fun_t fun(handle);

    // Fill arguments
    fill1(fun.in1());
    fill2(fun.in2());

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
    bool check = expected(fun.out());
    if (check) {
        std::printf("Output matches expected value!\n");
    } else {
        std::printf("Output DOES NOT MATCH expected value!\n");
    }

    dump(fun.in1());
    dump(fun.in2());
    std::printf("--------------\n");
    dump(fun.out());

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
                  [](const itype &vv) { std::printf("%ld ", vv); });

    if constexpr (max < size) {
        std::printf("...");
    }
    std::printf("]\n");
}

constexpr fred_mat<4> inA = {
    1, 2, 3, 4, //
    2, 3, 4, 5, //
    3, 4, 5, 6, //
    4, 5, 6, 7,
};

constexpr fred_mat<4> inB = {
    11, 12, 13, 14, //
    12, 13, 14, 15, //
    13, 14, 15, 16, //
    14, 15, 16, 17,
};

constexpr fred_mat<4> outC = {
    130, 140, 150, 160, //
    180, 194, 208, 222, //
    230, 248, 266, 284, //
    280, 302, 324, 346,
};

template <int size>
void fill_mat(fred_mat<size> &matrix, const fred_mat<4> &in) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] = in[i][j];
        }
    }

    // Zero everywhere else!
    for (int i = 4; i < size; ++i) {
        for (int j = 4; j < size; ++j) {
            matrix[i][j] = 0;
        }
    }
}

template <int size>
bool check_mat(const fred_mat<size> &matrix, const fred_mat<4> &out) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (matrix[i][j] != out[i][j])
                return false;
        }
    }

    // Cannot calculate by hand output outside the first 4x4 submatrix!
    return true;
}

template <int size>
void dump_mat(const fred_mat<size> &matrix) {
    std::printf("[ ");
    for (int i = 0; i < 4; ++i) {
        if (i > 0) {
            std::printf("\n  ");
        }

        for (int j = 0; j < 4; ++j) {
            std::printf("%ld ", matrix[i][j]);
        }
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
        (void)fred_speedtest<vec_sum_t>(handle, fill_vec<1024, 2>,
                                        fill_vec<1024, 3>, check_vec<1024, 5>,
                                        dump_vec<1024, 10>);
    } else if (invoke == "xor_vec") {
        // [2, 2...] ^ [3, 3...] = [1, 1...]
        (void)fred_speedtest<vec_xor_t>(handle, fill_vec<1024, 2>,
                                        fill_vec<1024, 3>, check_vec<1024, 1>,
                                        dump_vec<1024, 10>);
    } else if (invoke == "mat64") {
        using m64 = fred_mat<64>;

        auto fill_A = [](m64 &m) { fill_mat<64>(m, inA); };
        auto fill_B = [](m64 &m) { fill_mat<64>(m, inB); };
        auto check_C = [](m64 &m) { return check_mat<64>(m, outC); };

        (void)fred_speedtest<mat_mul64_t>(handle, fill_A, fill_B, check_C,
                                          dump_mat<64>);
    } else if (invoke == "mat128") {
        using m128 = fred_mat<128>;

        auto fill_A = [](m128 &m) { fill_mat<128>(m, inA); };
        auto fill_B = [](m128 &m) { fill_mat<128>(m, inB); };
        auto check_C = [](m128 &m) { return check_mat<128>(m, outC); };

        (void)fred_speedtest<mat_mul128_t>(handle, fill_A, fill_B, check_C,
                                           dump_mat<128>);
    } else {
        std::fprintf(stderr, "Must provide ONE of the following arguments: "
                             "sum_vec, xor_vec, mul64, mul128\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
