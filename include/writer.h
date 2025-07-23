#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include "parser.h"

void writer_arithmetic(FILE *asm_file, char *arithmetic);
void writer_push_pop(FILE *asm_file, CommandType type, char *segment, int value);

#endif