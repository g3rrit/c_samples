#ifndef LOG_H
#define LOG_H

#ifdef LOG
#include <stdio.h>

#ifdef LOG_FILE
#define log_out(...) FILE *log_out_file = fopen(#LOG_FILE, "a");    \
    fprintf(stdout, __VA_ARGS__);                                   \
    fclose(log_out_file);
#else
#define log_out(...) fprintf(stdout, __VA_ARGS__); 
#endif

#else
#define log_out(...) ;  

#endif


#endif
