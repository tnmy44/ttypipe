#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>

pid_t child_pid = 0;

void sigchld_handler(int signum){
	int status = 0;
	while(1){
		status=0;
		waitpid(child_pid, &status, 0);
		if(WIFEXITED(status))
			exit(WEXITSTATUS(status));
	}
}

int main(int argc, char ** argv){

	int masterfd = open("/dev/ptmx", O_RDWR);
	grantpt(masterfd);
	unlockpt(masterfd);

	int slavefd = open(ptsname(masterfd), O_RDWR);

	struct termios term_params;
	tcgetattr(slavefd, &term_params);
	cfmakeraw(&term_params);
	tcsetattr(slavefd, TCSANOW, &term_params);
		
	child_pid = fork();

	switch(child_pid){

		case 0:
			close(1);
			close(masterfd);
			dup2(slavefd,1);
			close(slavefd);
			char** nargv;
			nargv = argv + 1;
			execvp(nargv[0],nargv);
			exit(1);

		case -1:
			exit(1);
	}

	struct sigaction act;
	sigaction(SIGCHLD,NULL,&act);
	act.sa_handler = sigchld_handler;
	sigaction(SIGCHLD,&act,&act);

	char buffer [512] ;
	while(1){
		int count = read(masterfd,buffer,512);
		if(count <= 0) break;
		count = write(1, buffer, count);
		if(count==-1) break;
	}
	
	close(slavefd);
	close(masterfd);
}