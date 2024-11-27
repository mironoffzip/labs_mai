import time
import socket

HOST = "0.0.0.0"
PORT = 666

flag = False

if __name__ == '__main__':
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as serv_sock:
        serv_sock.bind((HOST, PORT))


        serv_sock.listen(1)
        while True:
            print("waiting for connection...")

            ServerLog = open('log.txt', 'a', encoding='cp866')
            if not flag:
                ServerLog.write(f"Server started:\t{time.ctime(time.time())}\n\n")
                flag = True

            sock, addr = serv_sock.accept()
            ServerLog.write(f"Connection time: {time.ctime(time.time())}\tConnection address: {addr}\n")
            with sock:
                while True:
                    try:
                        data = sock.recv(1024)
                    except ConnectionError:
                        ServerLog.write(f"Client suddenly while receiving\t{time.ctime(time.time())}\n")
                        break
                    if not data:
                        break
                    ServerLog.write(f"Message: {data.decode('cp866')}\tReceived: {time.ctime(time.time())}\n")
                    data = data[::-1]+b"\tWritten by Mironov A.D. M3O-110b-23"
                    time.sleep(5)
                    try:
                        sock.sendall(data)
                    except ConnectionError:
                        ServerLog.write(f"Client suddenly closed, cannot send\t{time.ctime(time.time())}\n")
                        break
                    ServerLog.write(f"Message: {data}\tSent: {time.ctime(time.time())}\n")
                    time.sleep(5)
                    ServerLog.write(f"Connection closed: {time.ctime(time.time())}\n\n")

