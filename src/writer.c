#include "writer.h"

#include <stdio.h>
#include <string.h>

#include "parser.h"

int id = 0;
char *static_name;

void writer_save_filename(char *filename) {
	static_name = filename;
}

void writer_arithmetic(FILE *asm_file, char *arithmetic) {
	char output[256];
	output[0] = '\0';
	if (strcmp(arithmetic, "add") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=D+M\n@SP\nM=M+1\n");
	} else if (strcmp(arithmetic, "sub") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M-D\n@SP\nM=M+1\n");
	} else if (strcmp(arithmetic, "neg") == 0) {
		snprintf(output, sizeof(output), "@SP\nAM=M-1\nM=-M\n@SP\nM=M+1\n");
	} else if (strcmp(arithmetic, "eq") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@TRUE_%d\nD;JEQ\nD="
						 "0\n@SP\nA=M\nM=D\n@CONTINUE_%d\n0;JMP\n(TRUE_%d)\nD=-1\n@SP\nA="
						 "M\nM=D\n(CONTINUE_%d)\n@SP\nM=M+1\n",
						 id, id, id, id);
		id++;
	} else if (strcmp(arithmetic, "gt") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@TRUE_%d\nD;JGT\nD="
						 "0\n@SP\nA=M\nM=D\n@CONTINUE_%d\n0;JMP\n(TRUE_%d)\nD=-1\n@SP\nA="
						 "M\nM=D\n(CONTINUE_%d)\n@SP\nM=M+1\n",
						 id, id, id, id);
		id++;
	} else if (strcmp(arithmetic, "lt") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@TRUE_%d\nD;JLT\nD="
						 "0\n@SP\nA=M\nM=D\n@CONTINUE_%d\n0;JMP\n(TRUE_%d)\nD=-1\n@SP\nA="
						 "M\nM=D\n(CONTINUE_%d)\n@SP\nM=M+1\n",
						 id, id, id, id);
		id++;
	} else if (strcmp(arithmetic, "and") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=D&M\n@SP\nM=M+1\n");
	} else if (strcmp(arithmetic, "or") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=D|M\n@SP\nM=M+1\n");
	} else {
		snprintf(output, sizeof(output), "@SP\nAM=M-1\nM=!M\n@SP\nM=M+1\n");
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
		} else if (strcmp(segment, "local") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@LCL\nA=D+M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
							 value);
		} else if (strcmp(segment, "argument") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@ARG\nA=D+M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
							 value);
		} else if (strcmp(segment, "this") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@THIS\nA=D+M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
							 value);
		} else if (strcmp(segment, "that") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@THAT\nA=D+M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
							 value);
		} else if (strcmp(segment, "static") == 0) {
			snprintf(output, sizeof(output),
							 "@%s.%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", static_name, value);
		} else if (strcmp(segment, "temp") == 0) {
			snprintf(output, sizeof(output),
							 "@5\nD=A\n@%d\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", value);
		} else {
			if (value == 0) {
				snprintf(output, sizeof(output),
								 "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
			} else {
				snprintf(output, sizeof(output),
								 "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
			}
		}
	} else {
		if (strcmp(segment, "local") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@LCL\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@"
							 "R13\nA=M\nM=D\n",
							 value);
		} else if (strcmp(segment, "argument") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@ARG\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@"
							 "R13\nA=M\nM=D\n",
							 value);
		} else if (strcmp(segment, "this") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@THIS\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@"
							 "R13\nA=M\nM=D\n",
							 value);
		} else if (strcmp(segment, "that") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@THAT\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@"
							 "R13\nA=M\nM=D\n",
							 value);
		} else if (strcmp(segment, "static") == 0) {
			snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\n@%s.%d\nM=D\n",
							 static_name, value);
		} else if (strcmp(segment, "temp") == 0) {
			snprintf(output, sizeof(output),
							 "@5\nD=A\n@%d\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@"
							 "R13\nA=M\nM=D\n",
							 value);
		} else {
			if (value == 0) {
				snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\n@THIS\nM=D\n");
			} else {
				snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\n@THAT\nM=D\n");
			}
		}
	}

	fprintf(asm_file, "%s", output);
}
