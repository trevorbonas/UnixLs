make:
	gcc -ggdb -Wall -o UnixLs driver.c infodemo.c UnixLs.c
clean:
	rm UnixLs
