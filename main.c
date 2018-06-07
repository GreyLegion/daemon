#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

static int running = 0;
static int delay = 1;
static char *app_name = NULL;
static char *conf_file_name = NULL;

int read_conf_file(int reload)
{
	FILE *conf_file = NULL;
	
	if(conf_file_name == NULL){ return 0; }
	
	/* //read config file
	conf_file = open(conf_file_name, "r");
	if(conf_file == NULL)
	{
		printf("Can't open config file %s\nError: %s\n", conf_file_name, strerror(errno));
		return -1;
	}
	*/
	
}

static void daemonize()
{
	int fd;
	pid_t pid = 0;
	
	// fork parent process
	pid = fork();
	
	// Error
	if(pid < 0){ exit(EXIT_FAILURE); }
	
	//Success. terminate parent
	if(pid > 0){ exit(EXIT_SUCCESS); }

	// child session leader
	if(setsid() < 0){ exit(EXIT_FAILURE); }
	
	//terminate parent
	signal(SIGCHLD, SIG_DFL);
	
	umask(0);
	
	if(chdir("/") != 0)
	{
		//write log: chdir("/") OK;
		exit(EXIT_FAILURE);
	}
	// close all file
	for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
	close(fd);
	}

}
void print_help(void)
{
	printf("\n Use %s [OPTIONS]\n", app_name);
	printf("  Options:\n");
	printf("	-h	--help		Print this help\n");
	printf("	-d	--daemon	daemonize \n");
}

void handle_signal(int sig)
{
	if(sig == SIGINT) {
		printf("Debug: stopping daemon ...\n");
		signal(SIGINT, SIG_DFL);
		running = 0;
	}else if(sig == SIGHUP){
		printf("Debug: reloading daemon config file ...\n");
	}else if(sig == SIGCHLD){
		printf("Debug: received SIGCHILD signal ...\n");
	}
}



int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"daemon", no_argument, 0, 'd'},
		{NULL, 0, 0, 0}
	};
	int value, option_index = 0;
	int start_daemonized = 0;
	
	app_name = argv[0];
	
	while((value = getopt_long(argc, argv, "hd", long_options, &option_index)) != -1)
	{
		switch(value)
		{
			case 'h':
			print_help();
			return EXIT_SUCCESS;
			
			case 'd':
			printf("daemonize test \n");
			start_daemonized = 1;
			running = 1;
			break;
			default: break;
		}
	}
	
	if(start_daemonized == 1){ daemonize(); }
	
	signal(SIGINT, handle_signal);
    signal(SIGHUP, handle_signal);
	
	// main daemon cycle
	while(running == 1)
	{ 
		sleep(1); 
	}
	
	
	return EXIT_SUCCESS;
}
