#ifndef RXS_WINDOWS_COMPAT_H
#define RXS_WINDOWS_COMPAT_H

#ifdef _MSC_VER

#ifndef NOMINMAX
#   define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif

#include <stddef.h>
#include <string.h>
#include <time.h>

#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#ifdef __cplusplus
static inline struct tm *rxs_localtime_r(const time_t *timep, struct tm *result)
#else
static __inline struct tm *rxs_localtime_r(const time_t *timep, struct tm *result)
#endif
{
    return localtime_s(result, timep) == 0 ? result : NULL;
}

#define localtime_r(timep, result) rxs_localtime_r(timep, result)

#endif

#endif
