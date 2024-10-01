#include<stdio.h>
#include<assert.h>
#include "../../include/parser.h"


void test_parse_itype() {
    assert(parse_i_type("addi" ,"x1", "x2", "10") == 0x00A10093);
    // assert(parse_i_type("lw" "x3", "4(x4)") == 0x00422183);
    // assert(parse_i_type("jalr" "x1", "0(x2)") == 0x000100E7);
    printf("parse_itype tests passed!\n");
}

int main(){
    test_parse_itype();
    return 0;
}