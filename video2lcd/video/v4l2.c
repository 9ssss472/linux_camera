#include <video_manager.h>
#include <disp_manager.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

T_VideoOpr V4L2Opr;

static int  g_aiSuportedFormat[]={V4L2_PIX_FMT_RGB565,  V4L2_PIX_FMT_YUYV, V4L2_PIX_FMT_JPEG};

#define DEFAULT_FORMAT V4L2_PIX_FMT_RGB565

int IsSupported(int format)
{
    int i;

    for(i = 0; i <3; i++)
    {
        if(format == g_aiSuportedFormat[i])
        {
            return 1;
        }

    }

    return 0;
}


int V4L2Init(PT_VideoDevice ptVideoDevice)
{
    int x, y, iBpp;
    struct v4l2_format tFmt;
    struct v4l2_requestbuffers tReqBuf;
    struct v4l2_buffer tBuffer;
    int iFd;

    iFd = open(VIDEO_PATH, O_RDWR);
    if(iFd < 0)
    {
        perror("open iFd");
        return -1;
    }
    
    ptVideoDevice ->iFd = iFd;

    GetDispResolution(&x, &y, &iBpp);

    tFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tFmt.fmt.pix.width = x;
    tFmt.fmt.pix.height = y;
    tFmt.fmt.pix.pixelformat = DEFAULT_FORMAT;

    if ( 0 > ioctl(iFd , VIDIOC_S_FMT, &tFmt))
    {
        perror("ioctl VIDIOC_S_FMT");
        return -1;
    }
    ptVideoDevice ->pixelFormat = DEFAULT_FORMAT;
    ptVideoDevice ->width = tFmt.fmt.pix.width;
    ptVideoDevice ->height = tFmt.fmt.pix.height;

    tReqBuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tReqBuf.count =  FRAME_COUNTS;
    tReqBuf.memory =  V4L2_MEMORY_MMAP;
    if (0 >ioctl(iFd, VIDIOC_REQBUFS, &tReqBuf))
    {
        perror("ioctl VIDIOC_REQBUFS");
        return -1;
    }

    ptVideoDevice ->frameCount = tReqBuf.count;

    tBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tBuffer.memory = V4L2_MEMORY_MMAP;
    for(tBuffer.index = 0; tBuffer.index < FRAME_COUNTS; tBuffer.index ++)
    {
        ioctl(iFd, VIDIOC_QUERYBUF, &tBuffer);
        
        ptVideoDevice->aucFrameBuffer[tBuffer.index] = mmap(NULL, tBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED,
         iFd, tBuffer.m.offset);

         if(MAP_FAILED == ptVideoDevice->aucFrameBuffer[tBuffer.index])
         {
            perror("mmap error");
            return -1;
         }    
    }
    ptVideoDevice ->sizeOfOneFrame = tBuffer.length;

    for(tBuffer.index = 0; tBuffer.index < FRAME_COUNTS; tBuffer.index ++)
    {
        if( 0 > ioctl(iFd, VIDIOC_QBUF, &tBuffer))
        {
            perror("ioctl VIDIOC_QBUF");
            return -1;
        }
    }
      ptVideoDevice ->ptVideoOpr =   &V4L2Opr;

    return 0;
}

int V4L2GetFrame(PT_VideoDevice ptVideoDevice,PT_PixelDataset ptPixelDataset)
{
    struct pollfd fds;
    struct v4l2_buffer buf;
    int ret;

    fds.fd = ptVideoDevice ->iFd;
    fds.events = POLLIN;
    fds.revents = 0;

    ret = poll(&fds, 1, -1);
    if(ret < 0)
    {
        perror("poll fds:");
        return -1;
    }



    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(ptVideoDevice->iFd, VIDIOC_DQBUF, &buf);
    if(ret < 0)
    {
        perror("ioctl VIDIOC_DQBUF:");
        return -1;
    }

    ptVideoDevice -> frameIndex = buf.index;

    ptPixelDataset ->tVideoBuffer.iWidth = ptVideoDevice ->width;
    ptPixelDataset ->tVideoBuffer.iHeight = ptVideoDevice ->height;
    ptPixelDataset ->tVideoBuffer.iBpp = (ptVideoDevice->pixelFormat == V4L2_PIX_FMT_YUYV) ? 16 : \
                                        (ptVideoDevice->pixelFormat == V4L2_PIX_FMT_MJPEG) ? 0 :  \
                                        (ptVideoDevice->pixelFormat == V4L2_PIX_FMT_RGB565) ? 16 :  \
                                        0;

    ptPixelDataset ->tVideoBuffer.iLineBytes = ptVideoDevice ->width * ptPixelDataset ->tVideoBuffer.iBpp/8;
    ptPixelDataset ->tVideoBuffer.iTotalBytes = buf.bytesused;
    ptPixelDataset ->tVideoBuffer.aucPixelDatas = ptVideoDevice ->aucFrameBuffer[buf.index];
    ptPixelDataset ->pixelFormat = ptVideoDevice -> pixelFormat;

    return 0;
}

int V4L2PutFrame(PT_VideoDevice ptVideoDevice,PT_PixelDataset ptPixelDataset)
{
    struct v4l2_buffer buf;
    int ret;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(ptVideoDevice->iFd, VIDIOC_QBUF, &buf);
    if(ret < 0)
    {
        perror("ioctl VIDIOC_QBUF:");
        return -1;
    }
    return 0;
}

int VideoExit(PT_VideoDevice ptVideoDevice)
{
    int i;
    for(i = 0; i < ptVideoDevice ->frameCount; i++)
    {
        munmap(ptVideoDevice ->aucFrameBuffer[i], ptVideoDevice ->sizeOfOneFrame);
        ptVideoDevice ->aucFrameBuffer[i] = NULL;
    }

    close(ptVideoDevice ->iFd);

    return 0;
}

int VideoStart(PT_VideoDevice ptVideoDevice)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(0 > ioctl(ptVideoDevice ->iFd, VIDIOC_STREAMON, &type))
    {
        perror("ioctl VIDIOC_STREAMON");
        return -1;
    }
    return 0;
}

int VideoStop(PT_VideoDevice ptVideoDevice)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(0 > ioctl(ptVideoDevice ->iFd, VIDIOC_STREAMOFF, &type))
    {
        perror("ioctl VIDIOC_STREAMON");
        return -1;
    }
    return 0;
}


T_VideoOpr V4L2Opr = {
    .VideoInit = V4L2Init,
    .VideoGetFrame = V4L2GetFrame,
    .VideoExit = VideoExit,
    .VideoStart = VideoStart,
    .VideoStop = VideoStop,
    .PutFrame  = V4L2PutFrame,
};

int RegisterV4L2Opr(void)
{
    return RegisterVideoOpr(&V4L2Opr);
}







