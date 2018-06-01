#ifndef WORKER_CALLBACK_H
#define WORKER_CALLBACK_H

#include "worker.h"

//only one worker can use one specific type of callback function at a time

#define CB_REQUEST_IS_ACTIVE            0
#define CB_REQUEST_GET_DATA             1

void *worker_callback(struct worker *this, void *data, int CB_TYPE);

int mutex_init();

int mutex_delete();

#endif
