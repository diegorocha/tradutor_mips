all: clean bin/tradutorMIPS

bin/tradutorMIPS:
		gcc src/tradutorMIPS.c -o bin/tradutorMIPS

build: bin/tradutorMIPS

clean:
		rm -f bin/tradutorMIPS

clean-run: clean bin/tradutorMIPS
		bin/tradutorMIPS
		
run: bin/tradutorMIPS
		bin/tradutorMIPS
		
debug: clean
		gcc src/tradutorMIPS.c -g -o bin/tradutorMIPS
		gdb bin/tradutorMIPS
		
build-run: clean build run

run-test: clean build
		bin/tradutorMIPS test/input test/ouput

debug-test: clean
		gcc src/tradutorMIPS.c -g -o bin/tradutorMIPS
		gdb --args bin/tradutorMIPS test/input test/ouput
