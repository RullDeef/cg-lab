import sys
from random import random

n = 10 if len(sys.argv) == 1 else int(sys.argv[1])
m = 5 # boundaries

data = [round(2 * m * (random() - 0.5), 1) for i in range(2 * n)]
print(*data, sep=", ", file=open("points.csv", "w"))
