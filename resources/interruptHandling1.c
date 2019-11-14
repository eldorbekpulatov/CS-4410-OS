#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// https://piazza.com/class/jw5gjfzlolt5di?cid=26

void print(char *s){
	while (*s != 0) {
		write(1, s, 1);
		s++;
	}
}

void interruptHandler(int sig){
	print("got interrupt\n");
}

int main(void){
	int x = 3;

	print("hello world\n");
	//signal(SIGINT, interruptHandler);
	//signal(SIGFPE, interruptHandler);
	//signal(15, interruptHandler);
	while (1)
		; // x = x / 0;
}
