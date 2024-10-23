#ifndef  __VIDEO_MANAGER_H__
#define __VIDEO_MANAGER_H__

#include <stdio.h>
#include "pic_operation.h"
#include <linux/videodev2.h>

#define VIDEO_PATH "/dev/video1"

typedef struct VideoOpr T_VideoOpr, *PT_VideoOpr;
typedef struct VideoDevice T_VideoDevice, *PT_VideoDevice;
typedef struct VideoBuffer  T_PixelDataset, *PT_PixelDataset;
#define FRAME_COUNTS  3;

struct VideoDevice {
    int iFd;
    int pixelFormat;
    int width;
    int height;
    int sizeOfOneFrame;
    int frameCount;
    int frameIndex;
    unsigned char *aucFrameBuffer[FRAME_COUNTS];

    PT_VideoOpr ptVideoOpr;
};

struct VideoOpr {
    char *name;
    int (*PutFrame)(PT_VideoDevice ptVideoDevice, PT_PixelDataset ptVideoBuf);
    int (*VideoInit)(PT_VideoDevice ptVideoDevice);
    int (*VideoGetFrame)(PT_VideoDevice ptVideoDevice, PT_PixelDataset PT_PixelDatas);
    int (*VideoExit)(PT_VideoDevice ptVideoDevice);
    int (*VideoStart)(PT_VideoDevice ptVideoDevice);
    int (*VideoStop)(PT_VideoDevice ptVideoDevice);
    struct VideoOpr *pNext;

};

struct VideoBuffer {
    T_PixelDatas tVideoBuffer;
    int pixelFormat;

};

int RegisterVideoOpr(PT_VideoOpr ptVideoOpr);
int RegisterV4L2Opr(void);
int VideoDeviceInit(char *strDevName, PT_VideoDevice ptVideoDevice);
int VideoOprInit(void);
PT_VideoOpr GetVideoOpr(char *pcName);

#endif // __VIDEO_MANAGER_H__