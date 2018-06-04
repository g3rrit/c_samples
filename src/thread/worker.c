#include "worker.h"

#include "worker_callback.h"
#include "mutex.h"

struct worker *finished_workers[MAX_WORKERS];        
int finished_workers_size = 0;

int workers_active = 0;

int worker_id = 0;

void cleanup_workers();

mutex_declare(m_lock);


int worker_init()
{
    mutex_init(m_lock);

    if(!callback_mutex_init())
        return 0;

    return 1;
}

int worker_delete()
{
    while(workers_active)
    {
        printf("still %i active workers waiting...\n", workers_active);
        sleep_ms(500);
    }
    cleanup_workers();

    mutex_delete(m_lock);

    if(!callback_mutex_delete())
        return 0;

    return 1;
}


int add_worker(int (*fun)(struct worker *this), void *arg, void *(*callback)(struct worker *this, void *data))
{
    cleanup_workers();

    struct thread *new_thread = malloc(sizeof(struct thread));

    struct worker *args = malloc(sizeof(struct worker));
    args->id = worker_id;
    args->this = new_thread;
    args->arg = arg;
    args->callback = callback;

    worker_id++;

    if(!thread_create(new_thread, fun, args))
    {
        set_error(251);
        return 0;
    }

    workers_active++;

    return 1;
}


void worker_finished(struct worker *this)
{
    mutex_lock(m_lock);

    finished_workers[finished_workers_size] = this;

    finished_workers_size++;
    workers_active--;

    if(finished_workers_size >= MAX_WORKERS)
    {
        set_error(252);
        mutex_unlock(m_lock);
        return;
    }
    
    mutex_unlock(m_lock);
}

void cleanup_workers()
{
    mutex_lock(m_lock);

    for(int i = 0; i < finished_workers_size; i++)
    {
        struct worker *temp = finished_workers[i];
        if(!thread_join(temp->this))
            set_error(253);

        free(temp->this);
        free(temp);
    }
    finished_workers_size = 0;

    mutex_unlock(m_lock);
}
