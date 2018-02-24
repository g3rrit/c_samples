#include "snow.h"

#define WOPENCL_C
#include "wopencl.c"
#undef WOPENCL_C

const char *kernel_source = "\n"\
"__kernel void mult(__global float *v)             \n"\
"{                                                  \n"\
"   int id;                                         \n"\
"   id = get_global_id(0);                          \n"\
"   v[id] = 2 * v[id];                              \n"\
"}                                                  \n";


describe(opencl,
{
    it("init",
    {
        cl_init(&kernel_source);
        cl_delete(&kernel_source);
    });

    it("create kernel",
    {
        cl_init(&kernel_source);
        int pos = cl_create_kernel("mult");
        assert(!pos);
        cl_delete(&kernel_source);
    });

    it("create buffer",
    {
        cl_init(&kernel_source);
        int pos = cl_create_kernel("mult");
        size_t bsize = sizeof(float) * 32;
        int bpos = cl_create_buffer(BUFFER_RW, bsize);
        assert(!bpos);
        cl_delete(&kernel_source);
    });

    it("read write buffer",
    {
        cl_init(&kernel_source);
        int pos = cl_create_kernel("mult");
        size_t bsize = sizeof(float) * 32;
        int bpos = cl_create_buffer(BUFFER_RW, bsize);

        print_cl_mem_kernel();

        asserteq(pos, 0);
        asserteq(bpos, 0);
        
        float *h_v = malloc(bsize);
        for(int i = 0; i < 32; i++)
            h_v[i] = i;

        cl_write_buffer(bpos, h_v, bsize);
        cl_finish();

        for(int i = 0; i < 32; i++)
            h_v[i] = 0;

        cl_read_buffer(bpos, h_v, bsize);
        cl_finish();

        for(int i = 0; i < 32; i++)
            asserteq(h_v[i], i);

        cl_delete(&kernel_source);
    });

    it("read write buffer at",
    {
        cl_init(&kernel_source);
        int pos = cl_create_kernel_at("mult", 3);
        size_t bsize = sizeof(float) * 32;
        int bpos = cl_create_buffer_at(BUFFER_RW, bsize, 3);

        printf("bpos %i\n", bpos);
        print_cl_mem_kernel();
        
        float *h_v = malloc(bsize);
        for(int i = 0; i < 32; i++)
            h_v[i] = i;

        cl_write_buffer(bpos, h_v, bsize);
        cl_finish();

        for(int i = 0; i < 32; i++)
            h_v[i] = 0;

        cl_read_buffer(bpos, h_v, bsize);
        cl_finish();

        for(int i = 0; i < 32; i++)
            asserteq(h_v[i], i);

        cl_delete(&kernel_source);
    });


    it("run kernel",
    {
        cl_init(&kernel_source);
        int pos = cl_create_kernel("mult");
        size_t bsize = sizeof(float) * 32;
        int bpos = cl_create_buffer(BUFFER_RW, bsize);
        
        float *h_v = malloc(bsize);
        for(int i = 0; i < 32; i++)
            h_v[i] = i;

        cl_write_buffer(bpos, h_v, bsize);
        cl_finish();

        cl_set_kernel_arg(pos, 0, sizeof(cl_mem), bpos);
        cl_enqueue_kernel(pos, 32, 4);
        cl_finish();

        cl_read_buffer(bpos, h_v, bsize);
        cl_finish();

        for(int i = 0; i < 32; i++)
            asserteq(h_v[i], 2*i);

        cl_delete(&kernel_source);
    });


});

snow_main();

