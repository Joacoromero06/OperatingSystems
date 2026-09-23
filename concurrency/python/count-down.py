import time

def count_down(n):
    while n > 0:
        n = n-1

start = time.time()
count_down(500000000)
end = time.time()
print(end-start)
