/*
 * First download and install the SDK for the GPU/CPU
 * compile with:    gcc -I"C:\Program Files (x86)\AMD APP SDK\3.0\include" main.c -o main.exe -L"C:\Program Files (x86)\AMD APP SDK\3.0\lib\x86" -lOpenCL
*/ 

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

int cl_init(const char **program_s);

int cl_create_kernel(char *name);
int cl_create_kernel_at(char *name, int x);

#define BUFFER_R 0
#define BUFFER_W 1
#define BUFFER_RW 2
int cl_create_buffer(int flag, size_t size);
int cl_create_buffer_at(int flag, size_t size, int x);

int cl_read_buffer(int d_buffer, void *buffer, size_t size);
int cl_write_buffer(int d_buffer, void *buffer, size_t size);

int cl_set_kernel_arg(int d_kernel, cl_uint arg_index, size_t arg_size, int d_mem);

int cl_enqueue_kernel(int d_kernel, size_t global_size, size_t local_size);

void cl_finish();

int cl_delete();

//debug:
void print_cl_error(cl_int err, char *err_txt);

void print_cl_mem_kernel();

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

int cl_init(const char **program_s)
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
    program = clCreateProgramWithSource(context, 1, (const char **) program_s, 0, &err);
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

    vector_init(&cl_kernel_v, sizeof(cl_mem*), 10);
    vector_init(&cl_mem_v, sizeof(cl_kernel*), 10);

    return 1;
}

int cl_create_kernel(char *name)
{
    cl_int err;
    cl_kernel *k_temp = malloc(sizeof(cl_kernel));
    *k_temp = clCreateKernel(program, name, &err);
    printerr("create kernel");

    vector_push_back(&cl_kernel_v, k_temp);
    return cl_kernel_v.size - 1;
}

int cl_create_kernel_at(char *name, int x)
{
    cl_int err;
    cl_kernel *k_temp = malloc(sizeof(cl_kernel));
    *k_temp = clCreateKernel(program, name, &err);
    printerr("create kernel");

    vector_set(&cl_kernel_v, x, k_temp);
    return x;
}

int cl_create_buffer(int flag, size_t size)
{
    cl_int err;
    cl_mem_flags clflags;
    if(flag == BUFFER_R)
        clflags = CL_MEM_READ_ONLY;
    else if(flag == BUFFER_W)
        clflags = CL_MEM_WRITE_ONLY;
    else
        clflags = CL_MEM_READ_WRITE;

    cl_mem *d_v = malloc(sizeof(cl_mem));
    *d_v = clCreateBuffer(context, clflags, size, 0, &err);
    printerr("create buffer");

    vector_push_back(&cl_mem_v, d_v);

    return cl_mem_v.size - 1;
}

int cl_create_buffer_at(int flag, size_t size, int x)
{
    cl_int err;
    cl_mem_flags clflags;
    if(flag == BUFFER_R)
        clflags = CL_MEM_READ_ONLY;
    else if(flag == BUFFER_W)
        clflags = CL_MEM_WRITE_ONLY;
    else
        clflags = CL_MEM_READ_WRITE;

    cl_mem *d_v = malloc(sizeof(cl_mem));
    *d_v = clCreateBuffer(context, clflags, size, 0, &err);
    printerr("create buffer");

    vector_set(&cl_mem_v, x, d_v);

    return x;
}

int cl_read_buffer(int d_buffer, void *buffer, size_t size)
{
    cl_int err;
    cl_mem *m_temp = vector_at(&cl_mem_v, d_buffer);
    err = clEnqueueReadBuffer(queue, *m_temp, CL_FALSE, 0, size, buffer, 0, 0, 0);
    printerr("write buffer");
    return 1;
}

int cl_write_buffer(int d_buffer, void *buffer, size_t size)
{
    cl_int err;
    cl_mem *m_temp = vector_at(&cl_mem_v, d_buffer);
    err = clEnqueueWriteBuffer(queue, *m_temp, CL_FALSE, 0, size, buffer, 0, 0, 0);
    printerr("read buffer");
    return 1;
}

int cl_set_kernel_arg(int d_kernel, cl_uint arg_index, size_t arg_size, int d_mem)
{
    cl_int err;
    cl_mem *m_temp = vector_at(&cl_mem_v, d_mem);
    cl_kernel *k_temp = vector_at(&cl_kernel_v, d_kernel);
    err = clSetKernelArg(*k_temp, arg_index, arg_size, m_temp);
    printerr("set kernel arg");
    return 1;
}

int cl_enqueue_kernel(int d_kernel, size_t global_size, size_t local_size)
{
    cl_int err;
    cl_kernel *k_temp = vector_at(&cl_kernel_v, d_kernel);
    err = clEnqueueNDRangeKernel(queue, *k_temp, 1, 0, &global_size, &local_size, 0, 0 ,0);
    printerr("enqueue kernel");
    return 1;
}

void cl_finish()
{
    clFinish(queue);
}

int cl_delete()
{
    //release OpenCL resources
    for(int i = 0; i < cl_mem_v.size; i++)
    {
        cl_mem *m_temp = vector_at(&cl_mem_v, i);
        clReleaseMemObject(*m_temp);
        free(m_temp);
    }
    clReleaseProgram(program);
    for(int i = 0; i < cl_kernel_v.size; i++)
    {
        cl_kernel *k_temp = vector_at(&cl_kernel_v, i);
        clReleaseKernel(*k_temp);
        free(k_temp);
    }
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

void print_cl_mem_kernel()
{
    printf("cl memory buffers:\n");
    for(int i = 0; i < cl_mem_v.size; i++)
    {
        void *temp = vector_at(&cl_mem_v, i); 
        printf("\tcl_mem_v[%i]: %p\n", i, temp); 
    }
    printf("cl kernels:\n");
    for(int i = 0; i < cl_mem_v.size; i++)
    {
        void *temp = vector_at(&cl_kernel_v, i); 
        printf("\tcl_kernel_v[%i]: %p\n", i, temp);
    }
}

#endif
