#include "parking.h"
#include "fpga_dot_font.h"
void* t_ultrasonic();
void* t_recv_from_server();
void* t_touch_screen();

void Print_lcd(char*, char*);
void Exit_button(void);
void Charge_button(void);
void Charge(char*);
void Parking(char,char*);
void LED(int,int);
void Lcd_clear(void);
void Signal_handler(int);
char* Read_switch();
int Is_exist_car(char*);

/****************************************************
*		This is functions that draw screen.			*
****************************************************/
int Makepixel(int  r, int g, int b);
void DrawLineHor(int height, int start_x, int length_x, int pixel);
void DrawLineVer(int width, int start_y, int length_y, int pixel);
void DrawRect(int x1, int y1, int x2, int y2, int pixel);
void ClearDisplay(void);
void DrawUpDiagonal(int x1, int y1, int end_x, int pixel);
void DrawDownDiagonal(int x1, int y1, int end_x, int pixel);
void DrawMainScreen(void);
void DrawArrow(int x1, int y1, int end_x, int end_y_up, int end_y_down);
void DrawCar(int x1, int y1,int flag);
void Circle( int x_center, int y_center, int radius);
void Ploat_circle(int x_center, int y_center, int x_coor, int y_coor);
void Dot(int x, int y);
void DrawRect2(int x1, int y1, int x2, int y2, int pixel);
void Car(int i, int flag);
void DrawTree(int x1, int y1);
void DrawGrass(int x1, int y1);
void DrawHouse(int x1, int y1, int roofColor, int houseColor);
void PushPayButton(void);
void ReleasePayButton(void);
void PushExitButton(void);
void ReleaseExitButton(void);

int status;
int client_sockfd,client_len,frame_fd;
int dev_button,dev_fnd,dev_dot,dev_led;
int available;
int count_car;
unsigned char led;
int* pfbdata;
struct fb_var_screeninfo fvs;
Parking_user user[AREA_NUM];
pthread_t p_thread[3];
int main(int argc, char **argv) {
	struct sockaddr_in clientaddr;
	char us_buff[2];
	char msg[BUFF_SIZE];
	int thread_id[3];
	int i;
	int check;
	int pixel;
	int offset, posx1, posy1, posx2, posy2;
	int repx, repy;	

	signal(SIGINT,(void*) Signal_handler);

	/************************************************************
	*					Connect to SERVER						*
	************************************************************/
	if ((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("error : ");
		return 1;
	}
	else printf("Client_socket() sockfd is OK...\n");

	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr(argv[1]);
	clientaddr.sin_port = htons(SERV_PORT);
	memset(&(clientaddr.sin_zero), 0, 8);

	client_len = sizeof(clientaddr);

	if (connect(client_sockfd, (struct sockaddr*)&clientaddr, client_len) == -1){
		perror("connect error : ");
		exit(1);
	}
	else printf("Client_connect() is OK...\n\n");
	memset(&user,0,sizeof(user));
	/************************************************************
	*					Open device driver						*
	************************************************************/
	dev_button=open("/dev/push_switch",O_RDONLY);
	dev_fnd=open("/dev/fnd",O_RDWR);
	dev_dot=open("/dev/dot",O_WRONLY);
	dev_led=open("/dev/led",O_RDWR);
	if((frame_fd = open("/dev/fb0",O_RDWR))<0){
 		perror("Frame Buffer Open Error!");
 		exit(1);
 	}
 	if((check=ioctl(frame_fd,FBIOGET_VSCREENINFO,&fvs))<0){
 		perror("Get Information Error - VSCREENINFO!");
 		exit(1);
 	}	
	/*		Frame buffer memory mapping		     */
	pfbdata = (int *) mmap(0, fvs.xres*fvs.yres*4, PROT_READ| \
 	PROT_WRITE, MAP_SHARED, frame_fd, 0);               

 	if((unsigned)pfbdata == (unsigned)-1) {
 		perror("Error Mapping!\n");
 	}
	
	/*		setup device & screen			     */
	write(dev_dot,fpga_number[count_car],sizeof(fpga_number[count_car]));
	write(dev_led, &led,1);
	DrawMainScreen();
	/************************************************************
	*				Create threads & start threads				*
	************************************************************/
	thread_id[0] = pthread_create(&p_thread[0], NULL, t_ultrasonic,NULL);
	thread_id[1] = pthread_create(&p_thread[1], NULL, t_recv_from_server,NULL);
	thread_id[2] = pthread_create(&p_thread[2], NULL, t_touch_screen,NULL);
	/*			  Main loop			    */
	while(1);

	
	/************************************************************
	*						Cancel threads						*
	************************************************************/
	pthread_cancel(p_thread[0]);
	pthread_cancel(p_thread[1]);
	pthread_cancel(p_thread[2]);
	pthread_join(p_thread[0], (void**)&status);
	pthread_join(p_thread[1], (void**)&status);
	pthread_join(p_thread[2], (void**)&status);

	/************************************************************
	*			Close devices and release memory map			*
	************************************************************/
	munmap(pfbdata,fvs.xres*fvs.yres*4); 
 	close(frame_fd);
	close(client_sockfd);
	close(dev_button);
	close(dev_fnd);
	close(dev_dot);
	close(dev_led);
	return 0;
}

/****************************************************
*		This is function that handles sinnal.		*
****************************************************/
void Signal_handler(int signo){
	/************************************************************
	*						Cancel threads						*
	************************************************************/
	pthread_cancel(p_thread[0]);
	pthread_cancel(p_thread[1]);
	pthread_cancel(p_thread[2]);
	pthread_join(p_thread[0], (void**)&status);
	pthread_join(p_thread[1], (void**)&status);
	pthread_join(p_thread[2], (void**)&status);

	/************************************************************
	*			Close devices and release memory map			*
	************************************************************/
	munmap(pfbdata,fvs.xres*fvs.yres*4); 
 	close(frame_fd);
	close(client_sockfd);
	close(dev_button);
	close(dev_fnd);
	close(dev_dot);
	close(dev_led);
	exit(1);
}
/****************************************************
*	This is thread that handles ultrasonic value.	*
****************************************************/
void* t_ultrasonic(){
	int dev_us;
	char us_buff[2];
	int dist;
	dev_us=open("/dev/us",O_RDWR);
	
	while(1){
		read(dev_us,us_buff,2);
		dist=us_buff[0];
	
		if(dist>=20 && dist<=25){
			printf("distance: %d\n",dist);
			if(available>=AREA_NUM){
				//lcd_print *not available*
				Print_lcd("SORRY","NOT AVAILABLE");
			}
			else{
				sleep(1);
				send(client_sockfd,"start",strlen("start")+1,0);
				/*
					Infinite loop until ultrasoic is 0	
				*/
				do{				
					read(dev_us,us_buff,2);		
					dist=us_buff[0];
				}while(dist<50);
			}
		}
		usleep(400000);
	}
}

/****************************************************
*	This is thread recieves data from server.		*
****************************************************/
void* t_recv_from_server(){
	char msg[BUFF_SIZE];
	char keyword;
	unsigned char value[16]="";
	int rcv_byte,i;
	while(1){
		memset(msg,0,sizeof(msg));
			
		rcv_byte=recv(client_sockfd, msg, sizeof(msg), 0);
		printf("%s\n",msg);
		keyword=msg[0];
		for(i=1;i<strlen(msg);i++){
			value[i-1]=msg[i];
		}
		value[i]='\0';
		switch(keyword){
			case	'$':
				Charge(value);
				memset(value,0,sizeof(value));
				break;
			default:
				Parking(keyword,value);
				memset(value,0,sizeof(value));
				break;
		}
	}
}

/****************************************************
*	This is a thread that handles screen events.	*
****************************************************/
void* t_touch_screen(){
	int screen_fd, ret;
	struct input_event iev[3];
	int x,y;
	screen_fd = open(DEV_SCREEN_PATH, O_RDONLY);
	if(screen_fd < 0) {
		perror("error: could not open evdev\n");
	}
	
	while(1){
        ret = read(screen_fd, iev, sizeof(struct input_event)*3);
		if(ret < 0) {
            perror("error: could not read input event");
            break;
        }
        if(iev[0].type == 1 && iev[1].type == 3 && iev[2].type == 3){
			x=iev[1].value;
			y=iev[2].value;
			/*		Touch the pay Icon		*/
			if(( 829<=x && x<=1013 ) && ( 504<=y && y<=589 )){
           		printf("x = %d, y = %d \n",x,y);				
				PushExitButton();
				usleep(100000);
				ReleaseExitButton();
				Exit_button();
				
			}	
			else if((659<=x && x<=833) && ( 504<=y && y<=589 )){
           		printf("x = %d, y = %d \n",x,y);
				PushPayButton();
				usleep(100000);
				ReleasePayButton();
				Charge_button();
			}
        }
    }
    close(screen_fd);
}
void Charge(char* info){
	int area,i;
	char car_number[5];
	char* fee;
	int size;
	size=sizeof(16);
	fee=(char*)malloc(size);
	memset(fee,0,size);
	for(i=0;i<strlen(info)-1;i++){
		if(i<4)
			car_number[i]=info[i];
		else
			fee[i-4]=info[i];
		
	}
	car_number[4]='\0';	//$2343000
	area=Is_exist_car(car_number);
	if(area==-1){
		Print_lcd("SORRY.","not exist car");
		Lcd_clear();
		perror("NO exist car!!\n");
		free(fee);
		return;
	}
	else{
		memset(&user[area],0,sizeof(user[area]));;	
		count_car--;
		LED(area,OFF);
		Car(area,FALSE);
		write(dev_dot,fpga_number[count_car],sizeof(fpga_number[count_car]));
		Print_lcd("Fee",fee);
		Lcd_clear();
	}
	free(fee);
}

void Charge_button(){
	char numbers[5]="";
	int i;
	strcpy(numbers,Read_switch());
	printf("numbers:  %s\n",numbers);
	for(i=0;i<6;i++){
	printf("user[i] info: %c ,%s\n",user[i].area,user[i].car_number);
	}
	if(Is_exist_car(numbers)!= -1){
		printf("%s",numbers);
		send(client_sockfd,numbers,strlen(numbers)+1,0);
	}
	else
		perror("No exist car\n");
}

int Is_exist_car(char* info){
	int ret=-1;
	int i;	
	for(i=0;i<AREA_NUM;i++){
		if(!strcmp(info,user[i].car_number)){
			ret=i;
			break;
		}
	}
	return ret;
}
char* Read_switch(){
	unsigned char push_sw_buf[IOM_PUSH_SWITCH_MAX_BUTTON];
	unsigned char fnd_data[4];
	char* ret="";
	int push_cnt=0;
	int i;
	ret=(char*)malloc(sizeof(char)*5);
	memset(ret,0,sizeof(ret));
	memset(fnd_data,0,sizeof(fnd_data));
	while(push_cnt<4){
		read(dev_button,&push_sw_buf,sizeof(push_sw_buf));
		for(i=0; i< IOM_PUSH_SWITCH_MAX_BUTTON;i++){
			if(push_sw_buf[i]){
				usleep(400000);
				fnd_data[push_cnt]=i+1;
				ret[push_cnt]='1'+i;
				push_cnt++;
				write(dev_fnd,fnd_data,FND_MAX_DIGIT);
			}
		}
	}
	ret[4]='\0';
	sleep(1);
	memset(fnd_data,0,sizeof(fnd_data));
	write(dev_fnd,fnd_data,FND_MAX_DIGIT);
	return ret;
}
void Exit_button(){
}
void Print_lcd(char* str1, char* str2) {
	unsigned char buf[TEXT_LCD_MAX_BUF];
	int dev, len1, len2;
	int i;
	len1 = strlen(str1);
	len2 = strlen(str2);
	dev = open(TEXT_LCD_DEVICE, O_WRONLY);
	memset(buf, ' ', TEXT_LCD_MAX_BUF);
	write(dev, buf, TEXT_LCD_MAX_BUF);
	memcpy(buf, str1, len1);
	memcpy(buf + TEXT_LCD_LINE_BUF, str2, len2);
 
	write(dev, buf, TEXT_LCD_MAX_BUF);

	close(dev);
}
void Lcd_clear(){
	sleep(3);
	Print_lcd("                    ","                    ");
}

void Parking(char area,char* num){
	int n=area-'A';
	int i;

	user[n].area=area;
	strcpy(user[n].car_number,num);
	count_car++;
	LED(n, ON);
	Car(n, TRUE);
	write(dev_dot,fpga_number[count_car],sizeof(fpga_number[count_car]));
}
void LED(int  num,int onoff){
	unsigned char temp;
	switch(num){
		case	0:
			temp=(1<<7);
			break;
		case	1:
			temp=(1<<3);
			break;
		case	2:
			temp= (1<<6);
			break;
		case	3:
			temp= (1<<2);
			break;
		case	4:
			temp= (1<<5);
			break;
		case	5:
			temp= (1<<1);
			break;
	}
	if(onoff)
		led|=(temp);
	else
		led&=~(temp);

	write(dev_led, &led,TRUE);

}

int Makepixel(int  r, int g, int b){
    return (int)((r<<16)|(g<<8)|b);
}

void DrawArrow(int x1, int y1, int end_x, int end_y_up, int end_y_down){
	
	int pixel = Makepixel(255,255,255);	
	int offset = y1 * fvs.xres + x1;
	int i, j, k = y1;	
	int startPoint = x1;
	int len = (end_x - x1)/3;

	for(i = y1, j = y1; i <= y1 + len; i++, j--){
		for(k = y1; k <= i; k++){
			offset = k * fvs.xres + startPoint;
			*(pfbdata + offset) = pixel;
		}
		for(k = y1; k >= j; k--){
			offset = k * fvs.xres + startPoint;
			*(pfbdata + offset) = pixel;		
		}
		startPoint += 1;	
	}

	DrawRect(startPoint,y1 - ((startPoint - x1)/2),end_x,y1 + ((startPoint - x1)/2),Makepixel(255,255,255));
}

void Dot(int x, int y){
	int offset;
	int pixel = Makepixel(0,0,0);
	offset = y * fvs.xres + x;
	*(pfbdata + offset) = pixel;
}

void Ploat_circle(int x_center, int y_center, int x_coor, int y_coor){
	Dot( (x_center + x_coor), (y_center + y_coor));
	Dot( (x_center - x_coor), (y_center + y_coor));
	Dot( (x_center + x_coor), (y_center - y_coor));
   	Dot( (x_center - x_coor), (y_center - y_coor));
   	Dot( (x_center + y_coor), (y_center + x_coor));
   	Dot( (x_center - y_coor), (y_center + x_coor));
   	Dot( (x_center + y_coor), (y_center - x_coor));
   	Dot( (x_center - y_coor), (y_center - x_coor));	
} 

void  Circle( int x_center, int y_center, int radius){
   int      x_coor;
   int      y_coor;
   int      p_value;

   x_coor   = 0;
   y_coor   = radius;
   p_value   = 3 - 2 * radius;
   while(x_coor < y_coor){
      Ploat_circle( x_center, y_center, x_coor, y_coor);
      if ( p_value < 0){
         p_value   += 4 * x_coor +6;
      }
      else{
         p_value   += 4 * ( x_coor -y_coor) +10;
         y_coor--;
      }
      x_coor++;
   }
   if ( x_coor == y_coor)
      Ploat_circle( x_center, y_center, x_coor, y_coor);
}

void DrawLineHor(int height, int start_x, int length_x, int pixel){
	int horOffset = height * fvs.xres;
	int i = 0;
	for(i = start_x; i < start_x + length_x; i++){
		*(pfbdata + horOffset + i) = pixel;	
	}
}


void DrawLineVer(int width, int start_y, int length_y, int pixel){
	int verOffset = width;
	int i = 0;
	for(i = start_y; i < start_y + length_y; i++){
		*(pfbdata + verOffset + (i * fvs.xres)) = pixel;	
	}
}

void DrawRect(int x1, int y1, int x2, int y2, int pixel){
	int i = 0, j = 0, rectOffset;	
	for(i = y1; i <= y2; i++){
 		rectOffset = i * fvs.xres;
	 	for(j = x1;j <= x2; j++)
 			*(pfbdata + rectOffset + j) = pixel;
 	}
}
//car size(width : 106, height : 150)
void DrawCar(int x1, int y1, int flag){
	int i, j, offset,pixel;
	int wheel_pixel = Makepixel(192,192,192);
	int wheel_Line_Pixel = Makepixel(0,0,0);
	if(flag == TRUE)
		pixel = Makepixel(255,0,0);
	else
		pixel = Makepixel(0,0,255);

	for(i = y1; i <= y1 + 150; i++){
		offset = i * fvs.xres;
		for(j = x1; j <= x1 + 106; j++)
			*(pfbdata + offset + j) = pixel;	
	}
	
	for(i = y1 + 20; i <= y1 + 50 ; i++){
		offset = i * fvs.xres;
		for(j = x1 - 20; j <= x1; j++){
			if(j == (x1 - 10)){
				*(pfbdata + offset + j) = wheel_Line_Pixel;
			}
			else
				*(pfbdata + offset + j) = wheel_pixel;
		}	
	}

	for(i = y1 + 20; i <= y1 + 50; i++){
		offset = i * fvs.xres;
		for(j = x1 + 106; j <= x1 + 126; j++){
			if(j == (x1 + 116))
				*(pfbdata + offset + j) = wheel_Line_Pixel;
			else			
				*(pfbdata + offset +j) = wheel_pixel;		
		}	
	}

	for(i = y1 + 100; i <= y1 + 130; i++){
		offset = i * fvs.xres;
		for(j = x1 - 20; j <= x1; j++){
			if(j == (x1 - 10))
				*(pfbdata + offset + j) = wheel_Line_Pixel;
			else
				*(pfbdata + offset + j) = wheel_pixel;	
		}
	}
	
	for(i = y1 + 100; i <= y1 + 130; i++){
		offset = i * fvs.xres;
		for(j = x1 + 106; j <= x1 + 126; j++){
			if(j == (x1 + 116))
				*(pfbdata + offset + j) = wheel_Line_Pixel;
			else
				*(pfbdata + offset + j) = wheel_pixel;	
		}
	}
} 

void ClearDisplay(){
	int i = 0, j = 0, rectOffset;	
	for(i = 0; i < 600; i++){
 		rectOffset = i * fvs.xres;
	 	for(j = 0;j < 1024; j++)
 			*(pfbdata + rectOffset + j) = Makepixel(0,0,0);
 	}
}
void DrawUpDiagonal(int x1, int y1, int end_x, int pixel){
	int i, upDiagonalOffset;
	for(i = x1; i <= end_x; i++){
		upDiagonalOffset = y1 * fvs.xres + x1;
		*(pfbdata + upDiagonalOffset + i) = pixel;
		y1 += 1;	
	} 
}
void DrawDownDiagonal(int x1, int y1, int end_x, int pixel){
	int i, downDiagonalOffset;
	for(i = x1; i <= end_x; i++){
		downDiagonalOffset = y1 * fvs.xres + x1;
		*(pfbdata + downDiagonalOffset + i) = pixel;
		y1 -= 1;	
	} 
}

void DrawMainScreen(){
	int i;
	ClearDisplay();
	//side	
	DrawRect(0,0,1023,10,Makepixel(255,255,255));
	DrawRect(0,0,10,599,Makepixel(255,255,255));
	DrawRect(0,590,1023,599,Makepixel(255,255,255));
	DrawRect(1013,0,1023,599,Makepixel(255,255,255));
	//parking area - up
	DrawRect(0,229,659,239,Makepixel(255,255,255));
	DrawRect(649,0,659,239,Makepixel(255,255,255));	
	DrawRect(215,0,225,239,Makepixel(255,255,255));
	DrawRect(431,0,441,239,Makepixel(255,255,255));
	//parking area - down
	DrawRect(0,349,658,359,Makepixel(255,255,255));
	DrawRect(648,349,658,599,Makepixel(255,255,255));
	DrawRect(215,349,225,599,Makepixel(255,255,255));
	DrawRect(431,349,441,599,Makepixel(255,255,255));
	//buttonLine
	DrawRect(824,499,1023,504,Makepixel(255,255,255));
	DrawRect(658,499,824,504,Makepixel(255,255,255));
	DrawRect(833,499,838,589,Makepixel(255,255,255));
	//payButton
	ReleasePayButton();
	//exitButton	
	ReleaseExitButton();	
	//Line
	DrawRect(40,289,80,294,Makepixel(255,255,255));
	DrawRect(180,289,220,294,Makepixel(255,255,255));	
	DrawRect(320,289,360,294,Makepixel(255,255,255));
	DrawRect(460,289,500,294,Makepixel(255,255,255));
	//Arrow
	DrawArrow(540,290,665,260,300);
	//Tree
	DrawTree(670,30);
	DrawTree(880,180);	
	DrawTree(700,340);	
	//grass
	DrawGrass(710, 250);
	DrawGrass(900, 400);
	DrawGrass(820, 110);		
	//house
	DrawHouse(965,15,Makepixel(255,0,0),Makepixel(255,255,0));
	//car
	for(i = 0 ; i < 6; i++)
		Car(i,FALSE);
}

void Car(int i, int flag){
	switch(i){
		case 0:
			DrawCar(59,49,flag);
			break;
		case 1:
			DrawCar(59,399,flag);
			break;
		case 2:
			DrawCar(277,49,flag);
			break;
		case 3:
			DrawCar(277,399,flag);
			break;
		case 4:
			DrawCar(494,49,flag);
			break;
		case 5:
			DrawCar(494,399,flag);
			break;
		default:
			break;
	}
}


void DrawTree(int x1, int y1){
	DrawRect(x1,y1,x1+90,y1+90,Makepixel(0,255,0));
	DrawRect(x1+30,y1+60,x1+60,y1+150,Makepixel(150,75,0));
}

void DrawGrass(int x1, int y1){
	int i;	
	DrawLineHor(y1,x1,60,Makepixel(0,255,0));
	for(i = 0; i <= 3; i++){
		DrawLineVer(x1,y1,30,Makepixel(0,255,0));
		x1 += 20;
	}
}


void DrawHouse(int x1, int y1, int roofColor, int houseColor){
	int i,j,k,offset;
	int startPoint = y1;

	for(i = x1,j = x1; i <= x1 + 45; i++, j--){
		for(k = x1; k <= i; k++){
			offset = startPoint * fvs.xres + k;
			*(pfbdata + offset) = roofColor;		
		}	
		for(k = x1; k >= j; k--){
			offset = startPoint * fvs.xres + k;
			*(pfbdata + offset) = roofColor;
		}
		startPoint += 1;
	}
	DrawRect(x1-45,y1+45,x1+45,y1+135,Makepixel(255,255,0));
	DrawRect(x1-15,y1+85,x1+15,y1+135,Makepixel(150,75,0));
	
}


void PushPayButton(){
	DrawRect(659,504,833,589,Makepixel(112,112,0));
	DrawRect(675,524,817,569,Makepixel(0,255,0));
	Circle(747,547,20);	
}
void ReleasePayButton(){
	DrawRect(659,504,833,589,Makepixel(255,255,0));
	DrawRect(675,524,817,569,Makepixel(0,255,0));
	Circle(747,547,20);	
}

void PushExitButton(){
	//exitButton	
	DrawRect(839,504,1013,589,Makepixel(112,0,0));
	DrawRect(899,524,949,574,Makepixel(255,255,255));
	DrawRect(903,529,945,569,Makepixel(112,0,0));
	DrawRect(922,514,926,544,Makepixel(255,255,255));
}

void ReleaseExitButton(){
	DrawRect(839,504,1013,589,Makepixel(255,0,0));
	DrawRect(899,524,949,574,Makepixel(255,255,255));
	DrawRect(903,529,945,569,Makepixel(255,0,0));
	DrawRect(922,514,926,544,Makepixel(255,255,255));
}


