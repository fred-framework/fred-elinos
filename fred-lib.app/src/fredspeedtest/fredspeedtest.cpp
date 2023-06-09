#include "fredcpp.h"

#include <algorithm>
#include <iostream>

/**
 * Calculate the difference between two timespecs.
 * Returned value is a third timespec containing the difference.
 */
timespec operator-(const timespec &time1, const timespec &time0) {
    timespec diff = {.tv_sec = time1.tv_sec - time0.tv_sec,
                     .tv_nsec = time1.tv_nsec - time0.tv_nsec};
    if (diff.tv_nsec < 0) {
        diff.tv_nsec += 1000000000; // nsec/sec
        diff.tv_sec--;
    }
    return diff;
}

// From the original implementation of the IPs, all data is 64 bits wide
using itype = int64_t;

// Input/Output vectors and matrices have this shape (same as declaring
// pointers, with the added benefit that we know the size in advance)
template <int size>
using fred_vec = std::array<itype, size>;
template <int size>
using fred_mat = std::array<std::array<itype, size>, size>;

/**
 * This class represents a Fred function operating on two vectors and
 * storing the output in a third one.
 *
 * For HW IPs that we have operating on two vectors, the arguments list is
 * the following:
 *  - 0: output
 *  - 1: input 1
 *  - 2: input 2
 *
 * NOTE: adds exactly zero overhead to the calling code when optimizations
 * are enabled in the compiler.
 *
 * @tparam FredID the id of the IP to look up
 * @tparam size the size of the vectors on which it operates
 */
template <int FredID, int size>
class vec_fun {
    // The actual fredcpp_function class, this is just a wrapper
    using Fun = fredcpp_function<FredID, fred_vec<size>, fred_vec<size>,
                                 fred_vec<size>>;
    Fun fun;

public:
    vec_fun(fredcpp &handle) : fun(handle) {}

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
        fun();
    }
};

/**
 * This class represents a Fred function operating on two matrices and
 * storing the output in a third one
 *
 * For HW IPs that we have operating on two matrices, the arguments list is
 * the following:
 *  - 0: input 1 (left operand)
 *  - 1: input 2 (right operand)
 *  - 2: output
 *
 * NOTE: adds exactly zero overhead to the calling code when optimizations
 * are enabled in the compiler.
 *
 * @tparam FredID the id of the IP to look up
 * @tparam size the size of the vectors on which it operates
 */
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

/**
 * The actual function classes. We have one for each IP:
 *
 * - 100: sums two vectors of int64_t of size 1024
 * - 101: xors two vectors of int64_t of size 1024
 *
 * - 102: multiplies two matrices of int64_t of size 128x128
 * - 103: multiplies two matrices of int64_t of size 64x64
 *
 * */
using vec_sum_t = vec_fun<100, 1024>;
using vec_xor_t = vec_fun<101, 1024>;
using mat_mul128_t = mat_fun<102, 128>;
using mat_mul64_t = mat_fun<103, 64>;

/*
 * Fills a vector with the specified value.
 *
 * In this and following functions, the value is supplied as a template
 * parameter in a sort of "bind", which also lets us optimize the code that
 * is generated.
 *
 * Basically, one different function will be generated for each value to
 * fill, and no virtual calls are ever outputed.
 */
template <int size, itype value>
inline auto fill_vec(fred_vec<size> &v) {
    return std::fill(std::begin(v), std::end(v), value);
}

/**
 * @brief Body of the speedtest. This is the same code executed by all
 * speed tests. Using a template to avoid code repetition and favoring
 * compiler optimizations.
 *
 * @tparam fred_fun_t       The type of the function to call, must be one
 *                          of those defined above (like vec_sum_t)
 * @tparam Fill1            The function to call to fill the first input
 *                          operand (passed as a reference)
 * @tparam Fill2            The function to call to fill the second input
 *                          operand (passed as a reference)
 * @tparam Expected         The function called to check the result of the
 *                          operation (passed as a reference), must return
 *                          bool
 * @tparam Dump             The function used to print to screen an operand
 *                          or the output of the operation (assuming all
 *                          three have the same type)
 *
 * @param handle            Reference to the fredcpp handle (maintaining
 *                          the connection to the server)
 * @param fill1             The function to call to fill the first input
 *                          operand (passed as a reference)
 * @param fill2             The function to call to fill the second input
 *                          operand (passed as a reference)
 * @param expected          The function called to check the result of the
 *                          operation (passed as a reference), must return
 *                          bool
 * @param dump              The function used to print to screen an operand
 *                          or the output of the operation (assuming all
 *                          three have the same type)
 *
 * @return std::pair<timespec, bool>
 */
template <typename fred_fun_t, typename Fill1, typename Fill2,
          typename Expected, typename Dump>
std::pair<timespec, bool> fred_speedtest(fredcpp &handle, Fill1 fill1,
                                         Fill2 fill2, Expected expected,
                                         Dump dump) {

    // Instantiate the function to call (will bind also operands to
    // pointers in memory)
    fred_fun_t fun(handle);

    // Fill arguments with input values
    fill1(fun.in1());
    fill2(fun.in2());

    timespec before;
    timespec after;

    // Execute fred call, measuring the exact time it took
    clock_gettime(CLOCK_MONOTONIC_RAW, &before);
    fun();
    clock_gettime(CLOCK_MONOTONIC_RAW, &after);

    // Calculate elapsed time by calling the operator- defined on top
    timespec difference = after - before;
    std::printf("That took: %lld.%.9ld seconds\n", (long long)difference.tv_sec,
                difference.tv_nsec);

    // Check that output is correct
    bool check = expected(fun.out());
    if (check) {
        std::printf("Output matches expected value!\n");
    } else {
        std::printf("Output DOES NOT MATCH expected value!\n");
    }

    // Print operands and output to screen
    dump(fun.in1());
    dump(fun.in2());
    std::printf("--------------\n");
    dump(fun.out());

    // Return time difference and whether the output is correct to the user
    return std::make_pair(difference, check);
}

// ========================= VECTOR OPERATIONS ========================= //

/**
 * Checks whether the vector is filled with value.
 *
 * @return true on success
 */
template <int size, itype value>
bool check_vec(const fred_vec<size> &v) {
    return std::all_of(std::begin(v), std::end(v),
                       [](const itype &vv) { return vv == value; });
}

/**
 * @brief Prints a vector to the screen, up to max numbers.
 *
 * NOTE: max must be less than size. If max is less than size, dots are
 * printed to indicate it to the user.
 */
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

// ======================== MATRICES OPERATIONS ======================== //

// NOTE: Since matrices of 64x64 and 128x128 are too big to check, I
// manually multiplied two 4x4 matrices.
//
// The matrices that will be multiplied will be like this (M is the matrix
// that will be multiplied, while m is the 4x4 portion filled with
// numbers):
//    M =   [m 0]
//          [0 0]
//
// Multiplying a matrix like this by another in the same shape guarantees
// that in the top left corner there is a 4x4 section that is equal to the
// multiplication of the two 4x4 submatrices in the top left corner of the
// two operands.
//
// Hence, only that portion of the output matrix will be checked.

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

// outC = inA * inB
constexpr fred_mat<4> outC = {
    130, 140, 150, 160, //
    180, 194, 208, 222, //
    230, 248, 266, 284, //
    280, 302, 324, 346,
};

/**
 * Fills a matrix with the provided 4x4 submatrix in the top left corner,
 * fills with zeros everywhere else.
 */
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
void fill_mat_A(fred_mat<size> &matrix) {
    fill_mat<size>(matrix, inA);
}

template <int size>
void fill_mat_B(fred_mat<size> &matrix) {
    fill_mat<size>(matrix, inB);
}

/**
 * Checks that the 4x4 top left corner has the same values as the supplied
 * matrix.
 */
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
bool check_mat_C(fred_mat<size> &matrix) {
    return check_mat<size>(matrix, outC);
}

/**
 * Print the 4x4 top left corner of the given matrix to the screen.
 */
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

void help_exit() {
    std::fprintf(stderr, "Must provide ONE of the following arguments: "
                         "sum_vec, xor_vec, mul64, mul128\n");
    exit(EXIT_FAILURE);
}

/**
 * Execute a speed test over several hardware IPs via Fred.
 *
 * Must supply one argument: the name of the IP to test.
 */
int main(int argc, char *argv[]) {
    fredcpp handle;

    if (argc != 2) {
        help_exit();
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
    } else if (invoke == "mul64") {
        (void)fred_speedtest<mat_mul64_t>(handle, fill_mat_A<64>,
                                          fill_mat_B<64>, check_mat_C<64>,
                                          dump_mat<64>);
    } else if (invoke == "mul128") {
        (void)fred_speedtest<mat_mul128_t>(handle, fill_mat_A<128>,
                                           fill_mat_B<128>, check_mat_C<128>,
                                           dump_mat<128>);
    } else {
        help_exit();
    }

    return EXIT_SUCCESS;
}
