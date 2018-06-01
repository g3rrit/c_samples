#include "thread.h"

#include "error.h"

int thread_create(struct thread *this, int (*fun)(void *arg), void *arg)
{
#ifdef _WIN32 
    this->handle = malloc(sizeof(HANDLE));
#else
    this->handle = malloc(sizeof(struct pthread_t));
#endif

#ifdef _WIN32
    if(!(*this->handle = CreateThread(0, 0, fun, arg, 0, &(this->thread_id))))
    {
        set_error(200);
        return 0;
    }
#else
    if(pthread_create(this->handle, 0, fun, arg))
    {
        set_error(200);
        return 0;
    }
#endif
    return 1;
}

int thread_join(struct thread *this)
{
    int res = 0;
#ifdef _WIN32
    DWORD exit_code;
    for(exit_code = STILL_ACTIVE; exit_code == STILL_ACTIVE; GetExitCodeThread(*(this->handle), &exit_code)) 
        Sleep(100);
    //WaitForSingleObject(*(this->handle), INFINITE);
    CloseHandle(*(this->handle));
    res = (int)exit_code;
#else
    int *ret = malloc(sizeof(int));
    pthread_join(this, &ret);
    res = *ret;
    free(ret);
#endif
    free(this->handle);

    return res;
}

