//这个demo用于体会如何创建一个进程，以及创建进程的各项细节


#include<stdio.h>
#include<unistd.h>

int main(){
	printf("----create child process start----%d\n",getpid());
	pid_t pid;//定义这个变量知识开辟了一块空间
	pid = fork();//这个函数的返回值对于父子进程是不一样的：在父进程中，返回的是创建的子进程的pid，在子进程中返回的是0
	if (pid == 0){
		//这是子进程,返回值为0
		printf("this is child %d--rpid:%d\n",getpid(),pid);
	}
	else if(pid > 0){
		//这是父进程,因为父进程的返回值是子进程的pid>0
		printf("this is parent %d--rpid:%d\n",getpid(),pid);
	}
	else{
		//出错了
		printf("fork error\n");
	}
	printf("----create child process end----%d\n",getpid());
	return 0;
}
