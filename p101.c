#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#define swap(a, b, type) do{type tmp; tmp = a; a = b; b = a;}while(0); 
#define aritm(a, cr, op) do{a = fasi(iasf(a) op iasf(cr));}while(0);

#define REG_I0 0
#define REG_I1 1
#define REG_M 2
#define REG_A 3
#define REG_R 4
#define REG_B 5
#define REG_b 6
#define REG_C 7
#define REG_c 8
#define REG_D 9
#define REG_d 10
#define REG_E 11
#define REG_e 12
#define REG_F 13
#define REG_f 14

enum OP_CODE {
    OP_PLUS,
    OP_MIN,
    OP_MUL,
    OP_DIV,
    OP_SQRT,
    OP_ABS,
    OP_TRANSFM,
    OP_TRANSIA,
    OP_EXCIA,
    OP_DECA,
    OP_CLEAR,
    OP_PRINT,
    OP_VSPACE,
    OP_STOP,
    OP_DREXC
};

static
uint32_t fasi(float f)
{
    union {
        float f;
        uint32_t u; 
    } fu = { .f = f };
    return fu.u;
}

static
float iasf(uint32_t i)
{
    union {
        float f;
        uint32_t u; 
    } fu = { .u = i };
    return fu.f;
}

struct P101 {
    uint32_t registers[15];
    uint32_t CR;
    int running;
};

static
void ins(struct P101* p, float n)
{
    uint32_t u = fasi(n);
    p->registers[REG_M] = u;
}

static
void dispatch(struct P101* p, enum OP_CODE op)
{
    switch (op) {
        case OP_PLUS: aritm(p->registers[REG_A], p->registers[p->CR], +); break;
        case OP_MIN: aritm(p->registers[REG_A], p->registers[p->CR], -); break;
        case OP_MUL: aritm(p->registers[REG_A], p->registers[p->CR], *); break;
        case OP_DIV: aritm(p->registers[REG_A], p->registers[p->CR], /); break;
        case OP_CLEAR: p->registers[p->CR] = 0; break;
        case OP_TRANSIA: p->registers[REG_A] = p->registers[p->CR]; break;
        case OP_TRANSFM: p->registers[p->CR] = p->registers[REG_M]; break;
        case OP_EXCIA: swap(p->registers[REG_A], p->registers[p->CR], float); break;
        case OP_PRINT: printf("PRINT: %f\n", iasf(p->registers[p->CR])); break;
        case OP_SQRT: p->registers[REG_A] = fasi(sqrt(iasf(p->registers[p->CR]))); break;
        case OP_ABS: p->registers[REG_A] = fasi(abs(iasf(p->registers[p->CR]))); break;
        case OP_VSPACE: printf("\n"); break;
        case OP_DECA: p->registers[REG_M] = fasi(iasf(p->registers[REG_A]) - iasf(fasi(abs(p->registers[REG_A])))); break;
    }
    p->CR = REG_M;
}

static
void init_p101(struct P101* p)
{
    p->CR = REG_M;
}

static
void test_plus()
{
    struct P101 p;
    init_p101(&p);
    ins(&p, 10);
    dispatch(&p, OP_TRANSIA);
    ins(&p, 10);
    dispatch(&p, OP_PLUS);
    assert(p.registers[REG_A] == fasi(20.0) && "Addition is broken");
}

static
void test_minus()
{
    struct P101 p;
    init_p101(&p);
    ins(&p, 10);
    dispatch(&p, OP_TRANSIA);
    ins(&p, 10);
    dispatch(&p, OP_MIN);
    assert(p.registers[REG_A] == 0 && "Subtraction is broken");
}

static
void test_mul()
{
    struct P101 p;
    init_p101(&p);
    ins(&p, 10);
    dispatch(&p, OP_TRANSIA);
    ins(&p, 10);
    dispatch(&p, OP_MUL);
    assert(p.registers[REG_A] == fasi(100.0) && "Multiplication is broken");
}

static
void test_div()
{
    struct P101 p;
    init_p101(&p);
    ins(&p, 10);
    dispatch(&p, OP_TRANSIA);
    ins(&p, 10);
    dispatch(&p, OP_DIV);
    assert(p.registers[REG_A] == fasi(1.0) && "Division is broken");
}

int main(void)
{
    test_plus();
    test_minus();
    test_mul();
    test_div();
    return 0;
}
