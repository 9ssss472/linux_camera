#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <encoding_manager.h>
#include <fonts_manager.h>
#include <disp_manager.h>
#include <input_manager.h>
#include <pic_operation.h>
#include "render.h"
#include <string.h>
#include <picfmt_manager.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "video_manager.h"
#include  "convert_manager.h"

int main(int argc, char **argv)
{	
	int iError;

	int iX, iY, iBpp;

	PT_VideoDevice ptVideoDevice;

	PT_VideoMem ptVideoMem;

	PT_VideoOpr ptVideoOpr;

	T_PixelDataset tPixelDataset;

	T_PixelDataset tFramBuffer;

	T_PixelDataset tConvertBuffer;

	T_PixelDataset tTmpBuffer;

	T_PixelDataset tZoomBuffer;

	/*显示器初始化*/
	DisplayInit();

	SelectAndInitDefaultDispDev("fb");

	/*摄像头初始化*/
	VideoOprInit();

	VideoDeviceInit("v4l2",ptVideoDevice);

	/*转换格式初始化*/
	ConvertOprInit();

	GetVideoBufForDisplay(&tFramBuffer);

	GetDispResolution(iX, iY, iBpp);

	ptVideoOpr = GetVideoOpr("v4l2");

	ptVideoOpr ->VideoStart(ptVideoDevice);

	while(1)
	{
		ptVideoOpr ->VideoGetFrame(ptVideoDevice, &tPixelDataset);

		memcpy(ptVideoMem ->tPixelDatas, tPixelDataset.tVideoBuffer, tPixelDataset.tVideoBuffer.iTotalBytes);

		tTmpBuffer.tVideoBuffer.aucPixelDatas = tPixelDataset.tVideoBuffer.aucPixelDatas;

		if(ptVideoDevice ->pixelFormat != tFramBuffer.pixelFormat)
		{
			iError = StartFormatConvert(&tPixelDataset, &tConvertBuffer);
			if(iError)
			{
				printf("convert error\r\n");
				return -1;
			}

			tTmpBuffer.tVideoBuffer.aucPixelDatas = tPixelDataset.tVideoBuffer.aucPixelDatas;
		}

		if(tTmpBuffer.tVideoBuffer.iWidth != iX, || tTmpBuffer.tVideoBuffer.iHeight != iY)
		{
			PicZoom(&tTmpBuffer.tVideoBuffer, &tZoomBuffer.tVideoBuffer);
		}



		FlushVideoMemToDev(ptVideoMem ->tPixelDatas);



	}



	return 0;
}

