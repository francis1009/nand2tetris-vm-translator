#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "writer.h"

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("Usage: %s <vm_filename>\n", argv[0]);
		return 1;
	}

	const char *vm_filename = argv[1];
	if (strlen(vm_filename) > 255) {
	}
	char vm_fullpath[256];
	int result = snprintf(vm_fullpath, sizeof(vm_fullpath), "vm/%s", vm_filename);
	if (result < 0 || result > (int) sizeof(vm_fullpath)) {
		fprintf(stderr, "Error: vm filename is too long\n");
		return 1;
	}

	FILE *vm_file = fopen(vm_fullpath, "r");
	if (!vm_file) {
		fprintf(stderr, "Error: Unable to open file\n");
		return 1;
	}

	char asm_filename[256];
	snprintf(asm_filename, sizeof(asm_filename), "%.*s.asm",
					 (int) strlen(vm_filename) - 3, vm_filename);
	FILE *asm_file = fopen(asm_filename, "w");

	char *filename = asm_filename;
	filename[strlen(asm_filename) - 4] = '\0';
	writer_save_filename(filename);

	char line[256];
	ParsedLine parsed;
	char output[80];
	while (fgets(line, sizeof(line), vm_file)) {
		parser_trim_whitespace(line);
		if (line[0] == '/' || line[0] == '\n' || line[0] == '\0') {
			continue;
		}

		parsed = parser_parse(line);
		if (parsed.type == C_ARITHMETIC) {
			snprintf(output, sizeof(output), "// %s\n", parsed.segment);
			fprintf(asm_file, "%s", output);
			writer_arithmetic(asm_file, parsed.segment);
		} else if (parsed.type == C_PUSH || parsed.type == C_POP) {
			snprintf(output, sizeof(output), "// %s %s %d\n",
							 parsed.type == C_PUSH ? "push" : "pop", parsed.segment,
							 parsed.value);
			fprintf(asm_file, "%s", output);
			writer_push_pop(asm_file, parsed.type, parsed.segment, parsed.value);
		} else if (parsed.type == C_LABEL) {
			snprintf(output, sizeof(output), "// label %s\n", parsed.segment);
			fprintf(asm_file, "%s", output);
			writer_label(asm_file, parsed.segment);
		} else if (parsed.type == C_GOTO) {
			snprintf(output, sizeof(output), "// goto %s\n", parsed.segment);
			fprintf(asm_file, "%s", output);
			writer_goto(asm_file, parsed.segment);
		} else if (parsed.type == C_IF) {
			snprintf(output, sizeof(output), "// if-goto %s\n", parsed.segment);
			fprintf(asm_file, "%s", output);
			writer_if(asm_file, parsed.segment);
		}
	}

	fclose(vm_file);
	fclose(asm_file);
	return 0;
}
