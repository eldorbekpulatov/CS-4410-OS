#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// https://piazza.com/class/jw5gjfzlolt5di?cid=27

void print(char *s){
	while (*s != 0) {
		write(1, s, 1);
		s++;
	}
}

void run(char *cmd){
	int pid = fork();
	if (pid == 0) {
		print("I am the child\n");
		execlp(cmd, cmd, NULL);
		print("child failed\n");
	}
	else {
		print("I am the parent\n");
		int status;
		wait(&status);
		print("child has terminated\n");
	}
}

int main(void){
	char line[8];

	print("hello world\n");
	while (1) {
		print("--> ");
		scanf("%s", line);
		run(line);
	}
	return 0;
}
