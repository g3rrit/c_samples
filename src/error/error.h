#ifndef ERROR_H
#define ERROR_H


struct error_t 
{
    char *msg;
    void (*err_h)();
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


#define err(_msg, _err_h)           {                                                                   \\
                                        struct result_t _res = malloc(sizeof(struct result_t));         \\
                                        _res->is_error = 1;                                             \\
                                        struct error_t _err = malloc(sizeof(struct error_t));           \\
                                        _err->msg = _msg;                                               \\
                                        _err->err_h = _err_h;                                           \\
                                        _res->err = _err;                                               \\
                                        _res->err->prev_err = 0;                                        \\
                                        return _res;                                                    \\
                                    }

#define ok(_ok)                     {                                                                   \\
                                        struct result_t _res = malloc(sizeof(struct result_t));         \\
                                        _res->is_error = 0;                                             \\
                                        _res->ok = _ok;                                                 \\
                                        return _res;                                                    \\
                                    }

#define try(_res, _msg, _err_h)     _res->ok;                                                           \\
                                    if(_res->is_error)                                                  \\
                                    {                                                                   \\
                                        struct error_t _n_err = malloc(sizeof(struct error_t));         \\
                                        _n_err->msg = _msg;                                             \\
                                        _n_err->err_h = _err_h;                                         \\
                                        _n_err->prev_err = _res->err;                                   \\
                                        _res->err = _n_err;                                             \\
                                        return _res;                                                    \\
                                    }                                                                   \\

#define catch(_res)                 _res->ok;                                                           \\
                                    char *msg = 0;                                                      \\
                                    void (*err_h)() = 0;                                                \\
                                    if(_res->is_error)                                                  \\
                                    for(msg = _res->err->msg, err_h = _res->err->err_h;


                




#endif

