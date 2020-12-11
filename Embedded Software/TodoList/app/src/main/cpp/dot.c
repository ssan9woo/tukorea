#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <fcntl.h>
#include <unistd.h>

#include "dot.h"
#define DOT_DEVICE "/dev/fpga_dot"

int fpga_dot(int x){
    int i;
    int dev;
    size_t str_size;

    str_size = sizeof(fpga_number[x]);

    dev = open(DOT_DEVICE,O_RDWR);
    if(x == 10){
        write(dev,fpga_set_blank, sizeof(fpga_set_blank));
    }else{
        write(dev,fpga_number[x],str_size);
    }
    close(dev);

    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_todolist_MainActivity_dot(JNIEnv* env,jobject this, jint val) {
    fpga_dot(val);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_todolist_CalendarActivity_dot(JNIEnv* env,jobject this, jint val) {
    fpga_dot(val);
    return 0;
}