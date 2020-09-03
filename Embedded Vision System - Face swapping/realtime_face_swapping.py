import os
import cv2
import numpy as np
import dlib
import time
from tkinter import *
import pytesseract
import PIL
from PIL import Image,ImageTk

global list1
list1 = ["You_Byeongjae","You_Seyun","jim_carrey","problem","hanyeseul","Park","trumfe","Park_Geunhye","jungwoosung","Lee_Byeongbak","Kim_Junhyeon","Hong_Yunhwa","hajeonwoo","BTS_JIN",]

def extract_index_nparray(nparray):
    index = None
    for num in nparray[0]:
        index = num
        break
    return index


def quit_cam():
    root.destroy()

def screenshot():
    img_sc = cap.read()[1]
    img_sc = cv2.flip(img_sc,1)
    name = entry_name.get()
    print(name)
    cv2.imwrite(name +'.jpg' , img_sc)
    cv2.waitKey(0)
    list1.append(name)
    cv2.imshow("snapshot",img_sc)
    
def get_image():
    mylist = root.grid_slaves()
    mylist2 = root.place_slaves()
    for i in mylist:
        i.destroy()
    for i in mylist2:
        i.destroy()

    global cap
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 500)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 500)

    root.geometry("500x550")
    lmain = Label(root)
    button = Button(root,text = "끝내기",command = back,width = 8,fg = "red",bd = 5,font = "Verdana 10 bold")
    button2 = Button(root, text = "snapshot",command = screenshot,fg = "blue",font = "Verdana 10 bold",bd = 5,width = 8)
    global entry_name
    entry_name = Entry(root,width = 15)
    label = Label(root, text = "이름 입력(영문) : ")

    label.place(x = 0, y = 505)
    entry_name.place(x = 98, y = 505)
    lmain.place(x = 0, y = 0)
    button.place(x = 414, y = 520)
    button2.place(x = 220, y = 500)

    def show_frame():
        _, frame = cap.read()
        frame = cv2.flip(frame, 1)
        cv2image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA)
        img = PIL.Image.fromarray(cv2image)
        imgtk = ImageTk.PhotoImage(image=img)
        lmain.imgtk = imgtk
        lmain.configure(image=imgtk)
        lmain.after(10, show_frame)
    show_frame()
    root.mainloop()
    
    
def clear():
    root.geometry("400x400")
    global img_ex
    img_ex = PhotoImage(file = "startmap2.png").subsample(2)
    
    mylist = root.grid_slaves()
    mylist2 = root.place_slaves()
    for i in mylist:
        i.destroy()
    for i in mylist2:
        i.destroy()       
    frame = Frame(root)
    frame.grid(row = 0, column = 0)
    frame2 = Frame(root)
    frame2.place(x = 0,y = 30)
    frame4 = Frame(root)
    frame4.place(x = 229,y = 375)
    frame5 = Frame(root)
    frame5.grid(row = 3, column = 0,pady = 50)
    frame6 = Frame(root)
    frame6.place(x = 88,y = 60)
    frame7 = Frame(root)
    frame7.place(x = 25, y = 90)
    frame3 = Frame(root)
    frame3.place(x = 220, y = 0)
    frame8 = Frame(root)
    frame8.place(x = 0, y = 250)
    frame9 = Frame(root)
    frame9.place(x = 50, y = 320)

    
    label1 = Label(frame,text = "1. 바꿀 대상의 이름을 입력하세요")
    label2 = Label(frame2,text = " Ex)  입력 : ")
    entry = Entry(frame2,bd = 5,width = 15)
    label2_1 = Label(frame6, text = "↓")
    label2_2 = Label(frame7, image = img_ex)
    label3 = Label(frame3, text = "2.  리스트 보기")
    button3 = Button(frame3, text = "[보기]",command = listbox)
    button2 = Button(frame4, text = "[뒤로가기]",command = back)
    label4 = Label(frame8, text = "---------------------[ 바꿀 대상의 이름을 입력하세요 ]----------------------")
    label5 = Label(frame9, text = "이름 : ")
    global entry2
    entry2 = Entry(frame9, bd = 5, width = 15)
    button1 = Button(frame9, text = "Start!!!",command = start)
    
    #frame1
    label1.grid(row = 0, column = 0)
    #frame2
    label2.grid(row = 0, column = 0)
    entry.grid(row = 0, column = 1)
    entry.insert(0,"You_Byeongjae")
    #frame3
    label3.grid(row = 0, column = 0)
    button3.grid(row = 0, column = 1,padx = 12)
    #frame4
    button2.grid(row = 0, column = 0,padx = 105)
    #frame6
    label2_1.grid(row = 1, column = 0)
    #frame7
    label2_2.grid(row = 0, column = 0)
    #frame8
    label4.grid(row = 0, column = 0)
    #frame9
    label5.grid(row = 0, column = 0)
    entry2.grid(row = 0, column = 1,padx = 10)
    button1.grid(row = 0, column = 2, padx = 10)
    


def listbox():
    frame = Frame(root)
    frame.place(x = 220, y = 30)

    scroll = Scrollbar(frame)
    listbox = Listbox(frame,yscrollcommand = scroll.set)
    for i in range(len(list1)):
        listbox.insert(i+1,list1[i])
    scroll["command"] = listbox.yview
    
    listbox.grid(row = 0, column = 0)
    scroll.grid(row = 0, column = 1)
    listbox.bind('<Double - 1>',click)

def click(event):
    widget = event.widget
    selection=widget.curselection()
    value = widget.get(selection[0])
    click_image = value + ".jpg"

    global img_click
    img_click = cv2.imread(click_image)
    print(img_click.shape[0])
    #0 : 높이 1 : 너비 
    if img_click.shape[0] > 500 or img_click.shape[1] > 500:
        img_click = cv2.resize(img_click, None, fx = 0.5, fy = 0.5)
    cv2.imshow('image', img_click)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

       
def back():
    root.geometry("400x400")
    mylist = root.place_slaves()
    for i in mylist:
        i.destroy()
    root.title("얼굴바꾸기")
    
    frame = Frame(root)
    frame.grid(row = 0, column = 0)
    label1 = Label(frame,image = start_image)
    label1.grid(row = 0, column = 0)


    frame2 = Frame(root)
    frame2.place(x = 50, y = 325)
    button = Button(frame2, text = "1. 얼굴바꾸기",command = clear,fg = "blue",font = "Verdana 10 bold")
    button.grid(row = 0, column = 0, pady = 10, padx = 30)
    button2 = Button(frame2, text = "2.  사진추가 ",fg = "blue",font = "Verdana 10 bold",command = get_image)
    button2.grid(row = 0, column = 1, pady = 10, padx = 30)
    
def start():
    global string
    string = entry2.get() + ".jpg"
    
    global start_flag
    start_flag = 1;
    print(start_flag)
    root.destroy();


root = Tk()

global img_man
img_man = PhotoImage(file = "man.png").subsample(2)
global start_image
start_image = PhotoImage(file = "startmap.png")#.subsample(2)


root.title("얼굴바꾸기")
root.geometry("400x400")

frame = Frame(root)
frame.grid(row = 0, column = 0)
label1 = Label(frame,image = start_image)
label1.grid(row = 0, column = 0)


frame2 = Frame(root)
frame2.place(x = 50, y = 325)
button = Button(frame2, text = "1. 얼굴바꾸기",command = clear,fg = "blue",font = "Verdana 10 bold")
button.grid(row = 0, column = 0, pady = 10, padx = 30)
button2 = Button(frame2, text = "2.  사진추가 ",fg = "blue",font = "Verdana 10 bold",command = get_image)
button2.grid(row = 0, column = 1, pady = 10, padx = 30)



root.mainloop()


img = cv2.imread(string)
print(string)
img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
mask = np.zeros_like(img_gray)

cap = cv2.VideoCapture(0)

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")

indexes_triangles = []

# Face 1
faces = detector(img_gray)
for face in faces:
    landmarks = predictor(img_gray, face)
    landmarks_points = []
    for n in range(0, 68):
        x = landmarks.part(n).x
        y = landmarks.part(n).y
        landmarks_points.append((x, y))

        # cv2.circle(img, (x, y), 3, (0, 0, 255), -1)

    points = np.array(landmarks_points, np.int32)
    convexhull = cv2.convexHull(points)
    # cv2.polylines(img, [convexhull], True, (255, 0, 0), 3)
    cv2.fillConvexPoly(mask, convexhull, 255)

    face_image_1 = cv2.bitwise_and(img, img, mask=mask)

    # Delaunay triangulation
    rect = cv2.boundingRect(convexhull)
    subdiv = cv2.Subdiv2D(rect)
    subdiv.insert(landmarks_points)
    triangles = subdiv.getTriangleList()
    triangles = np.array(triangles, dtype=np.int32)

    indexes_triangles = []
    for t in triangles:
        pt1 = (t[0], t[1])
        pt2 = (t[2], t[3])
        pt3 = (t[4], t[5])

        index_pt1 = np.where((points == pt1).all(axis=1))
        index_pt1 = extract_index_nparray(index_pt1)

        index_pt2 = np.where((points == pt2).all(axis=1))
        index_pt2 = extract_index_nparray(index_pt2)

        index_pt3 = np.where((points == pt3).all(axis=1))
        index_pt3 = extract_index_nparray(index_pt3)

        if index_pt1 is not None and index_pt2 is not None and index_pt3 is not None:
            triangle = [index_pt1, index_pt2, index_pt3]
            indexes_triangles.append(triangle)


while(1):
    while(start_flag == 0):
        print("A")
        
    while True:
        
        _, img2 = cap.read()
        print(img2)
        img2_gray = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
        img2_new_face = np.zeros_like(img2)

        # Face 2
        faces2 = detector(img2_gray)
        for face in faces2:
            landmarks = predictor(img2_gray, face)
            landmarks_points2 = []
            for n in range(0, 68):
                x = landmarks.part(n).x
                y = landmarks.part(n).y
                landmarks_points2.append((x, y))

            # cv2.circle(img2, (x, y), 3, (0, 255, 0), -1)
            points2 = np.array(landmarks_points2, np.int32)
            convexhull2 = cv2.convexHull(points2)

        lines_space_mask = np.zeros_like(img_gray)
        lines_space_new_face = np.zeros_like(img2)

        # Triangulation of both faces
        for triangle_index in indexes_triangles:
            # Triangulation of the first face
            tr1_pt1 = landmarks_points[triangle_index[0]]
            tr1_pt2 = landmarks_points[triangle_index[1]]
            tr1_pt3 = landmarks_points[triangle_index[2]]
            triangle1 = np.array([tr1_pt1, tr1_pt2, tr1_pt3], np.int32)

            rect1 = cv2.boundingRect(triangle1)
            (x, y, w, h) = rect1
            cropped_triangle = img[y: y + h, x: x + w]
            cropped_tr1_mask = np.zeros((h, w), np.uint8)

            points = np.array([[tr1_pt1[0] - x, tr1_pt1[1] - y],
                               [tr1_pt2[0] - x, tr1_pt2[1] - y],
                               [tr1_pt3[0] - x, tr1_pt3[1] - y]], np.int32)

            cv2.fillConvexPoly(cropped_tr1_mask, points, 255)
            
            # Triangulation of second face
            tr2_pt1 = landmarks_points2[triangle_index[0]]
            tr2_pt2 = landmarks_points2[triangle_index[1]]
            tr2_pt3 = landmarks_points2[triangle_index[2]]
            triangle2 = np.array([tr2_pt1, tr2_pt2, tr2_pt3], np.int32)

            rect2 = cv2.boundingRect(triangle2)
            (x, y, w, h) = rect2

            cropped_tr2_mask = np.zeros((h, w), np.uint8)

            points2 = np.array([[tr2_pt1[0] - x, tr2_pt1[1] - y],
                                [tr2_pt2[0] - x, tr2_pt2[1] - y],
                                [tr2_pt3[0] - x, tr2_pt3[1] - y]], np.int32)

            cv2.fillConvexPoly(cropped_tr2_mask, points2, 255)



            # Warp triangles
            points = np.float32(points)
            points2 = np.float32(points2)
            M = cv2.getAffineTransform(points, points2)
            warped_triangle = cv2.warpAffine(cropped_triangle, M, (w, h))
            warped_triangle = cv2.bitwise_and(warped_triangle, warped_triangle, mask=cropped_tr2_mask)


            # Reconstructing destination face
            img2_new_face_rect_area = img2_new_face[y: y + h, x: x + w]
            img2_new_face_rect_area_gray = cv2.cvtColor(img2_new_face_rect_area, cv2.COLOR_BGR2GRAY)
            _, mask_triangles_designed = cv2.threshold(img2_new_face_rect_area_gray, 1, 255, cv2.THRESH_BINARY_INV)
            warped_triangle = cv2.bitwise_and(warped_triangle, warped_triangle, mask=mask_triangles_designed)

            img2_new_face_rect_area = cv2.add(img2_new_face_rect_area, warped_triangle)
            img2_new_face[y: y + h, x: x + w] = img2_new_face_rect_area


        # Face swapped (putting 1st face into 2nd face)
        img2_face_mask = np.zeros_like(img2_gray)
        img2_head_mask = cv2.fillConvexPoly(img2_face_mask, convexhull2, 255)
        img2_face_mask = cv2.bitwise_not(img2_head_mask)


        img2_head_noface = cv2.bitwise_and(img2, img2, mask=img2_face_mask)
        result = cv2.add(img2_head_noface, img2_new_face)

        (x, y, w, h) = cv2.boundingRect(convexhull2)
        center_face2 = (int((x + x + w) / 2), int((y + y + h) / 2))

        seamlessclone = cv2.seamlessClone(result, img2, img2_head_mask, center_face2, cv2.MIXED_CLONE)
        

        img2 = cv2.flip(img2, 1)
        cv2.imshow("img2", img2)
        cv2.imshow("clone", seamlessclone)
        
        key = cv2.waitKey(1)
        if key == 27:
            break
    break;


cap.release()
cv2.destroyAllWindows()
