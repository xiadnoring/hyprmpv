#include <cstdarg>
#include "HyprMpvUtils.h"


void HyprMpv::debug (const char *fmt, ...) {
    va_list args;
    va_start (args, fmt);

    printf("[HyprMpv]: ");

    vprintf (fmt, args);

    printf("\n");

    va_end (args);
}