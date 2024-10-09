#include  "video_manager.h"

PT_VideoOpr g_ptVideoOprHead = NULL;

int RegisterVideoOpr(PT_VideoOpr ptVideoOpr)
{
	PT_VideoOpr ptTmp;

	if (!g_ptVideoOprHead)
	{
		g_ptVideoOprHead   = ptVideoOpr;
		ptVideoOpr->pNext = NULL;
	}
	else
	{
		ptTmp = g_ptVideoOprHead;
		while (ptTmp->pNext)
		{
			ptTmp = ptTmp->pNext;
		}
		ptTmp->pNext	  = ptVideoOpr;
		ptVideoOpr->pNext = NULL;
	}

	return 0;
}

/**********************************************************************
 * 函数名称： GetVideoOpr
 * 功能描述： 根据名字取出指定的"字体模块"
 * 输入参数： pcName - 名字
 * 输出参数： 无
 * 返 回 值： NULL   - 失败,没有指定的模块, 
 *            非NULL - 字体模块的PT_VideoOpr结构体指针
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
PT_VideoOpr GetVideoOpr(char *pcName)
{
	PT_VideoOpr ptTmp = g_ptVideoOprHead;
	
	while (ptTmp)
	{
		if (strcmp(ptTmp->name, pcName) == 0)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->pNext;
	}
	return NULL;
}

int VideoDeviceInit(char *strDevName, PT_VideoDevice ptVideoDevice)
{
    int iError;
	PT_VideoOpr ptTmp ;
	
	ptTmp = GetVideoOpr(strDevName);

	ptTmp ->VideoInit(ptVideoDevice);
	
    return -1;
}


int VideoOprInit(void)
{
    int ret = 0;
    ret = RegisterV4L2Opr();
    return ret;
}

