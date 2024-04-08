#include "util.h"

#ifdef WIN64
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

long getTimeMS() {
#ifdef WIN64
    return GetTickCount();
#else
    struct timeval time;
    gettimeofday(&time, nullptr);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
#endif
}
