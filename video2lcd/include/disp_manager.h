
#ifndef _PIC_OPERATION_H
#define _PIC_OPERATION_H

#include "video_manager.h"
#include <pic_operation.h>


typedef struct Layout {
	int iTopLeftX;
	int iTopLeftY;
	int iBotRightX;
	int iBotRightY;
	char *strIconName;
}T_Layout, *PT_Layout;



typedef enum {
	VMS_FREE = 0,
	VMS_USED_FOR_PREPARE,
	VMS_USED_FOR_CUR,	
}E_VideoMemState;

typedef enum {
	PS_BLANK = 0,
	PS_GENERATING,
	PS_GENERATED,	
}E_PicState;


typedef struct VideoMem {
	int iID;                        /* ID值,用于标识不同的页面 */
	int bDevFrameBuffer;            /* 1: 这个VideoMem是显示设备的显存; 0: 只是一个普通缓存 */
	E_VideoMemState eVideoMemState; /* 这个VideoMem的状态 */
	E_PicState ePicState;           /* VideoMem中内存里图片的状态 */
	T_PixelDatas tPixelDatas;       /* 内存: 用来存储图像 */
	struct VideoMem *ptNext;        /* 链表 */
}T_VideoMem, *PT_VideoMem;

typedef struct DispOpr {
	char *name;              /* 显示模块的名字 */
	int iXres;               /* X分辨率 */
	int iYres;               /* Y分辨率 */
	int iBpp;                /* 一个象素用多少位来表示 */
	int iLineWidth;          /* 一行数据占据多少字节 */
	unsigned char *pucDispMem;   /* 显存地址 */
	int (*DeviceInit)(void);     /* 设备初始化函数 */
	int (*ShowPixel)(int iPenX, int iPenY, unsigned int dwColor);    /* 把指定座标的象素设为某颜色 */
	int (*CleanScreen)(unsigned int dwBackColor);                    /* 清屏为某颜色 */
	int (*ShowPage)( PT_PixelDataset ptPixelDataset );                         /* 显示一页,数据源自ptVideoMem */
	struct DispOpr *ptNext;      /* 链表 */
}T_DispOpr, *PT_DispOpr;

/**********************************************************************
 * 函数名称： RegisterDispOpr
 * 功能描述： 注册"显示模块", 把所能支持的显示设备的操作函数放入链表
 * 输入参数： ptDispOpr - 一个结构体,内含显示设备的操作函数
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int RegisterDispOpr(PT_DispOpr ptDispOpr);

/**********************************************************************
 * 函数名称： ShowDispOpr
 * 功能描述： 显示本程序能支持的"显示模块"
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
void ShowDispOpr(void);

/**********************************************************************
 * 函数名称： DisplayInit
 * 功能描述： 注册显示设备
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int DisplayInit(void);

/**********************************************************************
 * 函数名称： SelectAndInitDefaultDispDev
 * 功能描述： 根据名字取出指定的"显示模块", 调用它的初始化函数, 并且清屏
 * 输入参数： name - 名字
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
void SelectAndInitDefaultDispDev(char *name);

/**********************************************************************
 * 函数名称： GetDefaultDispDev
 * 功能描述： 程序事先用SelectAndInitDefaultDispDev选择了显示模块,
 *            本函数返回该显示模块
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 显示模块的PT_DispOpr结构体指针
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
PT_DispOpr GetDefaultDispDev(void);

/**********************************************************************
 * 函数名称： GetDispResolution
 * 功能描述： 获得所使用的显示设备的分辨率和BPP
 * 输入参数： 无
 * 输出参数： piXres - 存X分辨率
 *            piYres - 存X分辨率
 *            piBpp  - 存BPP
 * 返 回 值： 0  - 成功
 *            -1 - 失败(未使用SelectAndInitDefaultDispDev来选择显示模块)
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int GetDispResolution(int *piXres, int *piYres, int *piBpp);

/**********************************************************************
 * 函数名称： AllocVideoMem
 * 功能描述： VideoMem: 为加快显示速度,我们事先在缓存中构造好显示的页面的数据,
 *            (这个缓存称为VideoMem)
 *            显示时再把VideoMem中的数据复制到设备的显存上
 * 输入参数： iNum
 * 输出参数： 无
 * 返 回 值： 0  - 成功
 *            -1 - 失败(未使用SelectAndInitDefaultDispDev来选择显示模块)
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int AllocVideoMem(int iNum);

/**********************************************************************
 * 函数名称： GetDevVideoMem
 * 功能描述： 获得显示设备的显存, 在该显存上操作就可以直接在LCD上显示出来
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 显存对应的VideoMem结构体指针
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
PT_VideoMem GetDevVideoMem(void);

/**********************************************************************
 * 函数名称： GetVideoMem
 * 功能描述： 获得一块可操作的VideoMem(它用于存储要显示的数据), 
 *            用完后用PutVideoMem来释放
 * 输入参数： iID  - ID值,先尝试在众多VideoMem中找到ID值相同的
 *            bCur - 1表示当前程序马上要使用VideoMem,无法如何都要返回一个VideoMem
 *                   0表示这是为了改进性能而提前取得VideoMem,不是必需的
 * 输出参数： 无
 * 返 回 值： NULL   - 失败,没有可用的VideoMem
 *            非NULL - 成功,返回PT_VideoMem结构体
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
PT_VideoMem GetVideoMem(int iID, int bCur);

/**********************************************************************
 * 函数名称： PutVideoMem
 * 功能描述： 使用GetVideoMem获得的VideoMem, 用完时用PutVideoMem释放掉
 * 输入参数： ptVideoMem - 使用完毕的VideoMem
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
void PutVideoMem(PT_VideoMem ptVideoMem);

/**********************************************************************
 * 函数名称： ClearVideoMem
 * 功能描述： 把VideoMem中内存全部清为某种颜色
 * 输入参数： ptVideoMem - VideoMem结构体指针, 内含要操作的内存
 *            dwColor    - 设置为该颜色
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
void ClearVideoMem(PT_VideoMem ptVideoMem, unsigned int dwColor);


/**********************************************************************
 * 函数名称： FBInit
 * 功能描述： 注册"framebuffer显示设备"
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2013/02/08	     V1.0	  韦东山	      创建
 ***********************************************************************/
int FBInit(void);

int GetVideoBufForDisplay(PT_PixelDataset ptFrameBuf);

#endif /* _DISP_MANAGER_H */

