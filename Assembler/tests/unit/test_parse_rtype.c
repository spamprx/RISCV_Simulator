// File: tests/unit/test_parse_rtype.c
#include<stdio.h>
#include <assert.h>
#include "../../include/parser.h"

void test_parse_rtype() {
    char *instruction = "add x1, x2, x3";
    uint32_t expected = 0x003100b3;  // Opcode for add x1, x2, x3
    uint32_t result = parse_r_type("add","x1","x2","x3");
    assert(result == expected);
    printf("parse_rtype test passed!\n");
}

int main() {
    test_parse_rtype();
    return 0;
}