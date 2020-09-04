#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/fb.h>
 
typedef unsigned short int U32;
unsigned int makepixel(U32  r, U32 g, U32 b)
{
    return (U32)((r<<16)|(g<<8)|b);
}
 
int main(int argc, char** argv)
{
    int check;
    int frame_fd;
    U32 pixel;
    int offset;
    int posx1, posy1, posx2, posy2;
    int repx, repy;
    struct fb_var_screeninfo fvs;
 
    if((frame_fd = open("/dev/fb0",O_RDWR))<0) {
        perror("Frame Buffer Open Error!");
        exit(1);
    }
 
    if((check=ioctl(frame_fd,FBIOGET_VSCREENINFO,&fvs))<0) {
        perror("Get Information Error - VSCREENINFO!");
        exit(1);
    }
 
    if(fvs.bits_per_pixel != 32) {
        perror("Unsupport Mode. 32Bpp Only.");
        exit(1);
    }
 
    if(lseek(frame_fd, 0, SEEK_SET) < 0) {  
        perror("LSeek Error.");
        exit(1);
    }
 
    pixel = makepixel(0,0,255); 
    posx1 = 100;  
    posx2 = 150;  
    posy1 = 120;
    posy2 = 170;
    
    for(repy=posy1; repy < posy2; repy++) {
        offset = repy * fvs.xres * (32/8) + posx1 * (32/8);
        if(lseek(frame_fd, offset, SEEK_SET) < 0) {
            perror("LSeek Error!");
            exit(1);
        }
        for(repx = posx1; repx <= posx2; repx++)
            write(frame_fd, &pixel,(32/8));
    }
    close(frame_fd);
    return 0;
    //dmesg | grep fb
    //size -> 0:5:6:5 -> 16bit N:R:G:B
}
