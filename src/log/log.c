//hdr
#ifndef LOG_H
#define LOG_H

#ifdef LOG
#include <stdio.h>

extern char *log_file_url;
extern FILE *log_file;

void _log_set_file(const char *url);

void _log_unset_file();


#define log_out(...)                        \
if(!log_file_url)                           \
{                                           \
    fprintf(stdout, __VA_ARGS__);           \
}                                           \
else                                        \
{                                           \
    log_file = fopen(log_file_url, "a");    \
    fprintf(log_file, __VA_ARGS__);         \
    fclose(log_file);                       \
}           
#define log_set_file(url) _log_set_file(url)
#define log_unset_file()  _log_unset_file()

#else

#define log_set_file(url) ;
#define log_unset_file()  ;
#define log_out(...)      ;

#endif

#endif

//src
#ifndef LOG_C

#ifdef LOG

char *log_file_url = 0;
FILE *log_file = 0;

void _log_set_file(const char *url)
{    
    log_file_url = url;
}

void _log_unset_file()
{
    log_file_url = 0;
}

#endif

#endif
