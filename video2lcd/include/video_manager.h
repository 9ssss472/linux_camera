#ifndef  __VIDEO_MANAGER_H__
#define __VIDEO_MANAGER_H__

#include <stdio.h>

#define VIDEO_PATH "/dev/video1"

typedef struct VideoOpr T_VideoOpr, *PT_VideoOpr;
typedef struct VideoDevice T_VideoDevice, *PT_VideoDevice;

#define FRAME_COUNTS  3;

struct VideoDevice {
    int iFd;
    int pixelFormat;
    int width;
    int height;
    int sizeOfOneFrame;
    int frameCount;
    unsigned char *aucFrameBuffer[FRAME_COUNTS];

    PT_VideoOpr ptVideoOpr;
};

struct VideoOpr {
    char *name;
    
    int (*VideoInit)(PT_VideoDevice ptVideoDevice);
    int (*VideoGetFrame)(PT_VideoDevice ptVideoDevice);
    int (*VideoExit)(PT_VideoDevice ptVideoDevice);
    int (*VideoStart)(PT_VideoDevice ptVideoDevice);
    int (*VideoStop)(PT_VideoDevice ptVideoDevice);
    struct VideoOpr *pNext;

};

int RegisterVideoOpr(PT_VideoOpr ptVideoOpr);
int RegisterV4L2Opr(void);

#endif // __VIDEO_MANAGER_H__