#include<http_parser.h>
#include<stdio.h>
#include<string.h>
#include<isc/string.h>
#include<stdlib.h>
#include<limits.h>
Headers http_parser(char *request) {
	char *token = NULL;
	char *saveptr = NULL;
	char *saveptr1 = NULL;
	char *token1;
	char *piece = NULL;
	Headers h;
	token = strtok_r(request, "\r\n", &saveptr);
	if(token) {
		token1 = malloc(strlen(token)+1);
		strlcpy(token1, token, strlen(token)+1);
		piece = strtok_r(token1, " ", &saveptr1);
		piece = strtok_r(NULL, " ", &saveptr1);
		if(strncmp(token, "GET", strlen("GET")) == 0) {
			h.type = GET;
		} else if(strncmp(token, "POST", strlen("POST")) == 0) {
			h.type = POST;
		} else if(strncmp(token, "HEAD", strlen("HEAD")) == 0) {
			h.type = HEAD;
		} else {
			h.type = -1;
		}
		h.path = malloc(strlen(piece)+1);
		strlcpy(h.path, piece, strlen(piece)+1);
		token = strtok_r(NULL, "\r\n", &saveptr);
	}
	while(token) {
		token = strtok_r(NULL, "\r\n", &saveptr);
	}
	return h;
}
