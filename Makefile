build:
	gcc -g tema3.c -o tema3 -Wall
check: build
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./tema3 < input/input26.txt
run:
	./tema3
clean:
	rm -rf tema3