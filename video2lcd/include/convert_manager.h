#ifndef CONVERT_MANAGER_H
#define CONVERT_MANAGER_H

#include "video_manager.h"
#include <linux/videodev2.h>

typedef struct ConvetrOpr
{
    char *name;
    int (*isSupported)(int formatSrc, int formatDest);
    int (*convertFormat)(PT_VideoDevice ptSource, PT_VideoDevice ptConvert);
    struct ConvetrOpr * pNext;
}T_ConvertOpr, *PT_ConvertOpr;

int RegisterConvertOpr(PT_ConvertOpr ptConvertOpr);

#endif // !CONVERT_MANAGER_H
