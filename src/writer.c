#include "writer.h"

#include <stdio.h>
#include <string.h>

#include "parser.h"

int id;
char *static_name;

void writer_save_filename(char *filename) {
	static_name = filename;
	id = 0;
}

void writer_init(FILE *asm_file) {
	fprintf(asm_file, "@256\nD=A\n@SP\nM=D\n");
	writer_call(asm_file, "Sys.init", 0);
}

void writer_arithmetic(FILE *asm_file, char *arithmetic) {
	char output[256];
	if (strcmp(arithmetic, "add") == 0) {
		snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\nA=A-1\nM=D+M\n");
	} else if (strcmp(arithmetic, "sub") == 0) {
		snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n");
	} else if (strcmp(arithmetic, "neg") == 0) {
		snprintf(output, sizeof(output), "@SP\nA=M-1\nM=-M\n");
	} else if (strcmp(arithmetic, "eq") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nD=M-D\nM=-1\n@CONTINUE_%d\nD;"
						 "JEQ\n@SP\nA=M-1\nM=0\n(CONTINUE_%d)\n",
						 id, id);
		id++;
	} else if (strcmp(arithmetic, "gt") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nD=M-D\nM=-1\n@CONTINUE_%d\nD;"
						 "JGT\n@SP\nA=M-1\nM=0\n(CONTINUE_%d)\n",
						 id, id);
		id++;
	} else if (strcmp(arithmetic, "lt") == 0) {
		snprintf(output, sizeof(output),
						 "@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nD=M-D\nM=-1\n@CONTINUE_%d\nD;"
						 "JLT\n@SP\nA=M-1\nM=0\n(CONTINUE_%d)\n",
						 id, id);
		id++;
	} else if (strcmp(arithmetic, "and") == 0) {
		snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\nA=A-1\nM=D&M\n");
	} else if (strcmp(arithmetic, "or") == 0) {
		snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\nA=A-1\nM=D|M\n");
	} else {
		snprintf(output, sizeof(output), "@SP\nA=M-1\nM=!M\n");
	}

	fprintf(asm_file, "%s", output);
}

void writer_push_pop(FILE *asm_file, CommandType type, char *segment,
										 int value) {
	char output[256];
	if (type == C_PUSH) {
		if (strcmp(segment, "constant") == 0) {
			snprintf(output, sizeof(output), "@%d\nD=A\n@SP\nM=M+1\nA=M-1\nM=D\n",
							 value);
		} else if (strcmp(segment, "local") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@LCL\nA=D+M\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n", value);
		} else if (strcmp(segment, "argument") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@ARG\nA=D+M\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n", value);
		} else if (strcmp(segment, "this") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@THIS\nA=D+M\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n", value);
		} else if (strcmp(segment, "that") == 0) {
			snprintf(output, sizeof(output),
							 "@%d\nD=A\n@THAT\nA=D+M\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n", value);
		} else if (strcmp(segment, "static") == 0) {
			snprintf(output, sizeof(output), "@%s.%d\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n",
							 static_name, value);
		} else if (strcmp(segment, "temp") == 0) {
			snprintf(output, sizeof(output), "@R%d\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n",
							 5 + value);
		} else {
			snprintf(output, sizeof(output), "@%s\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n",
							 value == 0 ? "THIS" : "THAT");
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
			snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\n@R%d\nM=D\n",
							 5 + value);
		} else {
			snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\n@%s\nM=D\n",
							 value == 0 ? "THIS" : "THAT");
		}
	}

	fprintf(asm_file, "%s", output);
}

void writer_label(FILE *asm_file, char *segment) {
	char output[256];
	snprintf(output, sizeof(output), "(%s)\n", segment);

	fprintf(asm_file, "%s", output);
}

void writer_goto(FILE *asm_file, char *segment) {
	char output[256];
	snprintf(output, sizeof(output), "@%s\n0;JMP\n", segment);

	fprintf(asm_file, "%s", output);
}

void writer_if(FILE *asm_file, char *segment) {
	char output[256];
	snprintf(output, sizeof(output), "@SP\nAM=M-1\nD=M\n@%s\nD;JGT\n", segment);

	fprintf(asm_file, "%s", output);
}

void writer_function(FILE *asm_file, char *segment, int num_vars) {
	fprintf(asm_file, "(%s)", segment);
	for (int i = 0; i < num_vars; i++) {
		fprintf(asm_file, "@SP\nM=M+1\nA=M-1\nM=0\n");
	}
}

void writer_call(FILE *asm_file, char *segment, int num_args) {
	char output[256];
	snprintf(output, sizeof(output), "@%s_RETURN_ADDR\nD=A\n@SP\nM=M+1\nA=M-1\nM=D\n@LCL\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n@ARG\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n@THIS\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n@THAT\nD=M\n@SP\nM=M+1\nA=M-1\nM=D\n@SP\nD=M\n@LCL\nM=D\n@%d\nD=D-A\n@ARG\nM=D\n@%s\n0;JMP\n(%s_RETURN_ADDR)\n", segment, 5 + num_args, segment, segment);

	fprintf(asm_file, "%s", output);
}

void writer_return(FILE *asm_file) {
	char output[256];
	snprintf(output, sizeof(output), "@LCL\nD=M\n@5\nD=D-A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\nD=A\n@SP\nM=D+1\n@R13\nD=M\n@LCL\nMD=D+1\n@ARG\nMD=D+1\n@THIS\nMD=D+1\n@THAT\nMD=D+1\n@R13\nA=M\n0;JMP\n");

	fprintf(asm_file, "%s", output);
}
