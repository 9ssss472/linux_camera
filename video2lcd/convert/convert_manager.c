#include  "convert_manager.h"

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


int ConvertOprInit(void)
{
    int ret = 0;
    
    return ret;
}

