#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

#include "parser.h"
#include "writer.h"

bool is_directory(const char *input_path) {
	char *file = strchr(input_path, '.');
	if (file) {
		return false;
	}
	return true;
}

void process_vm_file(FILE *vm_file, FILE *asm_file) {
	char line[256];
	ParsedLine parsed;
	while (fgets(line, sizeof(line), vm_file)) {
		parser_trim_whitespace_comment(line);
		if (line[0] == '/' || line[0] == '\n' || line[0] == '\0') {
			continue;
		}
		fprintf(asm_file, "// %s\n", line);

		parsed = parser_parse(line);
		switch (parsed.type) {
		case C_ARITHMETIC:
			writer_arithmetic(asm_file, parsed.segment);
			break;
		case C_PUSH:
		case C_POP:
			writer_push_pop(asm_file, parsed.type, parsed.segment, parsed.value);
			break;
		case C_LABEL:
			writer_label(asm_file, parsed.segment);
			break;
		case C_GOTO:
			writer_goto(asm_file, parsed.segment);
			break;
		case C_IF:
			writer_if(asm_file, parsed.segment);
			break;
		case C_FUNCTION:
			writer_function(asm_file, parsed.segment, parsed.value);
			break;
		case C_CALL:
			writer_call(asm_file, parsed.segment, parsed.value);
			break;
		case C_RETURN:
			writer_return(asm_file);
		}
	}

	fclose(vm_file);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s <vm_filename_or_directory>\n", argv[0]);
		return 1;
	}

	const char *input_path = argv[1];
	char vm_fullpath[256];
	int result = snprintf(vm_fullpath, sizeof(vm_fullpath), "vm/%s", input_path);
	if (result < 0 || result > (int) sizeof(vm_fullpath)) {
		fprintf(stderr, "Error: vm filename or directory is too long\n");
		return 1;
	}

	int is_dir = is_directory(vm_fullpath);
	DIR *vm_dir = NULL;
	FILE *vm_file = NULL;
	char **vm_files = NULL;
	int file_count = 0;

	if (is_dir) {
		vm_dir = opendir(vm_fullpath);
		if (!vm_dir) {
			fprintf(stderr, "Error: Unable to open directory\n");
			return 1;
		}

		struct dirent *entry;
		while ((entry = readdir(vm_dir)) != NULL) {
			const char *name = entry->d_name;
			int len = strlen(name);
			if (len > 3 && strcmp(name + len - 3, ".vm") == 0) {
				vm_files = realloc(vm_files, (file_count + 1) * sizeof(char *));
				vm_files[file_count] = strdup(name);
				file_count++;
			}
		}
		closedir(vm_dir);

		if (file_count == 0) {
			fprintf(stderr, "Error: No .vm files found in directory '%s'\n",
							vm_fullpath);
			return 1;
		}
	} else {
		vm_file = fopen(vm_fullpath, "r");
		if (!vm_file) {
			fprintf(stderr, "Error: Unable to open file\n");
			return 1;
		}
	}

	char asm_filename[256];
	snprintf(asm_filename, sizeof(asm_filename), "%.*s.asm",
					 is_dir ? (int) strlen(input_path) : (int) strlen(input_path) - 3,
					 input_path);
	FILE *asm_file = fopen(asm_filename, "w");
	if (!asm_file) {
		fprintf(stderr, "Error: Unable to create output file '%s'\n", asm_filename);
		if (vm_file) {
			fclose(vm_file);
		}
		if (vm_files) {
			for (int i = 0; i < file_count; i++) {
				free(vm_files[i]);
			}
			free(vm_files);
		}
		return 1;
	}

	if (is_dir) {
		writer_init(asm_file);

		for (int i = 0; i < file_count; i++) {
			char filepath[512];
			snprintf(filepath, sizeof(filepath), "%s/%s", vm_fullpath, vm_files[i]);
			FILE *file = fopen(filepath, "r");
			if (!file) {
				fprintf(stderr, "Warning: Could not open '%s', skipping\n", filepath);
				continue;
			}

			char static_name[256];
			strncpy(static_name, vm_files[i], sizeof(static_name));
			fprintf(asm_file, "\n/* FILE %s */\n", static_name);
			static_name[strlen(vm_files[i]) - 3] = '\0';
			writer_save_filename(static_name);

			process_vm_file(file, asm_file);
			free(vm_files[i]);
		}

		free(vm_files);
	} else {
		char *filename = asm_filename;
		filename[strlen(asm_filename) - 4] = '\0';
		writer_save_filename(filename);
		process_vm_file(vm_file, asm_file);
	}

	fclose(asm_file);
	return 0;
}
