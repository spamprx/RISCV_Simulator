#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "../include/utils.h"

int get_register_number(char *reg)
{
    if (reg == NULL || *reg == '\0')
    {
        //printf("Error: Empty or NULL register name\n");
        return -1;
    }

    if (reg[0] == 'x')
    {
        int num = atoi(reg + 1);
        if (num >= 0 && num <= 31)
        {
            return num;
        }
    }
    else if (strcmp(reg, "zero") == 0)
    {
        return 0;
    }
    else if (strcmp(reg, "ra") == 0)
    {
        return 1;
    }
    else if (strcmp(reg, "sp") == 0)
    {
        return 2;
    }
    else if (strcmp(reg, "gp") == 0)
    {
        return 3;
    }
    else if (strcmp(reg, "tp") == 0)
    {
        return 4;
    }
    else if (strcmp(reg, "fp") == 0 || strcmp(reg, "s0") == 0)
    {
        return 8;
    }
    else if (reg[0] == 't')
    {
        int num = atoi(reg + 1);
        if (num >= 0 && num <= 2)
            return num + 5;
        else if (num >= 3 && num <= 6)
            return num + 25;
    }
    else if (reg[0] == 'a')
    {
        int num = atoi(reg + 1);
        if (num >= 0 && num <= 7)
            return num + 10;
    }
    else if (reg[0] == 's')
    {
        int num = atoi(reg + 1);
        if (num == 0)
            return 8;
        else if(num == 1)
        return 9;
        else if (num >=1 && num <= 11)
            return num + 16;
    }

    printf("Error: Invalid register name '%s'\n", reg);
    return -1;
}

int find_label(char *label, Label *labels, int label_count)
{
    char trimmed_label[MAX_LABEL_LENGTH];
    strncpy(trimmed_label, label, MAX_LABEL_LENGTH - 1);
    trimmed_label[MAX_LABEL_LENGTH - 1] = '\0';
    trim(trimmed_label);

    for (int i = 0; i < label_count; i++)
    {
        char trimmed_stored_label[MAX_LABEL_LENGTH];
        strncpy(trimmed_stored_label, labels[i].name, MAX_LABEL_LENGTH - 1);
        trimmed_stored_label[MAX_LABEL_LENGTH - 1] = '\0';
        trim(trimmed_stored_label);

        if (strcasecmp(trimmed_label, trimmed_stored_label) == 0)
        {
            return labels[i].address;
        }
    }
    printf("Error: Label '%s' not found\n", label);
    return -1;
}

char *trim(char *str)
{
    char *end;
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return str;
}

int parse_labels(const char* filename, Label* labels, int* label_count)
{
    FILE* input_file;
    char line[MAX_LABEL_LENGTH];
    int pc = 0;
    int raw_line_count = 0;

    //printf("Attempting to open file: %s\n", filename);
    input_file = fopen(filename, "r");
    if (input_file == NULL)
    {
        printf("Error: Unable to open input file: %s\n", filename);
        return 0;
    }

    *label_count = 0;

    while (fgets(line, sizeof(line), input_file))
    {
        raw_line_count++;
        line[strcspn(line, "\n")] = 0;

        if (line[0] == ';' || line[0] == '\0')
        {
            continue;
        }

        char* comment = strchr(line, ';');
        if (comment != NULL)
        {
            *comment = '\0';
        }

        char* trimmed_line = trim(line);

        char* colon = strchr(trimmed_line, ':');
        if (colon != NULL)
        {
            *colon = '\0';
            char* label_name = trim(trimmed_line);
            
            if (*label_count < MAX_LABELS)
            {
                // Check for duplicate labels
                for (int i = 0; i < *label_count; i++)
                {
                    if (strcasecmp(labels[i].name, label_name) == 0)
                    {
                        printf("Error at line %d: Duplicate label '%s' (previously defined at address 0x%x)\n", 
                               raw_line_count, label_name, labels[i].address);
                        fclose(input_file);
                        return 0;
                    }
                }
                
                strncpy(labels[*label_count].name, label_name, MAX_LABEL_LENGTH - 1);
                labels[*label_count].name[MAX_LABEL_LENGTH - 1] = '\0';
                labels[*label_count].address = pc;
                (*label_count)++;
            }
            else
            {
                printf("Error at line %d: Maximum number of labels (%d) exceeded\n", raw_line_count, MAX_LABELS);
                fclose(input_file);
                return 0;
            }

            char* instruction = trim(colon + 1);
            if (strlen(instruction) > 0)
            {
                pc += 4;
            }
        }
        else if (strlen(trimmed_line) > 0)
        {
            pc += 4;
        }
    }

    if (ferror(input_file))
    {
        printf("Error: Failed to read the file\n");
        fclose(input_file);
        return 0;
    }

    fclose(input_file);
    return 1;
}