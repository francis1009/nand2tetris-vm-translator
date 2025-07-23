#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ParsedLine parser_parse(char *line) {
	ParsedLine parsed;
	parsed.type = C_RETURN;
	parsed.segment[0] = '\0';
	parsed.value = 0;

	char *token = strtok(line, " \n");
	if (strcmp(token, "push") == 0) {
		parsed.type = C_PUSH;
	} else if (strcmp(token, "pop") == 0) {
		parsed.type = C_POP;
	} else {
		parsed.type = C_ARITHMETIC;
		snprintf(parsed.segment, sizeof(parsed.segment), "%s", token);
		return parsed;
	}

	token = strtok(NULL, " \n");
	snprintf(parsed.segment, sizeof(parsed.segment), "%s", token);

	token = strtok(NULL, " \n");
	parsed.value = atoi(token);

	return parsed;
}
