#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/fb.h>

void swap(int *swapa, int *swapb); 
unsigned int random_pixel(void);  

int main(int argc, char** argv)
{
     int check, frame_fd; 
     unsigned int pixel;
     int offset, posx1, posy1, posx2, posy2;
     int repx, repy, count = 1000;
     unsigned int* pfbdata;
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
         perror("Unsupport Mode. 16Bpp Only.");
         exit(1);
     }

     pfbdata = (unsigned int *) mmap(0, fvs.xres*fvs.yres*32/8, PROT_READ | PROT_WRITE, MAP_SHARED, frame_fd, 0);  
     if((unsigned)pfbdata == (unsigned)-1) {
         perror("Error Mapping!\n");
     }
     while(1<count--) {

         pixel = random_pixel(); 

         posx1 = (int)((fvs.xres*1.0*rand())/(RAND_MAX+1.0)); 
         posx2 = (int)((fvs.xres*1.0*rand())/(RAND_MAX+1.0)); 
         posy1 = (int)((fvs.yres*1.0*rand())/(RAND_MAX+1.0)); 
         posy2 = (int)((fvs.yres*1.0*rand())/(RAND_MAX+1.0));  

         swap(&posx1, &posx2); 
         swap(&posy1, &posy2); 

         for(repy=posy1; repy <= posy2; repy++) {
             offset = repy * fvs.xres;

             for(repx=posx1;repx<=posx2;repx++)
                *(pfbdata+offset+repx) = pixel;
         } 
     } 

     munmap(pfbdata,fvs.xres*fvs.yres*32/8);  

     close(frame_fd);

     return 0;
}

unsigned int random_pixel(void)
{   
     return (int)(65536.0*rand()/(RAND_MAX+1.0));
}

void swap(int *swapa, int *swapb) {
     int temp;
     if(*swapa > *swapb) { 
         temp = *swapb;
         *swapb = *swapa;
         *swapa = temp;
    }
}   
