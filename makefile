GCC=gcc

sol:
	$(GCC) -o run -Wall *.c -lncursesw

debug:
	$(GCC) -g -o run -Wall *.c -lncursesw
