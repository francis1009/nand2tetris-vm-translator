#include "writer.h"

#include <stdio.h>
#include <string.h>

#include "parser.h"

int id = 0;

void writer_arithmetic(FILE *asm_file, char *arithmetic) {
	char output[256];
	output[0] = '\0';
	if (strcmp(arithmetic, "add") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=D+M\n@SP\nM=M+1\n");
	} else if (strcmp(arithmetic, "sub") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1\n");
	} else if (strcmp(arithmetic, "neg") == 0) {
		snprintf(output, sizeof(output), "@SP\nM=M-1\nA=M\nM=-M\n@SP\nM=M+1\n");
	} else if (strcmp(arithmetic, "eq") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@TRUE_%d\nD;JEQ\nD="
						 "0\n@SP\nA=M\nM=D\n@CONTINUE_%d\n0;JMP\n(TRUE_%d)\nD=-1\n@SP\nA="
						 "M\nM=D\n(CONTINUE_%d)\n@SP\nM=M+1\n",
						 id, id, id, id);
		id++;
	} else if (strcmp(arithmetic, "gt") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@TRUE_%d\nD;JGT\nD="
						 "0\n@SP\nA=M\nM=D\n@CONTINUE_%d\n0;JMP\n(TRUE_%d)\nD=-1\n@SP\nA="
						 "M\nM=D\n(CONTINUE_%d)\n@SP\nM=M+1\n",
						 id, id, id, id);
		id++;
	} else if (strcmp(arithmetic, "lt") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@TRUE_%d\nD;JLT\nD="
						 "0\n@SP\nA=M\nM=D\n@CONTINUE_%d\n0;JMP\n(TRUE_%d)\nD=-1\n@SP\nA="
						 "M\nM=D\n(CONTINUE_%d)\n@SP\nM=M+1\n",
						 id, id, id, id);
		id++;
	} else if (strcmp(arithmetic, "and") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=D&M\n@SP\nM=M+1\n");
	} else if (strcmp(arithmetic, "or") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=D|M\n@SP\nM=M+1\n");
	} else {
		snprintf(output, sizeof(output), "@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n");
	}

	fprintf(asm_file, "%s", output);
}

void writer_push_pop(FILE *asm_file, CommandType type, char *segment,
										 int value) {
	char output[256];
	output[0] = '\0';
	if (type == C_PUSH) {
		if (strcmp(segment, "constant") == 0) {
			snprintf(output, sizeof(output), "@%d\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
							 value);
		}
	} else {
	}

	fprintf(asm_file, "%s", output);
}
