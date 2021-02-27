import sys
import binascii

with open(sys.argv[1], 'r') as f:
    contents = f.read()
lines = contents.split('\n')[:-1]
ones_cnt = 0
for line in lines:
    ones_cnt = ones_cnt + str(bin(int(line, 16))[2:]).count('1')
avg = ones_cnt / (32 * len(lines))
print("Average: {}".format(avg))

std_dev = 0
for line in lines:
    line1 = str(bin(int(line, 16))[2:])
    for char in line1:
        std_dev = std_dev + ((float(char) - avg)**2)

print("Std Dev: {}".format(((std_dev / (len(lines) * 32))**-2)))

