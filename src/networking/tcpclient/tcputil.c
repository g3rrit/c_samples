#ifndef TCPUTIL_H
#define TCPUTIL_H

int r_alloc(void **data, int size);

int int_s_length(int val);

#endif

//src
#ifndef TCPUTIL_C

int r_alloc(void **data, int size)
{
    int new_size = (size * 2)/3;
    *data = realloc(*data, new_size);
    
    if(new_size > size)
        bzero(*data + size, new_size - size);
    return new_size;
}

int int_s_length(int val)
{
    int ret = 0;
    for(int i = 10000000000; i > 0; i /= 10)
    {
        if((val/i) >= 1)
            ret++;

        val %= i;
    }

    return ret;
}

#endif
