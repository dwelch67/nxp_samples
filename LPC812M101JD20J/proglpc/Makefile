
all : proglpc program

proglpc : proglpc.c ser.c ser.h
	gcc proglpc.c ser.c -o proglpc

program : program.c ser.c ser.h
	gcc program.c ser.c -o program

clean :
	rm -f proglpc
	rm -f program
