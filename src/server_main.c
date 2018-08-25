#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<http_parser.h>
void *process(int sock);
void writelog(char *message) {
	printf("[uhttpd]: %s\n", message);
	return;
}
int main(int argc, char *argv[]) {
	int server, client, clilen, pid;
	struct sockaddr_in addr, cli_addr;
	int opt = 1;
	if((server = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		writelog("socket failed");
		exit(EXIT_FAILURE);
	} else {
		writelog("socket created");
	}

	if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		writelog("setsockopt failed");
		exit(EXIT_FAILURE);
	} else {
		writelog("setsockopt set");
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(80);
	if(bind(server, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		writelog("bind failed");
		exit(EXIT_FAILURE);
	} else {
		writelog("bind successful");
	}

	if(listen(server, 100) < 0) {
		writelog("listen failed");
	} else {
		writelog("listen successful");
	}
	clilen = sizeof(cli_addr);
	while(1) {
		client = accept(server, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen);
		printf("[uhttpd]: new connection from ip %s\n", inet_ntoa(cli_addr.sin_addr));
		if(client < 0) {
			writelog("accept failed");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if(pid < 0) {
			writelog("fork failed");
			exit(EXIT_FAILURE);
		} else if(pid == 0) {
			close(server);
			process(client);
			return 0;
		} else {
			close(client);
		}
	}
}
void *process(int sock) {
	int n;
	int bytes_available;
	ioctl(sock, FIONREAD, &bytes_available);
	if(bytes_available > 90*1024*1024) { // if receive buffer > 90MiB
		bytes_available = 90*1024*1024; //cap at 90 MiB
	}
	char *buffer = malloc(bytes_available);
	n = read(sock, buffer, bytes_available);
	if(n < 0) {
		writelog("reading from socket failed");
	}
	Headers rq = http_parser(buffer);
	char *type;
	if(rq.type == GET) {
		type = "GET";
	} else if(rq.type == POST) {
		type = "POST";
	} else if(rq.type == HEAD) {
		type = "HEAD";
	} else {
		type = "UNKNOWN";
	}
	printf("[uhttpd]: Type: %s\n", type);
	printf("[uhttpd]: Path: %s\n", rq.path);
	const char *message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello, World</h1>";
	n = write(sock, message, strlen(message)+1);
	if(n < 0) {
		writelog("writing from socket failed");
	}
	close(sock);
}
