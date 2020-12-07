#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <fcntl.h>
#include <unistd.h>

#define PUSH_SWITCH_DEVICE "/dev/fpga_push_switch"
#define MAX_BUTTON 9

int gFd = -1;

int fpga_push_open(void){
    int dev;

    dev = open(PUSH_SWITCH_DEVICE, O_RDWR);
    if(dev < 0){
        return -1;
    }else{
        gFd = dev;
    }
    return 0;
}

int fpga_push_close(void){
    if(gFd < 0){
        return 0;
    }else{
        close(gFd);
        return -1;
    }
}

int fpga_push_switch(void){
    int i;
    int dev;
    size_t buff_size;
    int retval;

    unsigned char push_sw_buff[MAX_BUTTON];

    if(gFd < 0){
        return -1;
    }else{
        buff_size = sizeof(push_sw_buff);
        read(gFd,&push_sw_buff,buff_size);
        retval = 0;

        for(i = 0; i <MAX_BUTTON; i++){
            if(push_sw_buff[i] != 0){
                retval |= 0x1 << i;
            }
        }
    }
    return retval;
}

JNIEXPORT jint JNICALL Java_com_example_todolist_LoginActivity_ReceivePushSwitchValue(JNIEnv* env,jobject this) {
    int retval;
    retval = fpga_push_switch();
    return retval;
}

JNIEXPORT jint JNICALL Java_com_example_todolist_LoginActivity_DeviceOpen(JNIEnv* env,jobject this) {
    int retval;
    retval = fpga_push_open();
}

JNIEXPORT jint JNICALL Java_com_example_todolist_LoginActivity_DeviceClose(JNIEnv* env,jobject this) {
    int retval;
    retval = fpga_push_close();
}