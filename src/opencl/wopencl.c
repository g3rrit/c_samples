//hdr
#ifndef WOPENCL_H
#define WOPENCL_H

#include <stdio.h>
#include <stdlib.h>
 
#ifdef _WIN32
#include <CL/cl.h>
#elif __APPLE__
#include <OpenCL/opencl.h>
#endif

int cl_init(char **programm_s);

int cl_delete();

void print_cl_error(cl_int err, char *err_txt);

#endif

//src
#ifndef WOPENCL_C

#define CONTAINER_C
#include "container.c"
#undef CONTAINER_C

//openCL declarations
cl_platform_id platform;
cl_device_id device_id;
cl_context context;
cl_command_queue queue;
cl_program program;

//opencl kernels
struct vector cl_kernel_v;
//opencl memory buffers
struct vector cl_mem_v;

int cl_init(char **programm_s);
{
    cl_int err;
#define printerr(errmsg) if(err != CL_SUCCESS) {print_cl_error(err, errmsg); return 0;}
    
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
    err = clBuildProgram(program, 0 , 0, 0, 0, 0);

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

    vector_init(&cl_kernel_v);
    vector_init(&cl_mem_v);



    return 1;
}

int cl_delete()
{
    //release OpenCL resources
    for(int i = 0; i < cl_mem_v; i++)
        clReleaseMemObject(vector_at(cl_mem_v, i));
    clReleaseProgram(program);
    for(int i = 0; i < cl_kernel_v; i++)
        clReleaseKernel(vector_at(cl_kernel_v, i));
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 1;
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

#endif
