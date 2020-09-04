#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>  
 
int main(int argc, char** argv)
{
     int check;
     int frame_fd;
     struct fb_var_screeninfo st_fvs;  
     struct fb_fix_screeninfo st_ffs;  
     frame_fd = open("/dev/fb0",O_RDWR);
     if(frame_fd < 0)
     {
         perror("Frame Buffer Open Error!");
         exit(1);
     }
 
     check = ioctl(frame_fd, FBIOGET_VSCREENINFO,&st_fvs);
     if(check < 0)
     {
         perror("Get Information Error - VSCREENINFO!");
         exit(1);
     }
 
     check = ioctl(frame_fd,FBIOGET_FSCREENINFO,&st_ffs);
     if(check < 0)
     {
        perror("Get Information Error - FSCREENINFO!");
        exit(1);
     }
 
     system("clear");
     printf("==================\n");
     printf("Frame Buffer Info\n");
     printf("------------------\n");
     printf("X - res   : %d\n",st_fvs.xres);
     printf("Y - res   : %d\n",st_fvs.yres);
     printf("X - v_res : %d\n",st_fvs.xres_virtual);
     printf("Y - v_res : %d\n",st_fvs.yres_virtual);
     printf("Bit/Pixel : %d\n",st_fvs.bits_per_pixel);
     printf("------------------\n");
     printf("Buff Size : %d\n",st_ffs.smem_len);
     printf("==================\n");
 
     close(frame_fd);
 
     return 0;
}
