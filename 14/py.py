
start1 = 33
k1 = 103

start2 = 84
k2 = 101

l1 = list()
l2 = list()

for i in range(100):
    l1.append(start1 + k1*i)
    l2.append(start2 + k2*i)

for i1 in l1:
    for i2 in l2:
        if i1 == i2:
            print(i1)
            break