#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include "parser.h"

void writer_save_filename(char *filename);
void writer_init(FILE *asm_file);
void writer_arithmetic(FILE *asm_file, char *arithmetic);
void writer_push_pop(FILE *asm_file, CommandType type, char *segment, int value);
void writer_label(FILE *asm_file, char *segment);
void writer_goto(FILE *asm_file, char *segment);
void writer_if(FILE *asm_file, char *segment);
void writer_function(FILE *asm_file, char *segment, int num_vars);
void writer_call(FILE *asm_file, char *segment, int num_args);
void writer_return(FILE *asm_file);

#endif
