
#include <convert_manager.h>
#include <stdlib.h>
#include <string.h>

static int isSupportRgb2Rgb(int iPixelFormatIn, int iPixelFormatOut)
{
    if (iPixelFormatIn != V4L2_PIX_FMT_RGB565)
        return 0;
    if ((iPixelFormatOut != V4L2_PIX_FMT_RGB565) && (iPixelFormatOut != V4L2_PIX_FMT_RGB32))
    {
        return 0;
    }
    return 1;
}

static int Rgb2RgbConvert(PT_PixelDataset ptVideoBufIn, PT_PixelDataset ptVideoBufOut)
{   
    PT_PixelDatas ptPixelDatasIn  = &ptVideoBufIn->tVideoBuffer;
    PT_PixelDatas ptPixelDatasOut = &ptVideoBufOut->tVideoBuffer;

    int x, y;
    int r, g, b;
    int color;
    unsigned short *pwSrc = (unsigned short *)ptPixelDatasIn->aucPixelDatas;
    unsigned int *pdwDest;

    if (ptVideoBufIn->pixelFormat != V4L2_PIX_FMT_RGB565)
    {
        return -1;
    }

    if (ptVideoBufOut->pixelFormat == V4L2_PIX_FMT_RGB565)
    {
        ptPixelDatasOut->iWidth  = ptPixelDatasIn->iWidth;
        ptPixelDatasOut->iHeight = ptPixelDatasIn->iHeight;
        ptPixelDatasOut->iBpp    = 16;
        ptPixelDatasOut->iLineBytes  = ptPixelDatasOut->iWidth * ptPixelDatasOut->iBpp / 8;
        ptPixelDatasOut->iTotalBytes = ptPixelDatasOut->iLineBytes * ptPixelDatasOut->iHeight;
        if (!ptPixelDatasOut->aucPixelDatas)
        {
            ptPixelDatasOut->aucPixelDatas = malloc(ptPixelDatasOut->iTotalBytes);
        }
        
        memcpy(ptPixelDatasOut->aucPixelDatas, ptPixelDatasIn->aucPixelDatas, ptPixelDatasOut->iTotalBytes);
        return 0;
    }
    else if (ptVideoBufOut->pixelFormat == V4L2_PIX_FMT_RGB32)
    {
        ptPixelDatasOut->iWidth  = ptPixelDatasIn->iWidth;
        ptPixelDatasOut->iHeight = ptPixelDatasIn->iHeight;
        ptPixelDatasOut->iBpp    = 32;
        ptPixelDatasOut->iLineBytes  = ptPixelDatasOut->iWidth * ptPixelDatasOut->iBpp / 8;
        ptPixelDatasOut->iTotalBytes = ptPixelDatasOut->iLineBytes * ptPixelDatasOut->iHeight;
        if (!ptPixelDatasOut->aucPixelDatas)
        {
            ptPixelDatasOut->aucPixelDatas = malloc(ptPixelDatasOut->iTotalBytes);
        }

        pdwDest = (unsigned int *)ptPixelDatasOut->aucPixelDatas;
        
        for (y = 0; y < ptPixelDatasOut->iHeight; y++)
        {
            for (x = 0; x < ptPixelDatasOut->iWidth; x++)
            {
                color = *pwSrc++;
                /* ��RGB565��ʽ����������ȡ��R,G,B */
                r = color >> 11;
                g = (color >> 5) & (0x3f);
                b = color & 0x1f;

                /* ��r,g,bתΪ0x00RRGGBB��32λ���� */
                color = ((r << 3) << 16) | ((g << 2) << 8) | (b << 3);

                *pdwDest = color;
                pdwDest++;
            }
        }
        return 0;
    }

    return -1;
}

static int Rgb2RgbConvertExit(PT_PixelDataset ptVideoBufOut)
{
    if (ptVideoBufOut->tVideoBuffer.aucPixelDatas)
    {
        free(ptVideoBufOut->tVideoBuffer.aucPixelDatas);
        ptVideoBufOut->tVideoBuffer.aucPixelDatas = NULL;
    }
    return 0;
}

/* ���� */
static T_ConvertOpr g_tRgb2RgbConvert = {
    .name        = "rgb2rgb",
    .isSupported   = isSupportRgb2Rgb,
    .convertFormat     = Rgb2RgbConvert,
    .ConvertExit = Rgb2RgbConvertExit,
};


/* ע�� */
int Rgb2RgbInit(void)
{
    return RegisterConvertOpr(&g_tRgb2RgbConvert);
}



