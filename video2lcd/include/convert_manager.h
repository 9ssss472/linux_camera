#ifndef CONVERT_MANAGER_H
#define CONVERT_MANAGER_H

#include "video_manager.h"
#include <linux/videodev2.h>

typedef struct ConvetrOpr
{
    char *name;
    int (*isSupported)(int formatSrc, int formatDest);
    int (*convertFormat)(PT_PixelDataset ptSource, PT_PixelDataset ptConvert);
    int (*ConvertExit)(PT_PixelDataset ptSource);
    struct ConvetrOpr * ptNext;
}T_ConvertOpr, *PT_ConvertOpr;

int RegisterConvertOpr(PT_ConvertOpr ptConvertOpr);
int ConvertOprInit(void);
int Yuv2RgbInit(void);
int Rgb2RgbInit(void);
int registerMjpeg2rgb(void);
int StartFormatConvert(PT_PixelDataset PixelDataIn, PT_PixelDataset PixelDataOut);
#endif // !CONVERT_MANAGER_H
