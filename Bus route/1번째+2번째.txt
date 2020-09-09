import xlwings as xw
import os
os.environ["GOOGLE_API_KEY"] = "AIzaSyCfJkU1gZiOYQiN2Yvim73WQrrgixmcWSA"
import geocoder
import time
import pandas as pd
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import seaborn as sns
from collections import defaultdict
from heapq import *
from haversine import haversine
import openpyxl


excel = xw.Book('address.xlsx')             #100개의 학생주소를 받아온다.
sht = excel.sheets['Sheet1']                
py_list = sht.range('B2:B101').value        
df = pd.DataFrame(columns = ('x','y'))

i = 0
rowCount = 1
z = []
for address in py_list :                    #google_geocoder을 사용해 100개의 한글주소를 위도경도로 변환 한 후 list에 넣는다
    g = geocoder.google(address)
    if g.status == "OVER_QUERY_LIMIT":
        while True:
            print("Fail ===> Try Again")
            time.sleep(0.5)
            g = geocoder.google(address)
            if g.status == "OK":
                break
    geo = g.latlng
    df.loc[i] = geo                                     #변환시킨 x,y값[geo]을 Dataframe에 넣는다.
    i += 1
    z.append(geo)
    print("loading...",i)
print("complete!!!--------------------------------")    

data_points = df.values
geosch = geocoder.google("경기도 시흥시 정왕동 2121")    #학교의 좌표도 위도경도로 변환
school = geosch.latlng
Kmeans = KMeans(n_clusters=12).fit(data_points)          # K-평균 알고리즘으로 학생 100명의 분포도에 맞게 12개의 노드를 생성
df['cluster_id'] = Kmeans.labels_
centers = Kmeans.cluster_centers_                        
centers = centers.tolist()      
centers.sort(key = lambda x : x[0])                     
centers.append(school)#--------------------------------> 12개의 정류장을 만들고 정류장 좌표를 sort시켜 고정좌표로 만든다.

centers = z + centers                                    #100개의 위,경도 값과 정류장의 위,경도 좌표를 연속으로 붙힌다.
excel.close()                                            #(코드길이를 줄이기 위함)


excel2 = openpyxl.load_workbook('address.xlsx')
sheet2 = excel2.create_sheet('Cordinates')
for i in range(len(centers)):                            #구한 100 + 13개의 좌표를 또다른 엑셀로 넣기 위한 작업
    sheet2.cell(row=i+2,column=1).value = i              #다른 엑셀에 넣지 않고 두 코드를 합쳐서 컴파일한다면 k평균에 의한
    sheet2.cell(row=i+2,column=2).value = centers[i][0]  #정류장 좌표가 계속 달라지기 때문에 같은 주소여도 노선이 컴파일 
    sheet2.cell(row=i+2,column=3).value = centers[i][1]  #할 떄마다 바뀌게 된다. 그것을 방지하기 위해 코드를 2개로 분산.
    #[초기 주소에 대한 정보를 엑셀의 다른 Sheet로 넣는코드 + 그 엑셀에 Data를 가져와서 노선을 생성하는 코드]
excel2.save('address.xlsx')                              #Data를 넣은 후 엑셀파일을 저장한다.
excel2.close()

excel = xw.Book('address.xlsx')                             #엑셀을 사용하기 위한 코드 excel로 불러온다.
sht = excel.sheets['Cordinates']               
centers = sht.range('B102:B114','C102:C114').value          #centers == centers_address 두개를 각기 다른용도로 사용하기 위함.
centers_address = sht.range('B102:B114','C102:C114').value  #13개의 클러스터 정보를 가져온다.
cordinate = sht.range('B2:B101','C2:C101').value            #사람들의 좌표를 불러온다.

        
sht2 = excel.sheets['Sheet1']                               
ad_korean = sht2.range('B2:B101').value                     #한글 주소를 불러오기 위한 코드

route = [] #만들어진 노선을 저장하기 위한 리스트
x = []      
min_dis = []

for i in range(len(centers)):
    list1 = []
    for j in range(len(centers)):
        if i != j:
            list1.append([chr(65+j),haversine(centers[i],centers[j])])
    list1.sort(key = lambda x : x[1])
    min_dis.append(list1[0])
    for k in range(5):
        x.append(list1[k])   
 
roadMap = {'A':{},'B':{},'C':{},'D':{},'E':{},'F':{},'G':{},'H':{},'I':{},'J':{},'K':{},'L':{},'M':{}}

for i in range(13):             #다익스트라 최단경로를 만들기 위해 각 노드간 가중치를 가까운 5개의 노드로 잡는다.
    b = {}
    b = dict(x[i*5 : 5*(i+1)])
    roadMap[chr(65+i)] = b




fastWay={}
for i in roadMap.keys():
    fastWay[i]=[0, 99999999, 0] #각 노드에 대해 visit여부,length(가중치),이전경로에 대한 정보를 넣기위해 list로 선언

def findNext():                 #다음 경로를 찾아나가기 위한 함수.
    nextPlace='noWay'
    min=99999999
    for i in fastWay.keys():
        if fastWay.get(i)[0] == 0 and min > fastWay.get(i)[1]:
            nextPlace=i
            min=fastWay.get(i)[1]
    return nextPlace
    
    
def markingPath(place):         #지나온 경로를 마킹하고, 지나온 거리의 가중치를 더해주는 함수
    for i in roadMap.get(place).keys():
        if fastWay.get(i)[0]==0 and fastWay.get(i)[1] > fastWay.get(place)[1] + roadMap.get(place).get(i):
            fastWay.get(i)[1] = fastWay.get(place)[1] + roadMap.get(place).get(i)
            fastWay.get(i)[2] = place

            

loc_cnt = [0,0,0,0,0,0,0,0,0,0,0,0]     #학교를 제외한 12개의 노드에 한번이라도 들렸다면 0 -> 1로 바꿔서 지나간곳 확인
root = []                               #3개의 노선을 표시하기 위한 리스트
length_way = []                         #3개의 노선의 거리를 표시하기 위한 리스트

for i in range(3):
    max = 0
    index = 0
    visit = []
    for i in range(len(centers)-1):
        dir = haversine(centers[len(centers)-1],centers[i])
        if dir >= max:
            max = dir
            index = i


    start = chr(65+index)               #위에 for문으로 학교에서 가장 먼 3곳을 찾고 시작점으로 위치시킨다.
    finish = 'M'
    fastWay.get(start)[0]=1             #최초 시작점 방문처리
    fastWay.get(start)[1]=0             #최초 시작점 거리0
    fastWay.get(start)[2]='start'
    markingPath(start)     
    
    while True:                                             
        move = findNext()               # 다음 가야할곳을 move로 
        if move=='noWay':break          # 더이상 갈길이없으면 break
        fastWay.get(move)[0]=1          #visit
        markingPath(move)

    rootWay=[]
    result=''
    lenth=fastWay.get(finish)[1]
    while True:                         #경로 작성
        if finish=='start':break       
        rootWay.append(finish)
        finish=fastWay.get(finish)[2]
        #시작점에서 marking path를 하고 전체중에 제일 짧은곳을 scan한다음(findNext) 다시 그곳에서 marking!
    
    while rootWay:
        visit.append(rootWay.pop())     #노선의 위치를 하나씩 pop시켜 꺼내온다.
    length_way.append(lenth)            #가중치를 각 노드의 맞게 12개의 list에 넣는다
    
    for i in range(len(visit)):
        visit_index = ord(visit[i]) - 65
        centers[visit_index][0] = centers[12][0]    #처음 방문했던 곳들은 학교와 같은좌표로 설정해 다시 학교에서 최대로 먼
        centers[visit_index][1] = centers[12][1]    #노드를 구할 때 나올 수 없게 만든다.
    
    for i in range(len(visit)):         #지나온 모든 경로를 list에 넣는다(3번의 for문으로 각각 완료된 노선을 
        route.append(visit[i])
    root.append(visit)                  #root라는 list에 넣는다. 
                                        #for문 3번이 끝나면 root에는 3개의 노선이 저장된다.
    for i in roadMap.keys(): 
        for j in route:      
            if j != 'M':
                if i == j:              #학교를 제외하고 지나온 노드들은 visit으로 바꾸고 다른 노선의 가중치를 구하기 위해 초기화.
                    fastWay[i] = [1,99999999,0]
                    break;
                else:
                    fastWay[i]=[0, 99999999, 0]
                loc_cnt[ord(j)-65] = 1              
    lenth = 0 #가중치 초기화

index_noway = [] #지나오지 않은 정류장 들을 표시하기 위한 리스트
k = 0
flag = 0
flag2 = 0
for i in range(len(loc_cnt)):
    if loc_cnt[i] == 0:
        index_noway.append(chr(i+65)) 
#-----------index_noway -> 지나가지 않은 노드들을 저장 (다익스트라 알고리즘은 최소경로이기 때문에 최소한 한개이상 노드가 남을 확률이 크다. 따라서 노선에 없는 정
#                          따라서 노선에 없는 정류장들을 사이사이에 넣어줄 알고리즘이 필요하다. 바로밑줄부터 그 알고리즘이 실행된다.
#                          알고리즘에 따라 12개의 클러스터로 4개의 정류장, 학교까지 1개를 해서 총 5/5/5개의 정류장 3개를 만든다.
for i in index_noway:
    test = []
    for j in range(13):
        if j != ord(i) - 65:
            test.append([chr(65 + j),haversine(centers[ord(i) - 65],centers[j])])
    test.sort(key = lambda x : x[1])

    while(k < 12):
        alpa = test[k][0]
        for third in range(3):
            for two in range(len(root[third])):        #k = two , j = third , i = 그대로
                if alpa == root[third][two] and len(root[third]) < 5:
                    flag = 1
                    break
            if flag == 1:
                flag2 = 1
                break
        if flag2 == 1:
            break
        k+=1
    k = 0
    flag = 0
    flag2 = 0
    if two == 0:
        new_length = haversine(centers[ord(root[third][two]) - 65],centers[ord(i) - 65]) + haversine(centers[ord(root[third][two + 1]) - 65],centers[ord(i) - 65]) - haversine(centers[ord(root[third][two]) - 65],centers[ord(root[third][two + 1]) - 65])
        length_way[third] += new_length
        root[third].insert(1,i)

    elif two == len(root[third]) - 1:
        new_length = haversine(centers[ord(root[third][two]) - 65],centers[ord(i) - 65]) + haversine(centers[ord(root[third][two - 1]) - 65],centers[ord(i) - 65]) - haversine(centers[ord(root[third][two]) - 65],centers[ord(root[third][two - 1]) - 65])
        length_way[third] += new_length
        root[third].insert(1,i)
    else:
        dir1 = ord(root[third][two - 1]) - 65 #3
        dir2 = ord(root[third][two + 1]) - 65 #2
        dir1 = haversine(centers[ord(i) - 65],centers[dir1])
        dir2 = haversine(centers[ord(i) - 65],centers[dir2])
        if dir1 > dir2:
            new_length = haversine(centers[ord(root[third][two]) - 65],centers[ord(i) - 65]) + haversine(centers[ord(root[third][two + 1]) - 65],centers[ord(i) - 65]) - haversine(centers[ord(root[third][two]) - 65],centers[ord(root[third][two + 1]) - 65])
            length_way[third] += new_length
            root[third].insert(two + 1,i)
        else:
            new_length = haversine(centers[ord(root[third][two]) - 65],centers[ord(i) - 65]) + haversine(centers[ord(root[third][two - 1]) - 65],centers[ord(i) - 65]) - haversine(centers[ord(root[third][two]) - 65],centers[ord(root[third][two - 1]) - 65])
            length_way[third] += new_length
            root[third].insert(two,i)


kr_station = []                 #노드(정류장)을 한글로 바꿀 list
for i in range(3):              #root자체를 kr_station 에 복사하면 root까지 변경되는 오류가 계속발생. 따라서  
    line = []                   #root와 동일한 크기의 kr_station이라는 이중 리스트를 생성하는 작업
    for j in range(len(root[i])):
        line.append(0)
    kr_station.append(line)


min = 99999999
for k in range(3):                       #정류장 좌표를 reverse_geocoder 하면 ex) [경기도 광주시] 까지밖에 안나오므로 그 
    for j in range(len(kr_station[k])-1):#정류장과 학생의 주소 100개중 가장 가까운곳의 주소를 정류장의 위치로 놓기 위한 알고리즘
        for i in range(100):
            c = haversine(centers_address[ord(root[k][j])-65],cordinate[i])
            if c < min:
                min = c
                inum = i
        min = 9999    
        kr_station[k][j] = ad_korean[inum]      #ad_korean은 100명의 한글주소를 받아온 배열
                                                


for i in range(3):
    print("[노선",i+1,"]---------------------------------------------------------------",end='')
    print("----------------------------------------------")
    print("학교와 ",i+1,"번째로 먼 곳은",kr_station[i][0],"입니다,\n")
    for j in range(len(kr_station[i])-1):
        print(kr_station[i][j]," -> ",end='')
    print("학교")
    print("노선의 거리[Km] : ",length_way[i],"\n\n")

student = sht.range('B2:B101','C2:C101').value          #학생 100명에 대해 거리와 경로를 구하기 위해 좌표를 불러온다.
print("-----------------------------------------------------------------------------------------------------------------------")
student_num = input("\n학생 번호(id)를 입력하세요[1~100] :")
student_num = int(student_num)-1
print("학생의 주소 :[",ad_korean[student_num],"]\n")


min = 99999999
min_index = 0
walk_distance = 0
bus_distance = 0

for i in range(13):
    d = haversine(student[student_num],centers[i])
    if d < min:
        min = d
        walk_distance = min
        min_index = i



min_index = chr(min_index+65) #학생이랑 젤 가까운 centers의 index 번호가 min_index, 그걸 문자로 변환.
for j in range(3):            #학생이 노선의 중간에 껴있을 경우 거리를 구하는 알고리즘.
    for k in range(len(root[j])):
        if min_index == root[j][k]:
            print("학생이 타야 하는 정류장의 위치 :",kr_station[j][k],"\n")
            if k == 0:
                bus_distance = length_way[k]
            else:
                for i in range(k):
                    bus_distance += haversine(centers[ord(root[j][i])-65],centers[ord(root[j][i+1])-65])  
                bus_distance = length_way[j] - bus_distance
                break
#걷는 속도 5Km , 버스의 속도 70Km, 택시의 속도 80Km 로 가정
print("학생의 집 -> 정류장 -> 학교까지의 거리는",end='')
print("도보[Km] :",walk_distance,"  버스[Km] :",bus_distance,"입니다.\n")

while(1):
    taxi_flag = input("택시를 타시겠습니까? 탄다[1] 안탄다[0] :")
    if int(taxi_flag) == 1:
        walk_t = walk_distance/80 
        break
    elif int(taxi_flag) == 0:
        walk_t = walk_distance/5
        break
    else:
        print("잘못 입력했습니다")

bus_t = round(bus_distance/70,1)
hour = int(walk_t) + int(bus_t)
minute = int((walk_t - int(walk_t))*60) + int((bus_t - int(bus_t))*60)
if minute >= 60:
    minute = 0
    hour += 1
print("\n\n-> 학생",student_num+1,"의 등교시간은 ",hour,"시간",minute,"분 입니다.")


