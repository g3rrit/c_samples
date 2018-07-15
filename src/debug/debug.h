#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#define DEBUG
#define DEBUG_FILE stdout

void _debug_log(int fnum, const char *func_name, const char *file_name, int line);

#ifdef DEBUG
extern int _fnum_count;
#define debug_log()         static int _debug_func_num = 0;                                 \
                            if(_debug_func_num == 0)                                        \
                                _debug_func_num = ++_fnum_count;                            \
                            _debug_log(_debug_func_num, __func__, __FILE__, __LINE__);
#else
#define debug_log()         ;
#endif

#endif
