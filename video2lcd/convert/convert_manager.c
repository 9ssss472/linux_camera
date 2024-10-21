#include  "convert_manager.h"
#include "video_manager.h"

PT_ConvertOpr g_ptConvertOprHead = NULL;

int RegisterConvertOpr(PT_ConvertOpr ptConvertOpr)
{
	PT_ConvertOpr ptTmp;

	if (!g_ptConvertOprHead)
	{
		g_ptConvertOprHead   = ptConvertOpr;
		ptConvertOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptConvertOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptConvertOpr;
		ptConvertOpr->ptNext = NULL;
	}

	return 0;
}

static PT_ConvertOpr GetConverOpr(PT_PixelDataset PixelDataIn, PT_PixelDataset PixelDataOut)
{
	PT_ConvertOpr ptTmp;
	ptTmp = g_ptConvertOprHead;
	while(ptTmp)
	{
		if (ptTmp ->isSupported(PixelDataIn ->pixelFormat, PixelDataOut ->pixelFormat) )
		{
			return ptTmp;
		}

		ptTmp = ptTmp ->ptNext;
	}
	return NULL;
}

int StartFormatConvert(PT_PixelDataset PixelDataIn, PT_PixelDataset PixelDataOut)
{
	int iRet;
	PT_ConvertOpr ptConverOpr = GetConverOpr(PixelDataIn, PixelDataOut);
	if(ptConverOpr == NULL)
	{
		printf("cant get convert opr\r\n");
		return -1;
	}

	iRet = ptConverOpr ->convertFormat(PixelDataIn, PixelDataOut);

	return iRet;
}

int ConvertOprInit(void)
{
    int ret = 0;

	ret = Yuv2RgbInit();

	ret |= Rgb2RgbInit();

	ret |= registerMjpeg2rgb();
    
    return ret;
}

