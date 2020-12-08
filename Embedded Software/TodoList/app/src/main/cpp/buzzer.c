#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <unistd.h>
#include <fcntl.h>

#define BUZZER_DEVICE "/dev/fpga_buzzer"

int fpga_buzzer(int x){
    int dev;
    unsigned char data;
    unsigned char retval;

    data = (char)x;

    dev = open(BUZZER_DEVICE,O_RDWR);
    if(dev < 0){
        __android_log_print(ANDROID_LOG_INFO,"open error","driver = %d",x);
        return -1;
    }else{
        __android_log_print(ANDROID_LOG_INFO,"open success","driver = %d",x);
        write(dev,&data,1);
        close(dev);
        return 0;
    }
}


JNIEXPORT jint JNICALL Java_com_example_todolist_LoginActivity_buzzer(JNIEnv* env,jobject this, jint val) {
    jint result;
    __android_log_print(ANDROID_LOG_INFO,"example","value = %d",val);
    result = fpga_buzzer(val);

    return result;
}

