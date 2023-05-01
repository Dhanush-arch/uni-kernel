all: stack-overwrite-64-2 check-stack

check-stack: check-stack.c
	musl-gcc check-stack.c -o check-stack

stack-overwrite-64-2: stack-overwrite-64-2.c
	musl-gcc -fno-stack-protector stack-overwrite-64-2.c -o stack-overwrite-64-2

clean:
	rm -f stack-overwrite-64-2 check-stack

