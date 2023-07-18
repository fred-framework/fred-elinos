// #include <array>
// #include <chrono>
// #include <cmath>
// #include <fstream>
// #include <iostream>
// #include <sstream>
// #include <string>
// #include <thread>
// #include <vector>
// #include <CL_2_0/CL/cl.hpp>
// #include "hellogauss.h"

// Assume GAUSS_SIZE is defined (TODO: change to something better)
// Assume GAUSS_MSIZE is defined

#pragma once
#ifndef OPENCL_MATMUL_H
#define OPENCL_MATMUL_H

#if RTDAG_OPENCL_SUPPORT == OFF

void opencl_init() {}

static void gauss_multiply_cl(const double in1[GAUSS_MSIZE],
                           const double in2[GAUSS_MSIZE],
                           double out[GAUSS_MSIZE]) {

}

static inline bool gauss_is_equal(double x, double y) {
    const double epsilon = 1e-6;
    return gauss_absd(x - y) <= epsilon * gauss_absd(x);
}

static bool gauss_is_identity(const double in[GAUSS_MSIZE]) {

#endif

#include <CL/cl.hpp>
#include <vector>
#include <iosfwd>

#define OPENCL_COUT (std::cout << "OpenCL: ")
#define OPENCL_CERR (std::cerr << "OpenCL: ERROR: ")

cl::Platform cl_get_platform() {
    // Get all valid platforms
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() < 1) {
        OPENCL_CERR << "No platforms!\n";
        exit(EXIT_FAILURE);
    }

    // Assume that the first platform is the one we are interested in:
    auto cl_platform = all_platforms[0];
    OPENCL_COUT << "Selecting platform\t"
                << cl_platform.getInfo<CL_PLATFORM_NAME>() << '\n';

    return cl_platform;
}

cl::Device cl_get_device(const cl::Platform &pl) {
    // Get all valid devices
    std::vector<cl::Device> all_devices;
    pl.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
    if (all_devices.size() < 1) {
        OPENCL_CERR << "No GPU devices!\n";
        exit(EXIT_FAILURE);
    }

    // Assume that the first device is the one we are interested in:
    auto cl_device = all_devices[0];
    OPENCL_COUT << "Selecting device\t" << cl_device.getInfo<CL_DEVICE_NAME>()
                << '\n';

    return cl_device;
}

std::string readfile(const std::string &source_file) {
    std::ifstream istream(source_file);
    std::stringstream string_buffer;
    string_buffer << istream.rdbuf();
    return string_buffer.str();
}

cl::Program cl_get_program(const cl::Context &context, const cl::Device &device,
                           const std::string &source_file) {
    // Create our source program to run using OpenCL
    std::string source_string = readfile(source_file);
    auto sources = cl::Program::Sources();
    sources.push_back({source_string.c_str(), source_string.length()});

    // The program will be compiled now
    cl::Program program(context, sources);
    if (program.build({device}) != CL_SUCCESS) {
        OPENCL_CERR << "Could not build: "
                    << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
                    << "\n";
        exit(EXIT_FAILURE);
    }

    return program;
}

template <class DataType>
struct MyMatrixMulCLFunctor {
    cl::CommandQueue queue;
    cl::Kernel kernel;
    int size;
    cl::Buffer buffer_A;
    cl::Buffer buffer_B;
    cl::Buffer buffer_C;

    static constexpr size_t item_size = sizeof(DataType);

    MyMatrixMulCLFunctor(const cl::Context &context, const cl::Device &device,
                         const cl::Program &program, const char *func_name,
                         int size, int repetitions = 1) :
        // Create queue to which we will push commands for the device and a
        // reference to our kernel function
        queue(context, device),
        kernel(program, func_name),
        size(size),
        // Create one buffer per parameter
        buffer_A(context, CL_MEM_READ_WRITE, item_size * size * size),
        buffer_B(context, CL_MEM_READ_WRITE, item_size * size * size),
        buffer_C(context, CL_MEM_READ_WRITE, item_size * size * size) {
        int argcnt = 0;

        // Bind kernel arguments to buffers
        kernel.setArg(argcnt++, size);
        kernel.setArg(argcnt++, buffer_A);
        kernel.setArg(argcnt++, buffer_B);
        kernel.setArg(argcnt++, buffer_C);
        // kernel.setArg(argcnt++, repetitions);
    }

    bool operator()(const DataType *A, const DataType *B, DataType *C) {
        // FIXME: error handling

        // Copy input arrays A and B to the device
        // clang-format off
        queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, item_size * size * size, A);
        queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, item_size * size * size, B);
        // clang-format on

        // Invoke kernel and immediately wait for completion
        queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                                   cl::NDRange(size * size), cl::NullRange);
        queue.finish();

        // And we read the result in C from the device
        // clang-format off
        queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, item_size * size * size, C);
        // clang-format on

        return true;
    }
};

int main(int, char *argv[]) {

#ifdef USE_OPENCL
    // Setup platform and program to run on the selected device
    auto cl_platform = cl_get_platform();
    auto cl_device = cl_get_device(cl_platform);
    auto cl_context = cl::Context({cl_device});
    auto cl_program = cl_get_program(cl_context, cl_device, "matrix_mul.clc");
#endif

    constexpr int matrix_size = 100;
    int repetitions;
    auto iss = std::istringstream(std::string(argv[1]));
    iss >> repetitions;

#ifdef USE_OPENCL
    // Create a functor for later use
    auto cl_functor = MyMatrixMulCLFunctor<float>(
        cl_context, cl_device, cl_program, "naive_matrix_mul", matrix_size,
        repetitions);
#endif

    // Create and fill data
    // clang-format off
    std::array<float, matrix_size * matrix_size> A = {
        1, 2, 0, 7,
        2, 1, 0, 3,
        0, 0, 1, 0,
        0, 7, 2, 1,
    };
    std::array<float, matrix_size * matrix_size> B = {
        -0.270270,  0.635135,  0.027027, -0.013514,
        -0.027027,  0.013514, -0.297297,  0.148649,
         0.000000,  0.000000,  1.000000,  0.000000,
         0.189189, -0.094595,  0.081081, -0.040541,
    };
    std::array<float, matrix_size * matrix_size> C;
    // clang-format on

    // // Invoke the functor like a regular function on the corresponding
    // // vector variables
    // auto clock = std::chrono::high_resolution_clock();

    // // Wait 5 seconds
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    auto before = clock.now();
    for (int i = 0; i < repetitions; ++i) {
        cl_functor(A.data(), B.data(), C.data());
    }
    auto after = clock.now();

    OPENCL_COUT << "RESULT: \n";
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            float c = C[i * matrix_size + j];
            c = std::fabs(c) < 0.01 ? 0 : c;
            std::cout << c << '\t';
        }
        std::cout << '\n';
    }

    using std::chrono::duration_cast;
    using std::chrono::microseconds;

    std::cout << "Took " << duration_cast<microseconds>(after - before).count()
              << " us" << '\n';

    return 0;
}
#endif

#endif // OPENCL_MATMUL_H
