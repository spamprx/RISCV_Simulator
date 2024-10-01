#ifndef PARSER_H
#define PARSER_H

#include "assembler.h"

typedef struct {
    uint32_t lui_instruction;
    uint32_t addi_instruction;
    int is_large_imm;
} LiInstructions;

// In parser.h, add these new function prototypes:

uint32_t parse_rw_type(char *inst, char *rd, char *rs1, char *rs2);
uint32_t parse_iw_type(char *inst, char *rd, char *rs1, char *imm);

uint32_t parse_r_type(char *inst, char *rd, char *rs1, char *rs2);
uint32_t parse_i_type(char *inst, char *rd, char *rs1, char *imm);
uint32_t parse_s_type(char *inst, char *rs2, char *imm_rs1);
uint32_t parse_b_type(char *inst, char *rs1, char *rs2, char *label, int pc, Label *labels, int label_count);
uint32_t parse_u_type(char *inst, char *rd, char *imm);
uint32_t parse_j_type(char *inst, char *rd, char *label, int pc, Label *labels, int label_count);
uint32_t parse_jalr(char *rd, char *offset_rs1);


LiInstructions parse_li(char *rd, char *imm);
uint32_t parse_mv(char *rd, char *rs);
uint32_t parse_j(char *label, int pc, Label *labels, int label_count);
uint32_t parse_nop();
uint32_t parse_not(char *rd, char *rs);
uint32_t parse_neg(char *rd, char *rs);
uint32_t parse_seqz(char *rd, char *rs);
uint32_t parse_snez(char *rd, char *rs);
uint32_t parse_sltz(char *rd, char *rs);
uint32_t parse_sgtz(char *rd, char *rs);
uint32_t parse_beqz(char *rs, char *label, int pc, Label *labels, int label_count);
uint32_t parse_bnez(char *rs, char *label, int pc, Label *labels, int label_count);
uint32_t parse_blez(char *rs, char *label, int pc, Label *labels, int label_count);
uint32_t parse_bgez(char *rs, char *label, int pc, Label *labels, int label_count);
uint32_t parse_bltz(char *rs, char *label, int pc, Label *labels, int label_count);
uint32_t parse_bgtz(char *rs, char *label, int pc, Label *labels, int label_count);
uint32_t parse_jr(char *rs);
uint32_t parse_ret();

#endif // PARSER_H