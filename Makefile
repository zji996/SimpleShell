#Write the Makefile for this directory. You should make sure the dependencies are in the right order.
#Here is a link for reference: https://opensource.com/article/18/8/what-how-makefile

Demo:
	gcc -Wall ./src/shell.c -lreadline -o ./tests/shell
	./tests/shell
