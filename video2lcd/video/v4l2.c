#include  "video_manager.h"


T_VideoOpr V4L2Opr;
#define FRAME_COUNTS  3;
unsigned char * aucFrameBuffer[FRAME_COUNTS];

int V4L2Init(void)
{
    int x, y, iBpp;
    struct v4l2_format tFmt;
    struct v4l2_requestbuffers tReqBuf;
    struct v4l2_buffer tBuffer;

    V4L2Opr.ifd = open(VIDEO_PATH, O_RDWR);
    if(V4l2Opr.ifd < 0)
    {
        perror("open V4L2Opr.ifd");
        return -1;
    }

    GetDispResolution(x, y, iBpp);

    tFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tFmt.fmt.pix.width = x;
    tFmt.fmt.pix.height = y;
    tFmt.fmt.pix.format = V4L2_PIX_FMT_RGB565;

    if ( 0 > ioctl(V4L2Opr.ifd , VIDIOC_S_FMT, &tFmt))
    {
        perror("ioctl VIDIOC_S_FMT");
        return -1;
    }

    tReqBuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tReqBuf.count =  FRAME_COUNTS;
    tReqBuf.memory =  V4L2_MEMORY_MMAP;
    if (0 >ioctl(V4L2Opr.ifd, VIDIOC_REQBUFS, &tReqBuf))
    {
        perror("ioctl VIDIOC_REQBUFS");
        return -1;
    }

    tBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tBuffer.memory = V4L2_MEMORY_MMAP;
    for(tBuffer.index = 0; tBuffer.index < FRAME_COUNTS; tBuffer.index ++)
    {
        ioctl(V4L2Opr.ifd, VIDIOC_QUERYBUF, &tBuffer)
        
        aucFrameBuffer[tBuffer.index] = mmap(NULL, tBuffer.lenth, PORT_READ | PORT_WRITE, MAP_SHARED,
         V4L2Opr.ifd, tBuffer.m.offset);

         if(MAP_FAILED == aucFrameBuffer[tBuffer.index])
         {
            perror("mmap error")
            return -1;
         }    
    }

    for(tBuffer.index = 0; tBuffer.index < FRAME_COUNTS; tBuffer.index ++)
    {
        if( 0 > ioctl(V4L2Opr.ifd, VIDIOC_QBUF, &tBuffer))
        {
            perror("ioctl VIDIOC_QBUF")
            return -1;
        }
    }





}

int V4L2GetFrame(void)
{

}

int VideoExit(void)
{

}

int VideoStart(void)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(0 > ioctl(V4L2Opr.ifd, VIDIOC_STREAMON, &type))
    {
        perror("ioctl VIDIOC_STREAMON");
        return -1;
    }
}

int VideoStop(void)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(0 > ioctl(V4L2Opr.ifd, VIDIOC_STREAMOFF, &type))
    {
        perror("ioctl VIDIOC_STREAMON");
        return -1;
    }

}


 V4L2Opr = {
    .VideoInit = V4L2Init;
    .VideoGetFrame = V4L2GetFrame,
    .VideoExit = VideoExit,
    .VideoStart = V4L2Start,
    .VideoStop = V4L2Stop,
};

int RegisterV4L2Opr(void)
{
    return RegisterVideoOpr(&V4L2Opr);
}







