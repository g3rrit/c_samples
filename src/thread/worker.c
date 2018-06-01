#include "worker.h"

#include "worker_callback.h"

struct worker *finished_workers[MAX_WORKERS];        
int finished_workers_size = 0;

int workers_active = 0;

int worker_id = 0;

void cleanup_workers();

#ifdef _WIN32
#include <windows.h>
HANDLE m_lock;
#else
#include <unistd.h>     //for sleeping
pthread_mutex_t m_lock;
#endif

int worker_init()
{
#ifdef _WIN32
    m_lock = CreateMutex(0, FALSE, 0);
    if(!m_lock)
    {
        set_error(254);
        return 0;
    } 
#else
    if(pthread_mutex_init(&m_lock, 0))
    {
        set_error(254);
        return 0;
    }
#endif
    if(!mutex_init())
        return 0;

    return 1;
}

int worker_delete()
{
    while(workers_active)
    {
        printf("still active workers waiting...\n");
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }
    cleanup_workers();
#ifdef _WIN32
    CloseHandle(m_lock);
#else
    if(pthread_mutex_destroy(&m_lock))
    {
        set_error(255);
        return 0;
    }
#endif
    if(!mutex_delete())
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
#ifdef _WIN32
    WaitForSingleObject(m_lock, INFINITE);
#else
    pthread_mutex_lock(&m_lock);
#endif
    finished_workers[finished_workers_size] = this;

    finished_workers_size++;
    workers_active--;

    if(finished_workers_size >= MAX_WORKERS)
    {
        set_error(252);
        return;
    }
#ifdef _WIN32
    ReleaseMutex(m_lock);
#else
    pthread_mutex_unlock(&m_lock);
#endif
}

void cleanup_workers()
{
#ifdef _WIN32
    WaitForSingleObject(m_lock, INFINITE);
#else
    pthread_mutex_lock(&m_lock);
#endif
    for(int i = 0; i < finished_workers_size; i++)
    {
        struct worker *temp = finished_workers[i];
        if(!thread_join(temp->this))
            set_error(253);

        free(temp->this);
        free(temp);
    }
    finished_workers_size = 0;
#ifdef _WIN32
    ReleaseMutex(m_lock);
#else
    pthread_mutex_unlock(&m_lock);
#endif
}
