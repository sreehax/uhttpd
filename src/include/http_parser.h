#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#define GET 1
#define POST 2
#define HEAD 3
typedef struct http_headers {
	int type;
	char *path;
} Headers;
void http_parser(Headers h, char *input);
#endif
