#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>

#define MAX_LINES 60
#define MAX_LINE_LENGTH 50
#define MAX_LABEL_LENGTH 50
#define MAX_LABELS 50


typedef struct
{
    char name[MAX_LINE_LENGTH];
    int address;
} Label;

#endif // ASSEMBLER_H