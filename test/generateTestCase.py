from random import seed
from random import randint

seed(1)

w = 500
h = 500
to = w * h
colors = 20

print(h, w, colors)
for i in range(to):
	value = randint(1, colors)
	if i != 0 and i % w == 0 and i != to - 1:
		print()
	print(value, end ="")
	if i % w != w-1:
		print(" ", end ="")
