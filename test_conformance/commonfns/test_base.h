// Copyright (c) 2023 The Khronos Group Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef TEST_COMMONFNS_BASE_H
#define TEST_COMMONFNS_BASE_H

#include <vector>
#include <map>
#include <memory>

#include <CL/cl_half.h>
#include <CL/cl_ext.h>

#include "harness/deviceInfo.h"
#include "harness/testHarness.h"
#include "harness/typeWrappers.h"


template <typename T>
using VerifyFuncBinary = int (*)(const T *const, const T *const, const T *const,
                                 const int num, const int vs, const int vp);


template <typename T>
using VerifyFuncUnary = int (*)(const T *const, const T *const, const int num);


using half = cl_half;


struct BaseFunctionTest
{
    BaseFunctionTest(cl_device_id device, cl_context context,
                     cl_command_queue queue, int num_elems, const char *fn,
                     bool vsp)
        : device(device), context(context), queue(queue), num_elems(num_elems),
          fnName(fn), vecParam(vsp)
    {}

    // Test body returning an OpenCL error code
    virtual cl_int Run() = 0;

    cl_device_id device;
    cl_context context;
    cl_command_queue queue;

    int num_elems;
    std::string fnName;
    bool vecParam;

    static std::map<size_t, std::string> type2name;
};


struct MinTest : BaseFunctionTest
{
    MinTest(cl_device_id device, cl_context context, cl_command_queue queue,
            int num_elems, const char *fn, bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


struct MaxTest : BaseFunctionTest
{
    MaxTest(cl_device_id device, cl_context context, cl_command_queue queue,
            int num_elems, const char *fn, bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


struct ClampTest : BaseFunctionTest
{
    ClampTest(cl_device_id device, cl_context context, cl_command_queue queue,
              int num_elems, const char *fn, bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


struct DegreesTest : BaseFunctionTest
{
    DegreesTest(cl_device_id device, cl_context context, cl_command_queue queue,
                int num_elems, const char *fn, bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


struct RadiansTest : BaseFunctionTest
{
    RadiansTest(cl_device_id device, cl_context context, cl_command_queue queue,
                int num_elems, const char *fn, bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


struct SignTest : BaseFunctionTest
{
    SignTest(cl_device_id device, cl_context context, cl_command_queue queue,
             int num_elems, const char *fn, bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


struct SmoothstepTest : BaseFunctionTest
{
    SmoothstepTest(cl_device_id device, cl_context context,
                   cl_command_queue queue, int num_elems, const char *fn,
                   bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


struct StepTest : BaseFunctionTest
{
    StepTest(cl_device_id device, cl_context context, cl_command_queue queue,
             int num_elems, const char *fn, bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


struct MixTest : BaseFunctionTest
{
    MixTest(cl_device_id device, cl_context context, cl_command_queue queue,
            int num_elems, const char *fn, bool vsp)
        : BaseFunctionTest(device, context, queue, num_elems, fn, vsp)
    {}

    cl_int Run() override;
};


template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
    int sformat = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    if (sformat <= 0)
        throw std::runtime_error("string_format: string processing error.");
    auto format_size = static_cast<size_t>(sformat);
    std::unique_ptr<char[]> buffer(new char[format_size]);
    std::snprintf(buffer.get(), format_size, format.c_str(), args...);
    return std::string(buffer.get(), buffer.get() + format_size - 1);
}


template <class T>
int MakeAndRunTest(cl_device_id device, cl_context context,
                   cl_command_queue queue, int num_elements,
                   const char *fn = "", bool vsp = false)
{
    auto test_fixture = T(device, context, queue, num_elements, fn, vsp);

    cl_int error = test_fixture.Run();
    test_error_ret(error, "Test Failed", TEST_FAIL);

    return TEST_PASS;
}

#endif // TEST_COMMONFNS_BASE_H
