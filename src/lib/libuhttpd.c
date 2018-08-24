#include<http_parser.h>
#include<stdio.h>
#include<string.h>
#include<isc/string.h>
#include<stdlib.h>
void http_parser(Headers h, char *input) {
	char *token = NULL;
	char *request = malloc(strlen(input)+1);
	bzero(request, strlen(input)+1);
	strlcpy(request, input, strlen(input)+1);
	token = strtok(request, "\r\n");
	while(token) {
		printf("Current token: %s\n", token);
		token = strtok(NULL, "\r\n");
	}
	return;
}
