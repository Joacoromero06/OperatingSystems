import threading

c = 0
class solution1():
    def __init__(self, tam):
        c = 0
        self.tam = tam
    def solve():
        def loop():
            for i in range(self.tam):
                incrementar()
        def incrementar():
            global c
            temp = c 
            temp += 1
            c = temp
        loop()
    def show():
        print(c)

class test():
    def __init__(self, f, outF):
        threads = [threading.Thread(target=loop) for _ in range(10000)]
        run()
    def run(self):
        for t in threads: t.start()
        for t in threads: t.join()
        outF()

s1 = solution1()
test(s1.solve, s1.show)

