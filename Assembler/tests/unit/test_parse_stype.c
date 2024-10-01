#include<stdio.h>
#include<assert.h>
#include "../../include/parser.h"


void test_parse_stype() {
    assert(parse_s_type("sw" ,"x1", "8(x2)") == 0x00112423);
    assert(parse_s_type("sb", "x3", "-4(x4)") == 0xFE320FA3);
    printf("parse_stype tests passed!\n");
}

int main(){
    test_parse_stype();
    return 0;
}