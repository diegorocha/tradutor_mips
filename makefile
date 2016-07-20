all: clean build

bin/MIPSconst.o:
		gcc -c src/lib/MIPSconst.c -o bin/MIPSconst.o

bin/arrayUtils.o:
		gcc -c src/lib/arrayUtils.c -o bin/arrayUtils.o

bin/libMIPS.o:
		gcc -c src/lib/libMIPS.c -o bin/libMIPS.o

bin/tradutorMIPS: bin/MIPSconst.o bin/libMIPS.o	bin/arrayUtils.o
		gcc src/tradutorMIPS.c bin/libMIPS.o bin/MIPSconst.o bin/arrayUtils.o -lm -o bin/tradutorMIPS

build: bin/libMIPS.o bin/MIPSconst.o bin/arrayUtils.o bin/tradutorMIPS

build-debug: clean
		gcc -c src/lib/libMIPS.c -g -o bin/libMIPS.o
		gcc -c src/lib/MIPSconst.c -g -o bin/MIPSconst.o
		gcc -c src/lib/arrayUtils.c -g -o bin/arrayUtils.o
		gcc src/tradutorMIPS.c bin/libMIPS.o bin/MIPSconst.o bin/arrayUtils.o -g -lm -o bin/tradutorMIPS

clean:
		rm -f bin/tradutorMIPS
		rm -f bin/libMIPS.o
		rm -f bin/arrayUtils.o
		rm -f bin/MIPSconst.o
		
run: bin/tradutorMIPS
		bin/tradutorMIPS
		
debug: build-debug
		gdb bin/tradutorMIPS

run-test: build
		bin/tradutorMIPS test/input.asm test/output --newline

debug-test: build-debug
		gdb --args bin/tradutorMIPS test/input.asm test/output --newline
