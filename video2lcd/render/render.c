#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <render.h>
#include <string.h>
#include "video_manager.h"

/**********************************************************************
 * �������ƣ� FlushVideoMemToDev
 * ���������� �ѻ������е�����ˢ����ʾ�豸��ȥ,������ʾ�豸����ʾ�������е�ͼ��
 * ��������� ptVideoMem - ������,�ں���������
 * ��������� ��
 * �� �� ֵ�� 0 - �ɹ�, ����ֵ - ʧ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  Τ��ɽ	      ����
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



