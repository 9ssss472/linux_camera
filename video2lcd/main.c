#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <encoding_manager.h>
#include <fonts_manager.h>
#include <disp_manager.h>
#include <input_manager.h>
#include <pic_operation.h>
#include <render.h>
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


int main(int argc, char **argv)
{	
	int iError;

	PT_VideoDevice ptVideoDevice;

	/*显示器初始化*/
	DisplayInit();

	SelectAndInitDefaultDispDev("fb");

	/*摄像头初始化*/
	VideoOprInit();

	VideoDeviceInit("v4l2",ptVideoDevice);

	/*转换格式初始化*/
	ConvertOprInit();

	
	





	return 0;
}

