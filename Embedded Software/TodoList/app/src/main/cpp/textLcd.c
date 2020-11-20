#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <fcntl.h>
#include <unistd.h>

#define LINE_BUFF 16
#define MAX_BUFF 32

#define FPGA_TEXT_LCD_DEVICE "/dev/fpga_text_lcd"

int fpga_text_lcd(const char* str1, const char* str2){
    int i;
    int dev;
    size_t str_size;
    int chk_size;

    unsigned char string[32];
    memset(string,0, sizeof(string));

    dev = open(FPGA_TEXT_LCD_DEVICE,O_RDWR);
    str_size = strlen(str1);
    if(str_size > 0){
        strncat(string,str1,str_size);
        memset(string+str_size,' ',LINE_BUFF-str_size);
    }

    str_size = strlen(str2);
    if(str_size > 0){
        strncat(string,str2,str_size);
        memset(string+LINE_BUFF+str_size,' ', LINE_BUFF - str_size);
    }
    write(dev,string,MAX_BUFF-1);
    close(dev);

    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_todolist_ListViewAdapter_textLcd(JNIEnv* env,jobject this, jstring str1, jstring str2) {
    jint result = 0;
    const char* pstr1 = (*env) -> GetStringUTFChars(env,str1,NULL);
    const char* pstr2 = (*env) -> GetStringUTFChars(env,str2,NULL);

    fpga_text_lcd(pstr1,pstr2);
    (*env) -> ReleaseStringUTFChars(env,str1,pstr1);
    (*env) -> ReleaseStringUTFChars(env,str2,pstr2);

    return 0;
}


