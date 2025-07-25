#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parser_trim_whitespace_comment(char *line) {
	if (!line || !*line) {
		return;
	}

	line[strcspn(line, "\r\n")] = '\0';

	char *comment = strchr(line, '/');
	if (comment) {
		*comment = '\0';
	}

	char *start = line;
	while (isspace((unsigned char) *start)) {
		start++;
	}

	if (start != line) {
		memmove(line, start, strlen(line));
	}
}

ParsedLine parser_parse(char *line) {
	ParsedLine parsed;

	char *token = strtok(line, " \n");
	if (strcmp(token, "push") == 0) {
		parsed.type = C_PUSH;
	} else if (strcmp(token, "pop") == 0) {
		parsed.type = C_POP;
	} else if (strcmp(token, "label") == 0) {
		parsed.type = C_LABEL;
	} else if (strcmp(token, "if-goto") == 0) {
		parsed.type = C_IF;
	} else if (strcmp(token, "goto") == 0) {
		parsed.type = C_GOTO;
	} else if (strcmp(token, "function") == 0) {
		parsed.type = C_FUNCTION;
	} else if (strcmp(token, "call") == 0) {
		parsed.type = C_CALL;
	} else if (strcmp(token, "return") == 0) {
		parsed.type = C_RETURN;
	} else {
		parsed.type = C_ARITHMETIC;
		snprintf(parsed.segment, sizeof(parsed.segment), "%s", token);
		parsed.value = 0;
		return parsed;
	}

	token = strtok(NULL, " \n");
	if (token != NULL) {
		snprintf(parsed.segment, sizeof(parsed.segment), "%s", token);
	}

	token = strtok(NULL, " \n");
	if (token != NULL) {
		parsed.value = atoi(token);
	}

	return parsed;
}
