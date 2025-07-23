#include "writer.h"

void writer_arithmetic(FILE *asm_file, char *arithmetic);
void writer_push_pop(FILE *asm_file, CommandType type, char *segment,
										 int value);
