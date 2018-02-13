/*
 * This is an example Programm
 * First download and install the SDK for the GPU/CPU
 * compile with:    gcc -I"C:\Program Files (x86)\AMD APP SDK\3.0\include" main.c -o main.exe -L"C:\Program Files (x86)\AMD APP SDK\3.0\lib\x86" -lOpenCL
*/ 

#include <stdio.h>
#include <stdlib.h>
 
#ifdef _WIN32
#include <CL/cl.h>
#elif
#include <OpenCL/opencl.h>
#endif

//enable double precision float values
#pragma OPENCL Extension cl_khr_fp64 : enable

const char *kernel_source = 
"#pragma OPENCL Extension cl_khr_fp64 : enable      \n"\
"__kernel void mult(__global double *v)             \n"\
"{                                                  \n"\
"   int id;                                         \n"\
"   id = get_global_id(0);                          \n"\
"   v[id] = 2 * v[id];                              \n"\
"}                                                  \n";

void print_cl_error(cl_int err, char *err_txt);

int main(int argc, char **argv)
{
    unsigned int n = 128;
    size_t n_bytes = n * sizeof(double);

    //openCL declarations
    cl_platform_id platform;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel k_mult;

    //host version of v
    double *h_v = malloc(n_bytes);
    for(int i = 0; i < n; i++)
        h_v[i] = i;

    size_t global_size = n;
    size_t local_size = 32;

    //setup openCL
    cl_int err;
#define printerr(errmsg) if(err != CL_SUCCESS) print_cl_error(err, errmsg)
    
    err = clGetPlatformIDs(1, &platform, 0);
    printerr("platform ids");
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, 0); 
    printerr("device ids");
    context = clCreateContext(0, 1, &device_id, 0, 0, &err);
    printerr("create context");
    //queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
    queue = clCreateCommandQueue(context, device_id, 0, &err);
    printerr("create command queue");
    program = clCreateProgramWithSource(context, 1, (const char **) &kernel_source, 0, &err);
    printerr("create program with source");

    //build the program executable
    if(err != CL_SUCCESS)
    {
        printf("building programm failed!\n");
        if(err == CL_BUILD_PROGRAM_FAILURE)
        {
            size_t log_size;
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, 0, &log_size);
            char *log = malloc(log_size);
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, 0);
            printf("%s\n", log);
            free(log);
        }
    }

    k_mult = clCreateKernel(program, "mult", &err);
    printerr("create kernel");

    //create array on context and write to them
    cl_mem d_v;
    d_v = clCreateBuffer(context, CL_MEM_READ_WRITE, n_bytes, 0, 0);
    err = clEnqueueWriteBuffer(queue, d_v, CL_TRUE, 0, n_bytes, h_v, 0, 0, 0);
    printerr("enqueue write buffer");
    clFinish(queue);

    err = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &d_v);
    printerr("set kernel args");
    err = clEnqueueNDRangeKernel(queue, k_mult, 1, 0, &global_size, &local_size, 0, 0 ,0);
    printerr("enqueue kernel args");
    clFinish(queue);

    //transfer back
    clEnqueueReadBuffer(queue, d_v, CL_TRUE, 0, n_bytes, h_v, 0, 0, 0);
    clFinish(queue);

    int correct = 1;
    for(int i = 0; i < n; i++)
    {
        //printf("h_v[%i] : %f\n", i, h_v[i]);
        if(h_v[i] != (double) 2*i)
            correct = 0;
    }

    if(correct)
        printf("calculation worked!\n");
    else
        printf("calculation failed!\n");


    //release OpenCL resources
    clReleaseMemObject(d_v);
    clReleaseProgram(program);
    clReleaseKernel(k_mult);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    //relase host memory
    free(h_v);

    return 0;
}

void print_cl_error(cl_int err, char *err_txt)
{
    printf("cl error: --> %s:\n -- ", err_txt);
    switch(err)
    {
        case CL_INVALID_CONTEXT:
            printf("(CL_INVALID_CONTEXT) context is not a valid context\n");
            break;
        case CL_INVALID_DEVICE:
            printf("(CL_INVALID_DEVICE) device is not a valid device or is not associated with context\n");
            break;
        case CL_INVALID_QUEUE_PROPERTIES:
            printf("(CL_INVALID_QUEUE_PROPERTIES) values specified in properties are valid but are not supported by the device\n");
            break;
        case CL_OUT_OF_RESOURCES:
            printf("(CL_OUT_OUT_OF_RESOURCES) failure to allocate resources required by the opencl implementation of the device\n");            
            break;
        case CL_OUT_OF_HOST_MEMORY:
            printf("(CL_OUT_OG_HOST_MEMORY) failure to allocate resources required by the opencl implementation of the host\n");
            break;
        case CL_INVALID_PROGRAM:
            printf("(CL_INVALID_PROGRAM) program is not a valid program object\n");
            break;
        case CL_INVALID_PROGRAM_EXECUTABLE:
            printf("(CL_INVALID_PROGRAM_EXECUTABLE) there is no successfully build executable for program\n");
            break;
        case CL_INVALID_KERNEL_NAME:
            printf("(CL_INVALID_KERNEL_NAME) kernel_name is not found in program\n");
            break;
        case CL_INVALID_KERNEL_DEFINITION:
            printf("(CL_INVALID_KERNEL_DEFINITION) the function definition for __kernel function given by kernel_name such as the number of arguments, the argument types are not the same for alll devices for wich the program executable has been built\n");
            break;
        case CL_INVALID_VALUE:
            printf("(CL_INVALID_VALUE) kernel_name is NULL\n");
            break;
        default:
            printf("undefined error\n");
            break;
    }
}
