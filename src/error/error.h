#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"

struct error_t 
{
    char *msg;
    void *err_d;
    struct error_t *prev_err;
};

struct result_t 
{
    int is_error;
    union
    {
        void *ok;
        struct error_t *err;
    };
};

typedef struct result_t* result;

#pragma GCC diagnostic ignored "-Wint-to-void-pointer-cast"


#define err(__msg, __err_d)                                                                             \
                                    {                                                                   \
                                        char *_msg = __msg;                                             \
                                        void *_err_d = (void*)__err_d;                                  \
                                        struct result_t *_res = malloc(sizeof(struct result_t));        \
                                        _res->is_error = 1;                                             \
                                        struct error_t *_err = malloc(sizeof(struct error_t));          \
                                        _err->msg = _msg;                                               \
                                        _err->err_d = _err_d;                                           \
                                        _res->err = _err;                                               \
                                        _res->err->prev_err = 0;                                        \
                                        return _res;                                                    \
                                    }

#define ok(__ok)                                                                                        \
                                    {                                                                   \
                                        void *_ok = (void*)__ok;                                        \
                                        struct result_t *_res = malloc(sizeof(struct result_t));        \
                                        _res->is_error = 0;                                             \
                                        _res->ok = _ok;                                                 \
                                        return _res;                                                    \
                                    }

#define try(__ok, __res, __msg, __err_d)                                                                \
                                    {                                                                   \
                                        void *_ok = (void*)__ok;                                        \
                                        struct result_t *_res = __res;                                  \
                                        char *_msg = __msg;                                             \
                                        void *_err_d = (void*)__err_d;                                  \
                                        if(_res->is_error)                                              \
                                        {                                                               \
                                            struct error_t *_n_err = malloc(sizeof(struct error_t));    \
                                            _n_err->msg = _msg;                                         \
                                            _n_err->err_d = _err_d;                                     \
                                            _n_err->prev_err = _res->err;                               \
                                            _res->err = _n_err;                                         \
                                            return _res;                                                \
                                        }                                                               \
                                        else                                                            \
                                        {                                                               \
                                            _ok = _res->ok;                                             \
                                        }                                                               \
                                    }

#define catch(__ok, __res, _for_each)                                                                   \
                                    {                                                                   \
                                        struct result_t *_res = __res;                                  \
                                        void *_ok = (void*)__ok;                                        \
                                        if(_res->is_error)                                              \
                                        {                                                               \
                                            struct error_t *_err = _res->err;                           \
                                            while(_err)                                                 \
                                            {                                                           \
                                                void *err = _err->err_d;                                \
                                                char *msg = _err->msg;                                  \
                                                _for_each;                                              \
                                                struct error_t *_f_err = _err;                          \
                                                _err = _err->prev_err;                                  \
                                                free(_f_err);                                           \
                                            }                                                           \
                                            free(_res);                                                 \
                                        }                                                               \
                                        else                                                            \
                                        {                                                               \
                                            _ok = _res->ok;                                             \
                                        }                                                               \
                                    }                                   
                                            


                




#endif

