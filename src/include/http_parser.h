#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_
#define GET 1
#define POST 2
#define HEAD 3
typedef struct http_headers {
	int type;
	char *path;
	char *abspath;
} Headers;
Headers http_parser(char *input);
#endif
