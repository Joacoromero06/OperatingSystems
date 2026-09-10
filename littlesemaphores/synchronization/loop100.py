c = 0
for i in range(100):
    # --- 
    temp = c # fetch()
    # ---
    temp += 1 # work()
    # --- in moment t all threads can fetch and work concurrently but all have to wait for all threads to finish fetching and working so something like waitall() and in some way the las thread wakeup all threads this can done by an counter shared that when the counter reaches 100 wake up all or another semantic equivalent mechanism
    c = temp # write()
    # ---

c = 0
for i in range(100):
    # take the lock
    temp = c
    temp += 1
    c = temp
    # give up the lock this mechanism is easy but probably other more general techniques exists
