#define LOG_C
#include "log.c"
#undef LOG_C

int main()
{
    log_set_file("lo.txt");

    log_out("test %i\n", 10);
    log_out("test %i\n", 20);
    log_out("test %i\n", 30);

    log_unset_file();

    log_out("testunset: %i\n", 20);
    log_out("testunset: %i\n", 30);
    log_out("testunset: %i\n", 40);

    return 0;
}
