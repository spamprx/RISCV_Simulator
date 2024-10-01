#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/assembler.h"
#include "../include/parser.h"
#include "../include/utils.h"

#define MAX_DATA_SIZE 1024

typedef struct {
    uint32_t machine_code;
    char instruction[MAX_LINE_LENGTH];
} Instruction;

typedef struct {
    uint64_t data;
    int size; // 1 for byte, 2 for half, 4 for word, 8 for dword
} DataItem;

int main() {
    FILE *input_file, *output_file;
    char line[MAX_LINE_LENGTH];
    char instructions[MAX_LINES][MAX_LINE_LENGTH];
    Label labels[MAX_LABELS];
    Instruction encoded_instructions[MAX_LINES];
    DataItem data_section[MAX_DATA_SIZE];
    int line_count = 0;
    int label_count = 0;
    int encoded_count = 0;
    int data_count = 0;
    int pc = 0;
    int in_data_section = 0;
    int in_text_section = 0;

    // First pass: parse labels
    if (!parse_labels("input.s", labels, &label_count)) {
        printf("Error parsing labels\n");
        return 1;
    }

    // Second Pass: Read and process the input file
    input_file = fopen("input.s", "r");
    if (input_file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    pc = 0;
    int raw_line_count = 0;
    while (fgets(line, sizeof(line), input_file) && line_count < MAX_LINES) {
        raw_line_count++;
        line[strcspn(line, "\n")] = 0; // Remove newline character
        char *trimmed_line = trim(line);

        // Skip comments and empty lines
        if (trimmed_line[0] == ';' || trimmed_line[0] == '\0') {
            continue;
        }

        // Handle section directives
        if (strcmp(trimmed_line, ".data") == 0) {
            in_data_section = 1;
            in_text_section = 0;
            continue;
        } else if (strcmp(trimmed_line, ".text") == 0) {
            in_data_section = 0;
            in_text_section = 1;
            continue;
        }

        // Handle labels
        char *colon = strchr(trimmed_line, ':');
        if (colon != NULL) {
            trimmed_line = trim(colon + 1);
            if (trimmed_line[0] == '\0') {
                continue;
            }
        }

        // Remove inline comments
        char *comment = strchr(trimmed_line, ';');
        if (comment != NULL) {
            *comment = '\0';
            trimmed_line = trim(trimmed_line);
        }

        // Process data section
        if (in_data_section) {
            char directive[10];
            char value[256];
            if (sscanf(trimmed_line, "%9s %255[^\n]", directive, value) == 2) {
                if (strcmp(directive, ".byte") == 0) {
                    char *token = strtok(value, ",");
                    while (token != NULL && data_count < MAX_DATA_SIZE) {
                        data_section[data_count].data = (uint8_t)strtoul(token, NULL, 0);
                        data_section[data_count].size = 1;
                        data_count++;
                        token = strtok(NULL, ",");
                    }
                } else if (strcmp(directive, ".half") == 0) {
                    char *token = strtok(value, ",");
                    while (token != NULL && data_count < MAX_DATA_SIZE) {
                        data_section[data_count].data = (uint16_t)strtoul(token, NULL, 0);
                        data_section[data_count].size = 2;
                        data_count++;
                        token = strtok(NULL, ",");
                    }
                } else if (strcmp(directive, ".word") == 0) {
                    char *token = strtok(value, ",");
                    while (token != NULL && data_count < MAX_DATA_SIZE) {
                        data_section[data_count].data = (uint32_t)strtoul(token, NULL, 0);
                        data_section[data_count].size = 4;
                        data_count++;
                        token = strtok(NULL, ",");
                    }
                } else if (strcmp(directive, ".dword") == 0) {
                    char *token = strtok(value, ",");
                    while (token != NULL && data_count < MAX_DATA_SIZE) {
                        data_section[data_count].data = strtoull(token, NULL, 0);
                        data_section[data_count].size = 8;
                        data_count++;
                        token = strtok(NULL, ",");
                    }
                }
            }
        }

        // Process text section
        if (in_text_section && strlen(trimmed_line) > 0) {
            strcpy(instructions[line_count], trimmed_line);
            line_count++;
            pc += 4;
        }
    }

    fclose(input_file);

    // Process instructions
    pc = 0;
    for (int i = 0; i < line_count; i++)
    {
        char inst[20] = "", op1[20] = "", op2[20] = "", op3[20] = "";
        uint32_t instr_machine_code = 0;

        //printf("Processing line %d: '%s'\n", i + 1, instructions[i]);

        if (strlen(trim(instructions[i])) == 0) {
            printf("Skipping empty line %d\n", i + 1);
            continue;
        }

        int parsed = sscanf(instructions[i], "%19s %19[^,\n]%*[,] %19[^,\n]%*[,] %19s", inst, op1, op2, op3);

        if (parsed < 1)
        {
            printf("Error: Invalid instruction format at line %d: '%s'\n", i + 1, instructions[i]);
            return 1;
        }

        //printf("Parsed: inst='%s', op1='%s', op2='%s', op3='%s'\n", inst, op1, op2, op3);

        // Convert instruction to lowercase
        for (int j = 0; inst[j]; j++)
        {
            inst[j] = tolower(inst[j]);
        }

        // Process different instruction types
        if (strcmp(inst, "add") == 0 || strcmp(inst, "sub") == 0 || strcmp(inst, "sll") == 0 ||
            strcmp(inst, "slt") == 0 || strcmp(inst, "sltu") == 0 || strcmp(inst, "xor") == 0 ||
            strcmp(inst, "srl") == 0 || strcmp(inst, "sra") == 0 || strcmp(inst, "or") == 0 ||
            strcmp(inst, "and") == 0)
        {
            // R-type instruction processing
            int rd = get_register_number(op1);
            int rs1 = get_register_number(op2);
            int rs2 = get_register_number(op3);
            if (rd == -1 || rs1 == -1 || rs2 == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }   
            if(parsed < 4 ){
                printf("Error:Insufficent number of operands given to %s at line %d",inst,i+1);
            }
            else if(parsed > 4){
                printf("Error: Too many operands given to %s at line %d",inst,i+1);
            }
            instr_machine_code = parse_r_type(inst, op1, op2, op3);
        }
        else if (strcmp(inst, "addi") == 0 || strcmp(inst, "slti") == 0 || strcmp(inst, "sltiu") == 0 ||
                 strcmp(inst, "xori") == 0 || strcmp(inst, "ori") == 0 || strcmp(inst, "andi") == 0 ||
                 strcmp(inst, "slli") == 0 || strcmp(inst, "srli") == 0 || strcmp(inst, "srai") == 0)
        {
            // I-type instruction processing
            int rd = get_register_number(op1);
            int rs1 = get_register_number(op2);
            if (rd == -1 || rs1 == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            if(parsed < 4 ){
                printf("Error:Insufficent number of operands given to %s at line %d",inst,i+1);
            }
            else if(parsed > 4){
                printf("Error: Too many operands given to %s at line %d",inst,i+1);
            }
            instr_machine_code = parse_i_type(inst, op1, op2, op3);
        }
        else if (strcmp(inst, "lb") == 0 || strcmp(inst, "lh") == 0 || strcmp(inst, "lw") == 0 ||
                 strcmp(inst, "lbu") == 0 || strcmp(inst, "lhu") == 0 || strcmp(inst,"ld") == 0)
        {
            // I-type instruction processing
            int rd = get_register_number(op1);
            if (rd == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            if(parsed < 3 ){
                printf("Error:Insufficent number of operands given to %s at line %d",inst,i+1);
            }
            else if(parsed > 3){
                printf("Error: Too many operands given to %s at line %d",inst,i+1);
            }
            instr_machine_code = parse_i_type(inst, op1, op2, NULL);
        }
        else if (strcmp(inst, "sb") == 0 || strcmp(inst, "sh") == 0 || strcmp(inst, "sw") == 0 ||
                 strcmp(inst, "sd") == 0)
        {
            // S-type instruction processing
            int rs2 = get_register_number(op1);
            if (rs2 == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            if(parsed < 3 ){
                printf("Error:Insufficent number of operands given to %s at line %d",inst,i+1);
            }
            else if(parsed > 3){
                printf("Error: Too many operands given to %s at line %d",inst,i+1);
            }
            instr_machine_code = parse_s_type(inst, op1, op2);
        }
        else if (strcmp(inst, "beq") == 0 || strcmp(inst, "bne") == 0 || strcmp(inst, "blt") == 0 ||
                 strcmp(inst, "bge") == 0 || strcmp(inst, "bltu") == 0 || strcmp(inst, "bgeu") == 0)
        {
            // B-type instruction processing
            int rs1 = get_register_number(op1);
            int rs2 = get_register_number(op2);
            if (rs1 == -1 || rs2 == -1) 
            {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            if(parsed < 4 ){
                printf("Error:Insufficent number of operands given to %s at line %d",inst,i+1);
            }
            else if(parsed > 4){
                printf("Error: Too many operands given to %s at line %d",inst,i+1);
            }
            instr_machine_code = parse_b_type(inst, op1, op2, op3, pc, labels, label_count);
        }
        else if (strcmp(inst, "lui") == 0 || strcmp(inst, "auipc") == 0)
        {
            // U-type instruction processing
            int rd = get_register_number(op1);
            if (rd == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            if(parsed < 3 ){
                printf("Error:Insufficent number of operands given to %s at line %d",inst,i+1);
            }
            else if(parsed > 3){
                printf("Error: Too many operands given to %s at line %d",inst,i+1);
            }
            instr_machine_code = parse_u_type(inst, op1, op2);
        }
        else if (strcmp(inst, "jal") == 0)
        {
            // J-type instruction processing
            int rd = get_register_number(op1);
            if (rd == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            if(parsed < 3 ){
                printf("Error:Insufficent number of operands given to %s at line %d",inst,i+1);
            }
            else if(parsed > 3){
                printf("Error: Too many operands given to %s at line %d",inst,i+1);
            }
            instr_machine_code = parse_j_type(inst, op1, op2, pc, labels, label_count);
        }
        else if (strcmp(inst, "jalr") == 0)
        {
            // J-type instruction processing
            int rd = get_register_number(op1);
            if (rd == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            // if(parsed < 4 ){
            //     printf("Error:Insufficent number of operands given to %s at line %d",inst,i+1);
            // }
            // else if(parsed > 4){
            //     printf("Error: Too many operands given to %s at line %d",inst,i+1);
            // }
            instr_machine_code = parse_jalr(op1, op2);
        }
        else if (strcmp(inst, "addw") == 0 || strcmp(inst, "subw") == 0 ||
         strcmp(inst, "sllw") == 0 || strcmp(inst, "srlw") == 0 || strcmp(inst, "sraw") == 0)
        {
            // R-type instruction processing
        int rd = get_register_number(op1);
        int rs1 = get_register_number(op2);
        int rs2 = get_register_number(op3);
        if (rd == -1 || rs1 == -1 || rs2 == -1) {
            printf("Error: Invalid register in instruction at line %d\n", i + 1);
            continue;
        }
        instr_machine_code = parse_rw_type(inst, op1, op2, op3);
        }   
        else if (strcmp(inst, "addiw") == 0 || strcmp(inst, "slliw") == 0 ||
            strcmp(inst, "srliw") == 0 || strcmp(inst, "sraiw") == 0)
            {
                // I-type instruction processing
            int rd = get_register_number(op1);
            int rs1 = get_register_number(op2);
            if (rd == -1 || rs1 == 1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }

        instr_machine_code = parse_iw_type(inst, op1, op2, op3);
        }
            // else if (strcmp(inst, "li") == 0)
        // {
        //     int rd = get_register_number(op1);
        //     if (rd == -1) {
        //         printf("Error: Invalid register in instruction at line %d\n", i + 1);
        //         continue;
        //     }
        //     LiInstructions li_result = parse_li(op1, op2);
        //     if (li_result.is_large_imm)
        //     {
        //         encoded_instructions[encoded_count].machine_code = li_result.lui_instruction;
        //         strcpy(encoded_instructions[encoded_count].instruction, instructions[i]);
        //         encoded_count++;
        //         pc += 4;
        //         instr_machine_code = li_result.addi_instruction;
        //     }
        //     else
        //     {
        //         instr_machine_code = li_result.addi_instruction;
        //     }
        // }
        else if (strcmp(inst, "mv") == 0)
        {
            int rd = get_register_number(op1);
            int rs = get_register_number(op2);
            if (rd == -1 || rs == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            instr_machine_code = parse_mv(op1, op2);
        }
        else if (strcmp(inst, "j") == 0)
        {
            instr_machine_code = parse_j(op1, pc, labels, label_count);
        }
        else if (strcmp(inst, "nop") == 0)
        {
            instr_machine_code = parse_nop();
        }
        
        else if (strcmp(inst, "not") == 0 || strcmp(inst, "neg") == 0 || 
                 strcmp(inst, "seqz") == 0 || strcmp(inst, "snez") == 0 || 
                 strcmp(inst, "sltz") == 0 || strcmp(inst, "sgtz") == 0)
        {
            int rd = get_register_number(op1);
            int rs = get_register_number(op2);
            if (rd == -1 || rs == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            if (strcmp(inst, "not") == 0) instr_machine_code = parse_not(op1, op2);
            else if (strcmp(inst, "neg") == 0) instr_machine_code = parse_neg(op1, op2);
            else if (strcmp(inst, "seqz") == 0) instr_machine_code = parse_seqz(op1, op2);
            else if (strcmp(inst, "snez") == 0) instr_machine_code = parse_snez(op1, op2);
            else if (strcmp(inst, "sltz") == 0) instr_machine_code = parse_sltz(op1, op2);
            else if (strcmp(inst, "sgtz") == 0) instr_machine_code = parse_sgtz(op1, op2);
        }
        else if (strcmp(inst, "beqz") == 0 || strcmp(inst, "bnez") == 0 || 
                 strcmp(inst, "blez") == 0 || strcmp(inst, "bgez") == 0 || 
                 strcmp(inst, "bltz") == 0 || strcmp(inst, "bgtz") == 0)
        {
            int rs = get_register_number(op1);
            //printf("%s, %s\n",op1,op2);
            if (rs == -1) {
                printf("Error: Invalid register in instruction at line %d\n", i + 1);
                continue;
            }
            if (strcmp(inst, "beqz") == 0) instr_machine_code = parse_beqz(op1, op2, pc, labels, label_count);
            else if (strcmp(inst, "bnez") == 0) instr_machine_code = parse_bnez(op1, op2, pc, labels, label_count);
            else if (strcmp(inst, "blez") == 0) instr_machine_code = parse_blez(op1, op2, pc, labels, label_count);
            else if (strcmp(inst, "bgez") == 0) instr_machine_code = parse_bgez(op1, op2, pc, labels, label_count);
            else if (strcmp(inst, "bltz") == 0) instr_machine_code = parse_bltz(op1, op2, pc, labels, label_count);
            else if (strcmp(inst, "bgtz") == 0) instr_machine_code = parse_bgtz(op1, op2, pc, labels, label_count);
        }
        else{
            printf("Error: Invalid instruction '%s' at line %d\n", inst, i + 1);
            continue;
        }
       


        // Store the encoded instruction if valid
        if(instr_machine_code != 0){
            encoded_instructions[encoded_count].machine_code = instr_machine_code;
            strcpy(encoded_instructions[encoded_count].instruction, instructions[i]);
            pc += 4;
            encoded_count++;
        }
    }

    output_file = fopen("output.hex", "w");
    if (output_file == NULL)
    {
        printf("Error opening output file.\n");
        return 1;
    }

    //fprintf(output_file, "Machine Code | Assembly Instruction\n");
    //fprintf(output_file, "--------------------------------------\n");

    for (int i = 0; i < encoded_count; i++)
    {
        fprintf(output_file, "%08x\n", encoded_instructions[i].machine_code);
    }

    fclose(output_file);
    //printf("Conversion completed successfully.\n");
    return 0;
}