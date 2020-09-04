#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/fb.h>
 
typedef unsigned int U32;  
void swap(int *swapa, int *swapb);  
unsigned int random_pixel(void);   
 
int main(int argc, char** argv) {
    int check, frame_fd, offset; 
    U32 pixel;
    int posx1, posy1, posx2, posy2, repx, repy;
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
 
    while(1) {
        pixel = random_pixel(); 
        posx1 = (int)((fvs.xres*1.0*rand())/(RAND_MAX+1.0));  
        posx2 = (int)((fvs.xres*1.0*rand())/(RAND_MAX+1.0));  
        posy1 = (int)((fvs.yres*1.0*rand())/(RAND_MAX+1.0));  
        posy2 = (int)((fvs.yres*1.0*rand())/(RAND_MAX+1.0));  
        swap(&posx1, &posx2); 
        swap(&posy1, &posy2); 
 
        usleep(500000); 
 
        for(repy=posy1; repy < posy2; repy++) {
            offset = repy * fvs.xres * (32/8) + posx1 * (32/8);
            if(lseek(frame_fd, offset, SEEK_SET) < 0) {
                perror("LSeek Error!");
                exit(1);
            }
            for(repx = posx1; repx <= posx2; repx++)
                write(frame_fd, &pixel,(32/8));
        } 
    } 
    close(frame_fd);
 
    return 0;
}
 
unsigned int random_pixel(void)
{   
    return rand();
}
 
void swap(int *swapa, int *swapb) {
    int temp; 
    if(*swapa > *swapb) { 
        temp = *swapb;
        *swapb = *swapa;
        *swapa = temp;
    }
}
