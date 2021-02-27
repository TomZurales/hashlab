import sys

def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

with open(sys.argv[1], 'r') as f:
    contents = f.read()
cnts = {}
for i in range(1, 33):
    cnts[i] = 0;
lines = list(chunks(contents.split('\n')[:-1], 2))
for line in lines:
    cnt = str(bin(int(line[0], 16) & int(line[1], 16))).count("1")
    cnts[cnt] += 1
for cnt in cnts.values():
    print(cnt)
"""
avg = ones_cnt / (32 * len(lines))
print("Average: {}".format(avg))

std_dev = 0
for line in lines:
    line1 = str(bin(int(line, 16))[2:])
    for char in line1:
        std_dev = std_dev + ((float(char) - avg)**2)

print("Std Dev: {}".format(((std_dev / (len(lines) * 32))**-2)))
"""
