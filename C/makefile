compile:	map test
	gcc hash_map.o map_test.o -o map.out

map:	hash_map.c
	gcc -c -Wall hash_map.c

test:	map_test.c
	gcc -c -Wall map_test.c

clean:
	rm map.out hash_map.o map_test.o

rebuild:
	clean compile