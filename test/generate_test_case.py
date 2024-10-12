from random import seed
from random import randint

seed(1)

W = 500
H = 500
TO = W * H
COLORS = 20

print(H, W, COLORS)
for i in range(TO):
    value = randint(1, COLORS)
    if i != 0 and i % W == 0 and i != TO - 1:
        print()
    print(value, end ="")
    if i % W != W-1:
        print(" ", end ="")
