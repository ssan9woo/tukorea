#Pyhton <--> Java [Server Client 소켓통신 TestCode]
import socket

host = '192.168.0.185'
port = 9999

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((host, port))

print("complete")

try:
    while True:
        s = input()
        client_socket.sendall(bytes(s +"\n", 'UTF-8'))
except Exception as e:
    print(e)

#client_socket.close()
