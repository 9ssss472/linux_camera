#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <render.h>
#include <string.h>
#include "video_manager.h"

/**********************************************************************
 * 函数名称： FlushVideoMemToDev
 * 功能描述： 把缓冲区中的数据刷到显示设备上去,即在显示设备上显示缓冲区中的图像
 * 输入参数： ptVideoMem - 缓冲区,内含象素数据
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
// void FlushVideoMemToDev(PT_VideoMem ptVideoMem)
// {
// 	//memcpy(GetDefaultDispDev()->pucDispMem, ptVideoMem->tPixelDatas.aucPixelDatas, ptVideoMem.tPixelDatas.iHeight * ptVideoMem.tPixelDatas.iLineBytes);
// 	if (!ptVideoMem->bDevFrameBuffer)
// 	{
// 		GetDefaultDispDev()->ShowPage(ptVideoMem);
// 	}
// }

void FlushVideoMemToDev(PT_PixelDataset ptPixelDataset)
{

		GetDefaultDispDev()->ShowPage(ptPixelDataset);
	
}



