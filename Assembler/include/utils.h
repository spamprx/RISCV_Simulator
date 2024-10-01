#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include"assembler.h"

int get_register_number(char *reg);
int find_label(char *label, Label *labels, int label_count);
char *trim(char *str);
int parse_labels(const char* filename, Label* labels, int* label_count);

#endif // UTILS_H