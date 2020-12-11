#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <fcntl.h>
#include <unistd.h>

#define LED_DEVICE "/dev/fpga_led"

int fpga_led(int x){
   int dev;
   unsigned char data;
   unsigned char retval;

   unsigned char val[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00,0xFF};
   //0%, 25%, 50%, 75%, 100%
   unsigned char per[] = {0x00,0x11,0x33,0x77,0xFF};
   dev = open(LED_DEVICE, O_RDWR);

   write(dev,&per[x], sizeof(unsigned char));
   close(dev);

   return 0;
}

JNIEXPORT jint JNICALL Java_com_example_todolist_MainActivity_led(JNIEnv* env,jobject this,jint val) {
    fpga_led(val);
    return NULL;
}

JNIEXPORT jint JNICALL Java_com_example_todolist_CalendarActivity_led(JNIEnv* env,jobject this,jint val) {
   fpga_led(val);
   return NULL;
}

