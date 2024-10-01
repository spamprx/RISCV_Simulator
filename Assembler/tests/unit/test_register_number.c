// File: tests/unit/test_register_number.c
#include<stdio.h>
#include <assert.h>
#include "../../include/utils.h"

void test_register_number() {
    assert(get_register_number("x0") == 0);
    assert(get_register_number("x1") == 1);
    assert(get_register_number("x31") == 31);
    assert(get_register_number("zero") == 0);
    assert(get_register_number("ra") == 1);
    assert(get_register_number("sp") == 2);
    assert(get_register_number("t0") == 5);
    assert(get_register_number("invalid") == -1);
    printf("All register_number tests passed!\n");
}

int main() {
    test_register_number();
    return 0;
}