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
					 (int) (strlen(vm_filename) - 3), vm_filename);
	FILE *asm_file = fopen(asm_filename, "w");

	char line[256];
	ParsedLine parsed;
	char output[80];
	while (fgets(line, sizeof(line), vm_file)) {
		snprintf(output, sizeof(output), "%s", line);
		fprintf(asm_file, "%s", output);
	}

	fclose(vm_file);
	fclose(asm_file);
	return 0;
}
