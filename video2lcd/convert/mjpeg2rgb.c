#include "convert_manager.h"
#include "libjpeg.h"
#include <string.h>

int isSupportMjpeg2rgb(int formatSrc, int formatDest)
{
    if(formatSrc == V4L2_PIX_FMT_JPEG && formatDest == V4L2_PIX_FMT_RGB565)
        return 1;
    
    return 0;
}

int convertLine(int width,int srcBpp, int destBpp, unsigned char * pucSrcData, unsigned char *  pucDestData)
{
    unsigned int * puc32BppData = ( unsigned int *)pucDestData;
    unsigned short * puc16BppData = (unsigned short *)pucDestData;
    int iRed, iGreen, iBlue;
    int i;

    if(srcBpp != 24)
        return -1;

    if(destBpp == 24)
    {
        memcpy(pucDestData, pucSrcData, width*3);
    }
    else
    {
        if(destBpp == 32)
        {
            for(i = 0; i < width; i++)
            {
                iRed = *pucSrcData++;
                iGreen =  *pucSrcData++;
                iBlue =  *pucSrcData++;

                *puc32BppData = iRed << 16 | iGreen << 8 |iBlue;
                puc32BppData++;
            }
        }
        else if(destBpp == 16)
        {
            for(i = 0; i < width; i++)
            {
                iRed = *pucSrcData++;
                iGreen =  *pucSrcData++;
                iBlue =  *pucSrcData++;

                iRed = iRed >> 3;
                iGreen = iGreen >> 2;
                iBlue = iBlue >> 2;

                *puc16BppData = iRed << 11 | iGreen << 5 |iBlue;
                puc16BppData++;
            }
            
        }
    }
    return 0;

}

int convertMjpeg2rgbFormat(T_VideoDevice ptSource, PT_VideoDevice ptConvert)
{


}

T_ConvertOpr tMjpeg2rgbOpr
{
    .name = "mjpeg2rgb"
    .isSupported = isSupportMjpeg2rgb(int formatSrc, int formatDest);
    .convertFormat = convertMjpeg2rgbFormat(T_VideoDevice ptSource, PT_VideoDevice ptConvert);
};

int registerMjpeg2rgb(void)
{
    return  RegisterConvertOpr(&tMjpeg2rgbOpr);
}

