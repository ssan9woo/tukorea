import socket
import threading
import time
import cv2
import numpy as np
import pytesseract
from PIL import Image

host = '172.20.10.3'
port = 9999
cap = cv2.VideoCapture(0)
global isParked
isParked = [ ["",0],["",0],["",0],["",0],["",0],["",0] ]

#***********************************Create Socket************************************
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((host,port))
server_socket.listen(2)
#***************************************************************************************


def sendMsgToAchro(client_socket,message):
    print("sendToAchro :",message)
    client_socket.sendall(bytes(message,'UTF-8'))


def sendMsgToAndroid(client_socket,message):
    print("sendToAndroid :", message)
    client_socket.sendall(bytes(message+"\n",'UTF-8'))


def startCam():
    print("Cam Start")
    while True:
        ret,frame = cap.read()
        dst = cv2.resize(frame,(400,300))
        cv2.imshow('test',dst)
        cv2.moveWindow('test',400,70)

        key = cv2.waitKey(1)
        if key == 27:
            break
    cap.release()
    cv2.destroyAllWindows()


def Receive_Achro(x):
    print("Achro Receive Start")
    achro_socket = x[0][0]
    android_socket = x[1][0]
    while True:
        index = 0
        data = achro_socket.recv(1024)
        msg = data[:len(data)-1].decode("utf-8")   #delete NULL
        print("From Achro data : ", msg, "len : ",len(msg))
        if msg == "exit":
            print("Achro exit...")
            break
        elif msg == "start":
            for ex in range(len(isParked)):     #find none Parked index
                if isParked[ex][1] == 0:
                    index = ex
                    print("register : ",isParked)
                    break
            imageProcessing(achro_socket,android_socket,index)
        else:
            sendMsgToAndroid(android_socket, msg)
    achro_socket.close()


def Receive_Android(y):
    print("Android Receive Start")
    achro_socket = y[0][0]
    android_socket = y[1][0]
    
    while True:
        data = android_socket.recv(1024)
        msg = data[:len(data)-1].decode("utf-8")
        print("From Android data : ", msg, "len : ",len(msg))

        if msg == "exit":
            print("Android exit...")
            break
        else:
            for ex in range(len(isParked)):
                if isParked[ex][0] == msg[1:5]:
                    isParked[ex][0] = ""
                    isParked[ex][1] = 0
                    print("delete : ",isParked)
                    break
            print("parked_del :",isParked)
            sendMsgToAchro(achro_socket,msg)
    android_socket.close()

def imageProcessing(achro_socket,android_socket,carNum):
    print("screenShot!!!")
    ret, frame = cap.read()
    cv2.imwrite('carNumber' + str(carNum + 1) + '.jpg',frame)    #need?
    car = cv2.imread('carNumber' + str(carNum + 1) + '.jpg',cv2.IMREAD_COLOR) #need?
    
    copy_img = car.copy()
    img2 = cv2.cvtColor(car, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(img2,(3,3),0)
    canny = cv2.Canny(blur,100,200)
    cnts, contours, hierarchy = cv2.findContours(canny, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    box1 = []
    f_count = 0
    select = 0
    plate_width = 0

    for i in range(len(contours)):
        cnt = contours[i]
        area = cv2.contourArea(cnt)
        x,y,w,h = cv2.boundingRect(cnt)
        rect_area = w * h
        aspect_ratio = float(w) / h

        if(aspect_ratio >= 0.2) and (aspect_ratio <= 1.0) and (rect_area >= 100) and (rect_area <= 700):
            cv2.rectangle(frame,(x,y),(x + w, y + h),(0,255,0),1)
            box1.append(cv2.boundingRect(cnt))
    for i in range(len(box1)):
        for j in range(len(box1) - (i + 1)):
            if box1[j][0] > box1[j+1][0]:
                temp = box1[j]
                box1[j] = box1[j+1]
                box1[j+1] = temp

    for m in range(len(box1)):
        count = 0
        for n in range(m+1,(len(box1)-1)):
            delta_x = abs(box1[n+1][0] - box1[m][0])

            if delta_x > 150:
                break
            delta_y = abs(box1[n+1][1] - box1[m][1])
            if delta_x == 0:
                delta_x = 1
            if delta_y == 0:
                delta_y = 1
            gradient = float(delta_y) / float(delta_x)
            if gradient < 0.25:
                count += 1
        if count > f_count:
            select = m
            f_count = count
            plate_width = delta_x
    
    number_plate = copy_img[box1[select][1]-10:box1[select][3]+box1[select][1]+20,box1[select][0]-10:140+box1[select][0]]
    resize_plate = cv2.resize(number_plate,None,fx=1.8,fy=1.8,interpolation=cv2.INTER_CUBIC+cv2.INTER_LINEAR)
    plate_gray = cv2.cvtColor(resize_plate,cv2.COLOR_BGR2GRAY)
    ret, th_plate = cv2.threshold(plate_gray,150,255,cv2.THRESH_BINARY)

    kernel = np.ones((3,3),np.uint8)
    er_plate = cv2.erode(th_plate,kernel,iterations = 1)
    er_invplate = er_plate
    result = pytesseract.image_to_string(er_invplate)
    flag = 0
    if len(result) == 4:
        try:
            print(int(result))
            print("parked : ",isParked)
            for i in range(len(isParked)):
                if isParked[i][0] == result:
                    flag = 1
                    print("exist")
                    break
            if flag == 0:
                isParked[carNum] = [result,1]     #register
                result += str(chr(carNum + 65))
                sendMsgToAndroid(android_socket, result) #1234A
                result = result[4:] + result[:4]
                sendMsgToAchro(achro_socket,result) #A1234
        except:
            print("ImageProcessing Error...Retry Capture__Except!!!!")
    else:
        print("ImageProcessing Error...Retry Capture__Len!!!!")

def connect():
    isConnectedPhone = False
    isConnectedAchro = False
    clientInfo = [0 for i in range(2)]

    while True:
        print("wait...")
        client_socket, addr = server_socket.accept()
        if addr[0] == '172.20.10.5':
            print("--------Achro connect--------")
            clientInfo[0] = (client_socket, addr)
            isConnectedAchro = True
        elif addr[0] == '172.20.10.2':
            print("--------Android connect--------")
            clientInfo[1] = (client_socket, addr)
            isConnectedPhone = True

        if isConnectedAchro and isConnectedPhone:
            break
        else:
            print("connected by...", addr)
    print("----------------------Start camThread and recvThread-----------------------")
    camThread = threading.Thread(target = startCam)
    camThread.start()
    #Achro Receiver Start
    recvFromAchroThread = threading.Thread(target = Receive_Achro, args =[clientInfo])
    recvFromAchroThread.start()
    #Android Receiver Start
    recvFromAndroidThread = threading.Thread(target = Receive_Android, args =[clientInfo])
    recvFromAndroidThread.start()
    
if __name__ == "__main__":
    connect()



    

