#include "worker_callback.h"

#ifdef _WIN32
#include <windows.h>
HANDLE cb_request_is_active_lock;                   // 0
HANDLE cb_request_get_data_lock;                    // 1
#else
#include <unistd.h>     //for sleeping
pthread_mutex_t cb_request_is_active_lock;
pthread_mutex_t cb_request_get_data_lock;
#endif

#define win_create_mutex(cb_lock)   cb_lock = CreateMutex(0, FALSE, 0);     \
                                    if(!cb_lock)                            \
                                    {                                       \
                                        set_error(254);                     \
                                        return 0;                           \
                                    }                                       

#define uni_create_mutex(cb_lock)   if(pthread_mutex_init(&cb_lock, 0))     \
                                    {                                       \
                                        set_error(254);                     \
                                        return 0;                           \
                                    }

int mutex_init()
{
#ifdef _WIN32
    win_create_mutex(cb_request_is_active_lock);
    win_create_mutex(cb_request_get_data_lock);
#else
    uni_create_mutex(cb_request_is_active_lock);
    uni_create_mutex(cb_request_get_data_lock);
#endif
    return 1;
}

#define win_delete_mutex(cb_lock)   CloseHandle(cb_lock);

#define uni_delete_mutex(cb_lock)   if(pthread_mutex_destroy(&cb_lock))     \
                                    {                                       \
                                        set_error(255);                     \
                                        return 0;                           \
                                    }

int mutex_delete()
{
#ifdef _WIN32
    win_delete_mutex(cb_request_is_active_lock);
    win_delete_mutex(cb_request_get_data_lock);
#else
    uni_delete_mutex(cb_request_is_active_lock);
    uni_delete_mutex(cb_request_get_data_lock);
#endif
    return 1;

}

void *worker_callback(struct worker *this, void *data, int CB_TYPE)
{
    void *retval = 0;
    switch(CB_TYPE)
    {
        //------------------------------------------
        case CB_REQUEST_IS_ACTIVE:
#ifdef _WIN32
            WaitForSingleObject(cb_request_is_active_lock, INFINITE);
#else
            pthread_mutex_lock(&cb_request_is_active_lock);
#endif
            retval = this->callback(this, data);
#ifdef _WIN32
            ReleaseMutex(cb_request_is_active_lock);
#else
            pthread_mutex_unlock(&cb_request_is_active_lock);
#endif
            return retval;

        //------------------------------------------
        case CB_REQUEST_GET_DATA:
#ifdef _WIN32
            WaitForSingleObject(cb_request_get_data_lock, INFINITE);
#else
            pthread_mutex_lock(&cb_request_get_data_lock);
#endif
            retval = this->callback(this, data);
#ifdef _WIN32
            ReleaseMutex(cb_request_get_data_lock);
#else
            pthread_mutex_unlock(&cb_request_get_data_lock);
#endif
            return retval;


        default:
            set_error(256);
    }
    return 0;
}
