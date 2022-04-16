"""
Python script to quickly turn a file full of numbers, one on each line, like so:
2
3
5
...
into C array format, like so:
int_primes[] = {
2,
3,
5,
};
(The comma after the last element will need to be manually deleted)
"""

# Using readlines()
file1 = open('prime1.h', 'r')
Lines = file1.readlines()

file2 = open('primes.h', 'w')

file2.write("int primes[] = {\n")
 
count = 0
# Strips the newline character
for line in Lines:
    count += 1
    file2.write(f"{line.strip()},\n")

file2.write("};")

file1.close()
file2.close()