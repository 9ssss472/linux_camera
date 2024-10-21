#include "convert_manager.h"
#include "libjpeg.h"
#include <string.h>
#include <setjmp.h>

typedef stuct TMyErrorMgr {
    struct jpeg_error_mgr jerr;
    jmp_buf jmp_buf;
}T_MyErrorMgr, *PT_MyErrorMgr;

void my_error_exit(struct jpeg_decompress_struct *cinfo)
{
     static char errStr[JMSG_LENGTH_MAX];
    
	PT_MyErrorMgr ptMyErr = (PT_MyErrorMgr)ptCInfo->err;

    /* Create the message */
    (*ptCInfo->err->format_message) (ptCInfo, errStr);
    DBG_PRINTF("%s\n", errStr);

    longjmp(ptMyErr->jmp_buf,);
}

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

int convertMjpeg2rgbFormat(PT_PixelDataset ptSource, PT_PixelDataset ptConvert)
{
    T_MyErrorMgr tJerr;
    sturct jpeg_decompress_struct cinfo;
    unsigned char * pucLineBuffer;
    int i;
    unsigned char * pucConvertLineBuffer;

    if(setjmp(tJerr.jmp_buf))
    {
        jpeg_destroy_decompress(&cinfo);
        if(pucLineBuffer)
            free(pucLineBuffer);

        if(ptConvert ->tVideoBuffer.aucPixelDatas)
            free(ptConvert ->tVideoBuffer.aucPixelDatas);

        return -1; 

    }

    jpeg_create_decompress(&cinfo);
    
    jpeg_mem_src_tj(&cinfo, ptSource ->tVideoBuffer.aucPixelDatas, ptSource ->tVideoBuffer.iTotalBytes);

    jpeg_read_header(&cinfo, true);

    cinfo.out_color_space = JCS_RGB;
    cinfo.scale_num=1;
    cinfo.scale_denom=2;

    jpeg_start_decompress(&cinfo);

    ptConvert ->tVideoBuffer.iHeight = cinfo.out_height;
    ptConvert ->tVideoBuffer.iWidth = cinfo.out_width;
    ptConvert ->tVideoBuffer.iLineBytes = cinfo.out_width *ptConvert->tVideoBuffer.iBpp / 8;
    ptConvert ->tVideoBuffer.iTotalBytes =  ptConvert ->tVideoBuffer.iLineBytes * cinfo.out_height;
    if(ptConvert ->tVideoBuffer.aucPixelDatas == NULL)
    {
        ptConvert ->tVideoBuffer.aucPixelDatas = malloc(ptConvert ->tVideoBuffer.iTotalBytes);
        if(ptConvert ->tVideoBuffer.aucPixelDatas == (void *) -1)
        {
            perror("ptConvert ->tVideoBuffer.aucPixelDatas");
            return -1;
        }
    }

    pucConvertLineBuffer = ptConvert ->tVideoBuffer.aucPixelDatas;


    pucLineBuffer = malloc(cinfo.output_width * cinfo.output_components);
    if(pucLineBuffer == (void *) -1)
    {
        perror("malloc pucLineBuffer:");
        return -1;
    }

    while(cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanline(&cinfo, &pucLineBuffer,1);

        convertLine(cinfo.output_width, 24, ptConvert->tVideoBuffer.iBpp , pucLineBuffer, pucConvertLineBuffer);
        pucConvertLineBuffer +=  ptConvert ->tVideoBuffer.iLineBytes;

    }
    
    free(pucLineBuffer);
    jpeg_finish_decompress(&cinfo);
    jpeg_decompress_destroy(&cinfo);

    return 0;
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

