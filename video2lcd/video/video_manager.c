#include  "video_manager.h"

PT_VideoOpr g_ptVideoOprHead = NULL;

int RegisterVideoOpr(PT_VideoOpr ptVideoOpr)
{
	PT_VideoOpr ptTmp;

	if (!g_ptVideoOprHead)
	{
		g_ptVideoOprHead   = ptVideoOpr;
		ptDispOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptVideoOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptVideoOpr;
		ptDispOpr->ptNext = NULL;
	}

	return 0;
}


int VideoOprInit(void)
{
    int ret = 0;
    ret = RegisterV4L2Opr();
    return ret;
}

