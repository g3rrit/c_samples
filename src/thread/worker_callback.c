#include "worker_callback.h"

#include "mutex.h"

mutex_declare(cb_request_is_active_lock);
mutex_declare(cb_request_get_data_lock);

int callback_mutex_init()
{
   mutex_init(cb_request_is_active_lock);
   mutex_init(cb_request_get_data_lock);
   return 1;
}

int callback_mutex_delete()
{
    mutex_delete(cb_request_is_active_lock);
    mutex_delete(cb_request_get_data_lock);
    return 1;

}

void *worker_callback(struct worker *this, void *data, int CB_TYPE)
{
    void *retval = 0;
    switch(CB_TYPE)
    {
        //------------------------------------------
        case CB_REQUEST_IS_ACTIVE:
            mutex_lock(cb_request_is_active_lock);
            retval = this->callback(this, data);
            mutex_unlock(cb_request_is_active_lock);
            return retval;

        //------------------------------------------
        case CB_REQUEST_GET_DATA:
            mutex_lock(cb_request_get_data_lock);
            retval = this->callback(this, data);
            mutex_unlock(cb_request_get_data_lock);
            return retval;


        default:
            set_error(256);
    }
    return 0;
}
