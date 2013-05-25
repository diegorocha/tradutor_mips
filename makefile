# Se o primeiro argumento é "run"
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # Usa os outros argumentos como argumento do run
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

all: clean bin/tradutorMIPS

bin/tradutorMIPS:
		gcc src/tradutorMIPS.c -o bin/tradutorMIPS

build: bin/tradutorMIPS

clean:
		rm -f bin/tradutorMIPS

execute: bin/tradutorMIPS
		bin/tradutorMIPS
		
run: bin/tradutorMIPS
		bin/tradutorMIPS $(RUN_ARGS)
		
debug: clean
		gcc src/tradutorMIPS.c -g -o bin/tradutorMIPS
		gdb bin/tradutorMIPS
		
build-run: clean build run
