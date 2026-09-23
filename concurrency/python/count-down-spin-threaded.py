from threading import Thread
from subprocess import Popen
import time

def countdown(n):
    while n > 0:
        n -= 1

p = Popen(['python3', 'spin.py'])

N = 500_000_000
t1 = Thread(target=countdown, args=(N,))
t2 = Thread(target=countdown, args=(N,))

start = time.time()
t1.start(); t1.join()
t2.start(); t2.join()
end = time.time()

print(end-start)
p.terminate()

