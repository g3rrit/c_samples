#include "debug.h"

int _current_fnum = -1;

int _fnum_count = 0;

void _debug_log(int fnum, const char *func_name, const char *file_name, int line)
{
    if(fnum == _current_fnum)
    {
        fprintf(DEBUG_FILE, "!");
        return;
    }
    else
    {
        _current_fnum = fnum;
        fprintf(DEBUG_FILE, "\n");
    }

    fprintf(DEBUG_FILE, "[d]: [%s:%d] | %s | ", file_name, line, func_name);
}
