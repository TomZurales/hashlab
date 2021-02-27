import sys

with open(sys.argv[1], 'r') as f:
    contents = f.read()
lines = contents.split('\n')[:-1]
ones_cnt = 0
count_by_pos = {}
for i in range(32):
    count_by_pos[i] = 0
for line in lines:
    bin_str = str(bin(int(line, 16))[2:])[::-1]
    for i, char in enumerate(bin_str):
        if char == '1': count_by_pos[i] += 1
for val in count_by_pos.values():
    print(val)
