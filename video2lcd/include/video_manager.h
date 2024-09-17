#ifndef  __VIDEO_MANAGER_H__
#define __VIDEO_MANAGER_H__

#include <stdio.h>

#define VIDEO_PATH "/dev/video1"

typedef struct VideoOpr {
    int iFd;
    int (*VideoInit)(void);
    int (*VideoGetFrame)(void);
    int (*VideoExit)(void);
    int (*VideoStart)(void);
    int (*VideoStop)(void);
    struct VideoOpr *pNext;

}T_VideoOpr, *PT_VideoOpr;

int RegisterVideoOpr(PT_VideoOpr ptVideoOpr);
int RegisterV4L2Opr(void);

#endif // __VIDEO_MANAGER_H__