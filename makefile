all: clean bin/tradutorMIPS

bin/tradutorMIPS:
		gcc src/tradutorMIPS.c -o bin/tradutorMIPS

build: bin/tradutorMIPS

clean:
		rm -f bin/tradutorMIPS

execute: bin/tradutorMIPS
		bin/tradutorMIPS
		
debug: clean
		gcc src/tradutorMIPS.c -g -o bin/tradutorMIPS
		gdb bin/tradutorMIPS
