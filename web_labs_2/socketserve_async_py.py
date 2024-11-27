import selectors
import socket
from threading import Thread
import time

ServerLog = open('log.txt', 'a', encoding='utf-8')

def run(main):
    loop = get_event_loop()
    loop.run_forever(main)

loop = None

def get_event_loop():
    global loop
    if not loop:
        loop = SelectorLoop()
    return loop

class SelectorLoop:
    def __init__(self):
        super().__init__()
        self._selector = selectors.DefaultSelector()
        self._current_gen = None  # Currently executing generator
        self._ready = []
        self._run_forever_gen = None

    def run_forever(self, main_gen):
        self.create_task(main_gen)
        while True:
            self._run_once()

    def create_task(self, gen):
        self._ready.append(gen)

    def wait_for(self, fileobj):
        self._selector.register(fileobj, selectors.EVENT_READ, self._current_gen)
        yield  # Yield back to main loop to select other ready I/O objects

    def _run_once(self):
        self._process_tasks(self._ready)
        print("Waiting for connections or data...")
        events = self._selector.select()
        self._process_events(events)

    def _process_tasks(self, tasks):
        while tasks:
            self._run(tasks.pop(0))

    def _process_events(self, events):
        for key, mask in events:
            self._selector.unregister(key.fileobj)
            gen = key.data
            self._run(gen)

    def _run(self, gen):
        self._current_gen = gen
        try:
            next(gen)
        except StopIteration:  # Disconnected (returned, not yielded)
            pass

    def sock_accept(self, serv_sock):
        try:
            sock, addr = serv_sock.accept()
            sock.setblocking(False)
            return sock, addr
        except (BlockingIOError, InterruptedError):
            yield from self.wait_for(serv_sock)
            return (yield from self.sock_accept(serv_sock))

    def sock_recv(self, sock, nbytes):
        try:
            return sock.recv(nbytes)
        except (BlockingIOError, InterruptedError):
            yield from self.wait_for(sock)
            return (yield from self.sock_recv(sock, nbytes))

    def sock_sendall(self, sock, data):
        sock.sendall(data)


def main(host, port):
    global ServerLog
    flag = False
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as serv_sock:
        serv_sock.bind((host, port))
        serv_sock.listen(1)
        serv_sock.setblocking(False)
        print("Server started")

        loop = get_event_loop()
        while True:
            if not flag:
                ServerLog.write(f"Server started:\t{time.ctime(time.time())}\n\n")
                flag = True
            sock, addr = yield from loop.sock_accept(serv_sock)
            ServerLog.write(f"Connection time: {time.ctime(time.time())}\tConnection address: {addr}\n")
            loop.create_task(handle_connection(sock, addr))
            ServerLog = open('log.txt', 'a', encoding='utf-8')
            p = Thread(target=handle_connection, args=(sock, addr))
            p.start()


def handle_connection(sock, addr):
    global ServerLog
    ServerLog = open('log.txt', 'a', encoding='utf-8')
    print("Connected by", addr)
    while True:
        try:
            data = yield from loop.sock_recv(sock, 1024)
            data = data.decode('UTF-8')
        except ConnectionError:
            ServerLog.write(f"Client suddenly while receiving\t{time.ctime(time.time())}\n")
            break

        if not data:
            break  # EOF - closed by client

        ServerLog.write(f"Message: {data}\tReceived: {time.ctime(time.time())}\n")
        data = data[::-1] + ' | Written by Mironov A.D.'
        data = data.encode('UTF-8')
        time.sleep(5)

        try:
            loop.sock_sendall(sock, data)
        except ConnectionError:
            ServerLog.write(f"Client suddenly closed, cannot send\t{time.ctime(time.time())}\n")
            break

        ServerLog.write(f"Message: {data.decode('utf-8')}\tSent: {time.ctime(time.time())}\n")
        time.sleep(5)

    sock.close()
    ServerLog.write(f"Connection closed, {addr}")


HOST, PORT = "0.0.0.0", 666

if __name__ == "__main__":
    run(main(HOST, PORT))

ServerLog.close()
