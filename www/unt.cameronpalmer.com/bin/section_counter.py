import re

infile = '../data/txt/1061/mathematics_1061.txt'
f = open(infile, "rb")

section = re.compile(r'^\s*\d{3}\s+\(\d+\)')
i = 0

for line in f:
    if section.match(line):
        i = i + 1

print i
