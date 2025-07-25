#ifndef PARSER_H
#define PARSER_H

typedef enum {
	C_ARITHMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
} CommandType;

typedef struct {
	CommandType type;
	char segment[80];
	int value;
} ParsedLine;

void parser_trim_whitespace_comment(char *line);
ParsedLine parser_parse(char *line);

#endif
