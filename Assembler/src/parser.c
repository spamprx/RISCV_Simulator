#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/utils.h"

/**
 * @brief Parses a RV64I R-type word instruction for word operations.
 *
 * @param inst The name of the instruction.
 * @param rd The destination register.
 * @param rs1 The first source register.
 * @param rs2 The second source register.
 *
 * @return The 32-bit machine code representation of the instruction.
 */
uint32_t parse_rw_type(char *inst, char *rd, char *rs1, char *rs2)
{
    uint32_t opcode = 0x3B;  // Opcode for RV64I R-type word instructions
    uint32_t funct3 = 0;
    uint32_t funct7 = 0;

    if (strcmp(inst, "addw") == 0)
        funct3 = 0x0;
    else if (strcmp(inst, "subw") == 0)
    {
        funct3 = 0x0;
        funct7 = 0x20;
    }
    else if (strcmp(inst, "sllw") == 0)
        funct3 = 0x1;
    else if (strcmp(inst, "srlw") == 0)
        funct3 = 0x5;
    else if (strcmp(inst, "sraw") == 0)
    {
        funct3 = 0x5;
        funct7 = 0x20;
    }

    uint32_t rd_num = get_register_number(rd);
    uint32_t rs1_num = get_register_number(rs1);
    uint32_t rs2_num = get_register_number(rs2);

    return (funct7 << 25) | (rs2_num << 20) | (rs1_num << 15) | (funct3 << 12) | (rd_num << 7) | opcode;
}

/**
 * @brief   Parse RISC-V 64-bit I-type word instructions and return the corresponding 32-bit binary representation.
 *
 * @param   inst    The instruction to parse (e.g., "addiw", "slliw", "srliw", "sraiw").
 * @param   rd      The destination register (e.g., "x0", "x1", ..., "x31").
 * @param   rs1     The source register (e.g., "x0", "x1", ..., "x31").
 * @param   imm     The immediate value (as a string, e.g., "10", "-5", "0x1F").
 *
 * @return  The 32-bit binary representation of the instruction.
 *
 * @example
 *      uint32_t binary = parse_iw_type("addiw", "x0", "x1", "10");
 *      printf("Binary representation: 0x%08x\n", binary);
 */
uint32_t parse_iw_type(char *inst, char *rd, char *rs1, char *imm)
{
    uint32_t opcode = 0x1B;  // Opcode for RV64I I-type word instructions
    uint32_t funct3 = 0;

    if (strcmp(inst, "addiw") == 0)
        funct3 = 0x0;
    else if (strcmp(inst, "slliw") == 0)
        funct3 = 0x1;
    else if (strcmp(inst, "srliw") == 0 || strcmp(inst, "sraiw") == 0)
        funct3 = 0x5;

    uint32_t rd_num = get_register_number(rd);
    uint32_t rs1_num = get_register_number(rs1);
    int64_t imm_val = strtoll(imm, NULL, 0);  // Use 64-bit integer

    if (strcmp(inst, "slliw") == 0 || strcmp(inst, "srliw") == 0 || strcmp(inst, "sraiw") == 0)
    {
        uint32_t shamt = imm_val & 0x1F;  // Only 5 bits for word shifts
        uint32_t funct7 = (strcmp(inst, "sraiw") == 0) ? 0x20 : 0x00;
        return (funct7 << 25) | (shamt << 20) | (rs1_num << 15) | (funct3 << 12) | (rd_num << 7) | opcode;
    }
    else
    {
        return ((imm_val & 0xFFF) << 20) | (rs1_num << 15) | (funct3 << 12) | (rd_num << 7) | opcode;
    }
}


/**
 * @brief Parses and encodes an R-type instruction.
 *
 * @param inst The R-type instruction name.
 * @param rd The destination register.
 * @param rs1 The first source register.
 * @param rs2 The second source register.
 *
 * @return The encoded 32-bit R-type instruction.
 */
uint32_t parse_r_type(char *inst, char *rd, char *rs1, char *rs2)
{
    uint32_t opcode = 0x33;
    uint32_t funct3 = 0;
    uint32_t funct7 = 0;

    if (strcmp(inst, "add") == 0)
        funct3 = 0x0;
    else if (strcmp(inst, "sub") == 0)
    {
        funct3 = 0x0;
        funct7 = 0x20;
    }
    else if (strcmp(inst, "sll") == 0)
        funct3 = 0x1;
    else if (strcmp(inst, "slt") == 0)
        funct3 = 0x2;
    else if (strcmp(inst, "sltu") == 0)
        funct3 = 0x3;
    else if (strcmp(inst, "xor") == 0)
        funct3 = 0x4;
    else if (strcmp(inst, "srl") == 0)
        funct3 = 0x5;
    else if (strcmp(inst, "sra") == 0)
    {
        funct3 = 0x5;
        funct7 = 0x20;
    }
    else if (strcmp(inst, "or") == 0)
        funct3 = 0x6;
    else if (strcmp(inst, "and") == 0)
        funct3 = 0x7;

    uint32_t rd_num = get_register_number(rd);
    uint32_t rs1_num = get_register_number(rs1);
    uint32_t rs2_num = get_register_number(rs2);

    return (funct7 << 25) | (rs2_num << 20) | (rs1_num << 15) | (funct3 << 12) | (rd_num << 7) | opcode;
}



/**
 * Parses an I-type instruction.
 *
 * @param inst The instruction name.
 * @param rd The destination register.
 * @param rs1 The source register 1.
 * @param imm The immediate value.
 *
 * @return The 32-bit machine code representation of the I-type instruction.
 */
uint32_t parse_i_type(char *inst, char *rd, char *rs1, char *imm)
{
    uint32_t opcode = 0x13;
    uint32_t funct3 = 0;
    uint32_t funct7 = 0;
    char actual_rs1[10];
    char actual_imm[10];

    // Determine the funct3 and funct7 values based on the instruction name
    if (strcmp(inst, "addi") == 0)
        funct3 = 0x0;
    else if (strcmp(inst, "slti") == 0)
        funct3 = 0x2;
    else if (strcmp(inst, "sltiu") == 0)
        funct3 = 0x3;
    else if (strcmp(inst, "xori") == 0)
        funct3 = 0x4;
    else if (strcmp(inst, "ori") == 0)
        funct3 = 0x6;
    else if (strcmp(inst, "andi") == 0)
        funct3 = 0x7;
    else if (strcmp(inst, "slli") == 0)
    {
        funct3 = 0x1;
        funct7 = 0x00;
    }
    else if (strcmp(inst, "srli") == 0 || strcmp(inst, "srai") == 0)
    {
        funct3 = 0x5;
        funct7 = (strcmp(inst, "srai") == 0) ? 0x20 : 0x00;
    }
    
    // Handle load instructions
    else if (strcmp(inst, "lb") == 0)
    {
        opcode = 0x03;
        funct3 = 0x0;
    }
    else if (strcmp(inst, "lh") == 0)
    {
        opcode = 0x03;
        funct3 = 0x1;
    }
    else if (strcmp(inst, "lw") == 0)
    {
        opcode = 0x03;
        funct3 = 0x2;
    }
    else if(strcmp(inst,"ld") == 0){
        opcode = 0x03;
        funct3 = 0x3;
    }
    else if (strcmp(inst, "lbu") == 0)
    {
        opcode = 0x03;
        funct3 = 0x4;
    }
    else if (strcmp(inst, "lhu") == 0)
    {
        opcode = 0x03;
        funct3 = 0x5;
    }
    else if (strcmp(inst, "lwu") == 0)
    {
        opcode = 0x03;
        funct3 = 0x6;
    }

    // Handle offset-immediate instructions
    if (opcode == 0x03)
    {
        sscanf(rs1, "%[^(](%[^)])", actual_imm, actual_rs1);
    }
    else
    {
        strcpy(actual_rs1, rs1);
        strcpy(actual_imm, imm);
    }

    uint32_t rd_num = get_register_number(rd);
    uint32_t rs1_num = get_register_number(actual_rs1);
    int64_t imm_val = strtol(actual_imm, NULL, 0);

    // Check if the immediate value is within the valid range
    if (imm_val < -2048 || imm_val > 2047)
    {
        printf("Error: Immediate value %ld out of range (-2048 to 2047) for I-type instruction\n", imm_val);
        return 0;
    }

    // Generate the machine code for slli, srli, and srai instructions
    if (strcmp(inst, "slli") == 0 || strcmp(inst, "srli") == 0 || strcmp(inst, "srai") == 0)
    {
        imm_val &= 0x1F;
        return (funct7 << 25) | ((imm_val & 0x1F) << 20) | (rs1_num << 15) | (funct3 << 12) | (rd_num << 7) | opcode;
    }
    else
    {
        return ((imm_val & 0xFFF) << 20) | (rs1_num << 15) | (funct3 << 12) | (rd_num << 7) | opcode;
    }
}
    

/**
 * @brief Parses and generates the machine code for store instructions (S-type).
 *
 * @param inst The mnemonic of the store instruction.
 * @param rs2 The name of the second source register.
 * @param imm_rs1 The immediate value and the source register in the format "imm(rs1)".
 *
 * @return The generated machine code for the store instruction.
 */
uint32_t parse_s_type(char *inst, char *rs2, char *imm_rs1)
{
    uint32_t opcode = 0x23;
    uint32_t funct3 = 0;

    if (strcmp(inst, "sb") == 0)
        funct3 = 0x0;
    else if (strcmp(inst, "sh") == 0)
        funct3 = 0x1;
    else if (strcmp(inst, "sw") == 0)
        funct3 = 0x2;
    else if (strcmp(inst, "sd") == 0)
        funct3 = 0x3;

    char imm[10], rs1[10];
    sscanf(imm_rs1, "%[^(](%[^)])", imm, rs1);

    uint32_t rs1_num = get_register_number(rs1);
    uint32_t rs2_num = get_register_number(rs2);
    int64_t imm_val = strtol(imm, NULL, 0);

    uint32_t imm_11_5 = (imm_val & 0xFE0) >> 5;
    uint32_t imm_4_0 = imm_val & 0x1F;

    return (imm_11_5 << 25) | (rs2_num << 20) | (rs1_num << 15) | (funct3 << 12) | (imm_4_0 << 7) | opcode;
}

/**
 * @brief Parses and generates the machine code for branch instructions (B-type).
 *
 * @param inst The mnemonic of the branch instruction.
 * @param rs1 The name of the first source register.
 * @param rs2 The name of the second source register.
 * @param label The label to which the branch instruction will jump.
 * @param pc The program counter of the current instruction.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the branch instruction.
 */
uint32_t parse_b_type(char *inst, char *rs1, char *rs2, char *label, int pc, Label *labels, int label_count)
{
    uint32_t opcode = 0x63;
    uint32_t funct3 = 0;

    if (strcmp(inst, "beq") == 0)
        funct3 = 0x0;
    else if (strcmp(inst, "bne") == 0)
        funct3 = 0x1;
    else if (strcmp(inst, "blt") == 0)
        funct3 = 0x4;
    else if (strcmp(inst, "bge") == 0)
        funct3 = 0x5;
    else if (strcmp(inst, "bltu") == 0)
        funct3 = 0x6;
    else if (strcmp(inst, "bgeu") == 0)
        funct3 = 0x7;

    uint32_t rs1_num = get_register_number(rs1);
    uint32_t rs2_num = get_register_number(rs2);

    int label_addr = find_label(label, labels, label_count);
    if (label_addr == -1)
    {
        printf("Error: Label '%s' not found for instruction '%s'\n", label, inst);
        exit(1);
    }

    int32_t offset = label_addr - (pc);
    if (offset < -4096 || offset > 4095 || (offset & 1) != 0)
    {
        printf("Error: Branch offset %d out of range or not aligned for instruction '%s'\n", offset, inst);
        return 0;
    }

    //printf("%d\n",offset);
    offset/=2;


    uint32_t imm_12 = (offset >> 11) & 0x1;
    uint32_t imm_11 = (offset >> 10) & 0x1;
    uint32_t imm_10_5 = (offset >> 4) & 0x3F;
    uint32_t imm_4_1 = (offset >> 0) & 0xF;

    return (imm_12 << 31) | (imm_10_5 << 25) | (rs2_num << 20) | (rs1_num << 15) |
           (funct3 << 12) | (imm_4_1 << 8) | (imm_11 << 7) | opcode;
}

/**
 * @brief Parses and generates the machine code for U-type instructions (LUI and AUIPC).
 *
 * @param inst The mnemonic of the U-type instruction.
 * @param rd The name of the destination register.
 * @param imm The immediate value for the U-type instruction.
 *
 * @return The generated machine code for the U-type instruction.
 */
uint32_t parse_u_type(char *inst, char *rd, char *imm)
{
    uint32_t opcode = (strcmp(inst, "lui") == 0) ? 0x37 : 0x17;
    uint32_t rd_num = get_register_number(rd);

    uint32_t imm_val;
    if (imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X'))
    {
        imm_val = (uint32_t)strtoul(imm, NULL, 16);
    }
    else
    {
        imm_val = (uint32_t)strtoul(imm, NULL, 10);
    }

    if (imm_val > 0xFFFFF)
    {
        printf("Error: Immediate value 0x%x out of range (0x0 to 0xFFFFF) for U-type instruction '%s'\n", imm_val, inst);
        return 0;
    }

    return (imm_val << 12) | (rd_num << 7) | opcode;
}

/**
 * @brief Parses and generates the machine code for J-type instructions
 *
 * @param inst The mnemonic of the J-type instruction.
 * @param rd The name of the destination register.
 * @param label The label to which the jump instruction will jump.
 * @param pc The program counter of the current instruction.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the J-type instruction.
 */
uint32_t parse_j_type(char *inst, char *rd, char *label, int pc, Label *labels, int label_count)
{
    uint32_t opcode = 0x6F;
    uint32_t rd_num = get_register_number(rd);
    int label_addr = find_label(label, labels, label_count);

    int32_t offset = label_addr - pc;
    if (offset < -1048576 || offset > 1048575 || (offset & 1) != 0)
    {
        printf("Error: Jump offset %d out of range or not aligned for instruction '%s'\n", offset, inst);
        return 0;
    }

    uint32_t imm_20 = (offset >> 20) & 0x1;
    uint32_t imm_19_12 = (offset >> 12) & 0xFF;
    uint32_t imm_11 = (offset >> 11) & 0x1;
    uint32_t imm_10_1 = (offset >> 1) & 0x3FF;

    return (imm_20 << 31) | (imm_10_1 << 21) | (imm_11 << 20) | (imm_19_12 << 12) | (rd_num << 7) | opcode;
}

/**
 * @brief Parses and generates the machine code for JALR instruction.
 *
 * @param rd The name of the destination register.
 * @param offset_rs1 A string containing the offset and the source register in the format "offset(rs1)".
 *
 * @return The generated machine code for the JALR instruction.
 */
uint32_t parse_jalr(char *rd, char *offset_rs1)
{
    uint32_t opcode = 0x67;
    uint32_t funct3 = 0x0;
    uint32_t rd_num = get_register_number(rd);

    char offset[10], rs1[10];
    sscanf(offset_rs1, "%[^(](%[^)])", offset, rs1);

    uint32_t rs1_num = get_register_number(rs1);
    int32_t imm_val = strtol(offset, NULL, 0);

    //printf("%u",imm_val);

    return (imm_val & 0xFFF) | (rs1_num << 15) | (funct3 << 12) | (rd_num << 7) | opcode;
}

/**
 * @brief Parses and generates the machine code for a move instruction.
 *
 * This function implements the 'mv' pseudo-instruction by using an 'addi' instruction with an immediate value of 0.
 *
 * @param rd The name of the destination register.
 * @param rs The name of the source register.
 *
 * @return The generated machine code for the move instruction.
 */
uint32_t parse_mv(char *rd, char *rs)
{
    return parse_i_type("addi", rd, rs, "0");
}

/**
 * @brief Parses and generates the machine code for a J-type instruction (JAL).
 *
 * This function implements the 'j' pseudo-instruction by using a 'jal' instruction with x0 as the destination register.
 *
 * @param label The label to which the jump instruction will jump.
 * @param pc The program counter of the current instruction.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the J-type instruction (JAL).
 */
uint32_t parse_j(char *label, int pc, Label *labels, int label_count)
{
    return parse_j_type("jal", "x0", label, pc, labels, label_count);
}

/**
 * @brief Generates the machine code for a NOP (No Operation) instruction.
 *
 * This function implements the 'nop' pseudo-instruction by using an 'addi' instruction that adds 0 to x0 and stores the result in x0.
 *
 * @return The generated machine code for the NOP instruction.
 */
uint32_t parse_nop()
{
    return parse_i_type("addi", "x0", "x0", "0");
}

/**
 * @brief Parses and generates the machine code for a bitwise NOT operation.
 *
 * This function implements the 'not' pseudo-instruction by using an 'xori' instruction with -1 as the immediate value.
 *
 * @param rd The name of the destination register.
 * @param rs The name of the source register.
 *
 * @return The generated machine code for the NOT operation.
 */
uint32_t parse_not(char *rd, char *rs)
{
    return parse_i_type("xori", rd, rs, "-1");
}

/**
 * @brief Parses and generates the machine code for a two's complement negation.
 *
 * This function implements the 'neg' pseudo-instruction by subtracting the source register from x0.
 *
 * @param rd The name of the destination register.
 * @param rs The name of the source register.
 *
 * @return The generated machine code for the negation operation.
 */
uint32_t parse_neg(char *rd, char *rs)
{
    return parse_r_type("sub", rd, "x0", rs);
}

/**
 * @brief Parses and generates the machine code for the 'seqz' (Set if Equal to Zero) instruction.
 *
 * This function implements the 'seqz' pseudo-instruction by using an 'sltiu' instruction with 1 as the immediate value.
 *
 * @param rd The name of the destination register.
 * @param rs The name of the source register.
 *
 * @return The generated machine code for the 'seqz' instruction.
 */
uint32_t parse_seqz(char *rd, char *rs)
{
    return parse_i_type("sltiu", rd, rs, "1");
}

/**
 * @brief Parses and generates the machine code for the 'snez' (Set if Not Equal to Zero) instruction.
 *
 * This function implements the 'snez' pseudo-instruction by using an 'sltu' instruction with x0 as the first source register.
 *
 * @param rd The name of the destination register.
 * @param rs The name of the source register.
 *
 * @return The generated machine code for the 'snez' instruction.
 */
uint32_t parse_snez(char *rd, char *rs)
{
    return parse_r_type("sltu", rd, "x0", rs);
}

/**
 * @brief Parses and generates the machine code for the 'sltz' (Set if Less Than Zero) instruction.
 *
 * This function implements the 'sltz' pseudo-instruction by using an 'slt' instruction with x0 as the second source register.
 *
 * @param rd The name of the destination register.
 * @param rs The name of the source register.
 *
 * @return The generated machine code for the 'sltz' instruction.
 */
uint32_t parse_sltz(char *rd, char *rs)
{
    return parse_r_type("slt", rd, rs, "x0");
}

/**
 * @brief Parses and generates the machine code for the 'sgtz' (Set if Greater Than Zero) instruction.
 *
 * This function implements the 'sgtz' pseudo-instruction by using an 'slt' instruction with x0 as the first source register.
 *
 * @param rd The name of the destination register.
 * @param rs The name of the source register.
 *
 * @return The generated machine code for the 'sgtz' instruction.
 */
uint32_t parse_sgtz(char *rd, char *rs)
{
    return parse_r_type("slt", rd, "x0", rs);
}

/**
 * @brief Parses and generates the machine code for the 'beqz' (Branch if Equal to Zero) instruction.
 *
 * This function implements the 'beqz' pseudo-instruction by using a 'beq' instruction with x0 as the second source register.
 *
 * @param rs The name of the source register.
 * @param label The label to branch to.
 * @param pc The current program counter.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the 'beqz' instruction.
 */
uint32_t parse_beqz(char *rs, char *label, int pc, Label *labels, int label_count)
{
    return parse_b_type("beq", rs, "x0", label, pc, labels, label_count);
}

/**
 * @brief Parses and generates the machine code for the 'bnez' (Branch if Not Equal to Zero) instruction.
 *
 * This function implements the 'bnez' pseudo-instruction by using a 'bne' instruction with x0 as the second source register.
 *
 * @param rs The name of the source register.
 * @param label The label to branch to.
 * @param pc The current program counter.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the 'bnez' instruction.
 */
uint32_t parse_bnez(char *rs, char *label, int pc, Label *labels, int label_count)
{
    return parse_b_type("bne", rs, "x0", label, pc, labels, label_count);
}

/**
 * @brief Parses and generates the machine code for the 'blez' (Branch if Less Than or Equal to Zero) instruction.
 *
 * This function implements the 'blez' pseudo-instruction by using a 'bge' instruction with x0 as the first source register.
 *
 * @param rs The name of the source register.
 * @param label The label to branch to.
 * @param pc The current program counter.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the 'blez' instruction.
 */
uint32_t parse_blez(char *rs, char *label, int pc, Label *labels, int label_count)
{
    return parse_b_type("bge", "x0", rs, label, pc, labels, label_count);
}

/**
 * @brief Parses and generates the machine code for the 'bgez' (Branch if Greater Than or Equal to Zero) instruction.
 *
 * This function implements the 'bgez' pseudo-instruction by using a 'bge' instruction with x0 as the second source register.
 *
 * @param rs The name of the source register.
 * @param label The label to branch to.
 * @param pc The current program counter.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the 'bgez' instruction.
 */
uint32_t parse_bgez(char *rs, char *label, int pc, Label *labels, int label_count)
{
    return parse_b_type("bge", rs, "x0", label, pc, labels, label_count);
}

/**
 * @brief Parses and generates the machine code for the 'bltz' (Branch if Less Than Zero) instruction.
 *
 * This function implements the 'bltz' pseudo-instruction by using a 'blt' instruction with x0 as the second source register.
 *
 * @param rs The name of the source register.
 * @param label The label to branch to.
 * @param pc The current program counter.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the 'bltz' instruction.
 */
uint32_t parse_bltz(char *rs, char *label, int pc, Label *labels, int label_count)
{
    return parse_b_type("blt", rs, "x0", label, pc, labels, label_count);
}

/**
 * @brief Parses and generates the machine code for the 'bgtz' (Branch if Greater Than Zero) instruction.
 *
 * This function implements the 'bgtz' pseudo-instruction by using a 'blt' instruction with x0 as the first source register.
 *
 * @param rs The name of the source register.
 * @param label The label to branch to.
 * @param pc The current program counter.
 * @param labels An array of Label structures containing all the labels and their addresses.
 * @param label_count The number of labels in the labels array.
 *
 * @return The generated machine code for the 'bgtz' instruction.
 */
uint32_t parse_bgtz(char *rs, char *label, int pc, Label *labels, int label_count)
{
    return parse_b_type("blt", "x0", rs, label, pc, labels, label_count);
}



