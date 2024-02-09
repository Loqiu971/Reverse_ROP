/* Corp.net, inc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_vm.h"

void    call(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC CALL\n");
    s->sp -= 1;
    s->stack[s->sp] = s->ip;
    int rel_adr = atoi(s->operand);
    s->ip += rel_adr;
}

void    ret(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC RET\n");
    s->ip = s->stack[s->sp];
    s->sp += 1;
}

void    push_a(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC PUSH_A\n");
    s->sp -= 1;
    s->stack[s->sp] = s->a;
}

void    pop_a(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC POP_A\n");
    s->a = s->stack[s->sp];
    s->sp += 1;
}

void    push_b(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC PUSH_B\n");
    s->sp -= 1;
    s->stack[s->sp] = s->b;
}

void    pop_b(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC POP_B\n");
    s->b = s->stack[s->sp];
    s->sp += 1;
}

void    push_c(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC PUSH_C\n");
    s->sp -= 1;
    s->stack[s->sp] = s->c;
}

void    pop_c(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC POP_C\n");
    s->c = s->stack[s->sp];
    s->sp += 1;
}

void    push_si(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC PUSH_SI\n");
    s->sp -= 1;
    s->stack[s->sp] = s->si;
}

void    pop_si(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC POP_SI\n");
    s->si = s->stack[s->sp];
    s->sp += 1;
}

void    push_di(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC PUSH_DI\n");
    s->sp -= 1;
    s->stack[s->sp] = s->di;
}

void    pop_di(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC POP_DI\n");
    s->di = s->stack[s->sp];
    s->sp += 1;
}

void    dump(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC DUMP\n");
    printf("A:%d B:%d C:%d SI:%d DI:%d IP:%d\n", s->a, s->b, s->c, s->si, s->di, s->ip);
}

void    prints(struct cmd_state_t *s)
{
#define SZ 256
    void    *p = &s;
    char    buf[SZ];
    fprintf(stderr, "EXEC PRINTS\n");
    strncpy(buf, s->operand, SZ);
    if (p != &s)
        exit(2600);
    printf(buf);
    printf("\n");
#undef SZ
}

void    printd(struct cmd_state_t *s)
{
#define SZ 128
    void    *p = &s;
    char    buf[SZ];
    fprintf(stderr, "EXEC PRINTD\n");
    strncpy(buf, &s->mem[s->di], atoi(s->operand));
    if (p != &s)
        exit(2600);
    printf(buf);
    printf("\n");
#undef SZ
}

void    cmp_ab(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC CMP_AB\n");
    s->flags = s->a - s->b;
}

void    cmp_bc(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC CMP_BC\n");
    s->flags = s->b - s->c;
}

void    cmp_ac(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC CMP_AC\n");
    s->flags = s->a - s->c;
}

void    add(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC ADD\n");
    s->flags = s->a = s->b + s->c;
}

void    mul(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC MUL\n");
    s->flags = s->a = s->b * s->c;
}

void    divide(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC DIVIDE\n");
    s->flags = s->a = s->b / s->c;
}

void    mod(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC MOD\n");
    s->flags = s->a = s->b % s->c;
}

void    load_b(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC LOAD_B\n");
    s->flags = s->b = atoi(s->operand);
}

void    load_c(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC LOAD_C\n");
    s->flags = s->c = atoi(s->operand);
}

void    mov_b(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC MOV_B\n");
    s->flags = s->b = s->a;
}

void    mov_c(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC MOV_C\n");
    s->flags = s->c = s->a;
}

void    mov_si(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC MOV_SI\n");
    s->flags = s->si = s->a;
}

void    mov_di(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC MOV_DI\n");
    s->flags = s->di = s->a;
}


#if LEVEL < 8
void    fetch_buf(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC FETCH_BUF\n");
    // load at most 128 bytes of operand into current position of SI
    int end = (s->di + 128) % 9999;
    if (end < s->di)
        end = 9999;
    for (int begin = s->di; begin < end && s->operand[begin - s->di]; begin += 1)
        s->flags = s->mem[begin] = s->operand[begin - s->di];
}
#endif

void    movmem(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC MOVMEM\n");
    // si to di copy A bytes
    for (int i = 0; i < s->a; i += 1)
    {
        int dest = (s->di + i) % 9999;
        int src = (s->si + i) % 9999;
        s->flags = s->mem[dest] = s->mem[src];
    }
}

void    swapdisi(struct cmd_state_t *s)
{
    int tmp = s->di;
    s->di = s->si;
    s->si = tmp;
}

void    jmp(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC JMP\n");
    int rel_adr = atoi(s->operand);
    s->ip += rel_adr;
}

void    je(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC JE\n");
    int rel_adr = atoi(s->operand);
    if (s->flags == 0)
        s->ip += rel_adr;
}

void    jne(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC JNE\n");
    int rel_adr = atoi(s->operand);
    if (s->flags != 0)
        s->ip += rel_adr;
}

void    jlt(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC JLT\n");
    int rel_adr = atoi(s->operand);
    if (s->flags < 0)
        s->ip += rel_adr;
}

void    jle(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC JLE\n");
    int rel_adr = atoi(s->operand);
    if (s->flags <= 0)
        s->ip += rel_adr;
}

void    jgt(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC JGT\n");
    int rel_adr = atoi(s->operand);
    if (s->flags > 0)
        s->ip += rel_adr;
}

void    jge(struct cmd_state_t *s)
{
    fprintf(stderr, "EXEC JGE\n");
    int rel_adr = atoi(s->operand);
    if (s->flags >= 0)
        s->ip += rel_adr;
}

void    connect(struct cmd_state_t *s)
{
    // TODO: in future version must open /dev/frogXX
}

void    disconnect(struct cmd_state_t *s)
{
    // TODO: in future version release file descriptor
}

void exec_instructions(struct cmd_instruction_t *insn)
{
    struct cmd_state_t  state;
    memset(&state, 0, sizeof (state));
    while (2600)
    {
        fprintf(stderr, "EXEC INSN IP %d\n", state.ip);
        if (!insn[state.ip].f)
            break;
        char *op = insn[state.ip].operand;
        if (op != NULL)
            state.operand = strdup(op);
        insn[state.ip].f(&state);
        if (state.operand != NULL)
        {
            free(state.operand);
            state.operand = NULL;
        }
        state.ip += 1;
    }
    fprintf(stderr, "END OF INSN\n");
}

struct cmd_fun_t    cmds[] = {
    {"dump", NO_OP, dump},
    {"prints", OP_STR, prints},
    {"printd", OP_INT, printd},
    {"add", NO_OP, add},
    {"mul", NO_OP, mul},
    {"div", NO_OP, divide},
    {"mod", NO_OP, mod},
    {"load_b", OP_INT, load_b},
    {"load_c", OP_INT, load_c},
    {"mov_b", NO_OP, mov_b},
    {"mov_c", NO_OP, mov_c},
    {"mov_si", NO_OP, mov_si},
    {"mov_di", NO_OP, mov_di},
#if LEVEL < 8
    {"fetch_buf", OP_STR, fetch_buf},
#endif
    {"movmem", NO_OP, movmem},
    {"swapdisi", NO_OP, swapdisi},
    {"jmp", OP_INT, jmp},
    {"cmp_ab", NO_OP, cmp_ab},
    {"cmp_bc", NO_OP, cmp_bc},
    {"cmp_ac", NO_OP, cmp_ac},
    {"je", OP_INT, je},
    {"jne", OP_INT, jne},
    {"jlt", OP_INT, jlt},
    {"jle", OP_INT, jle},
    {"jgt", OP_INT, jgt},
    {"jge", OP_INT, jge},
    {"push_a", NO_OP, push_a},
    {"push_b", NO_OP, push_b},
    {"push_c", NO_OP, push_c},
    {"push_si", NO_OP, push_si},
    {"push_di", NO_OP, push_di},
    {"pop_a", NO_OP, pop_a},
    {"pop_b", NO_OP, pop_b},
    {"pop_c", NO_OP, pop_c},
    {"pop_si", NO_OP, pop_si},
    {"pop_di", NO_OP, pop_di},
    {"call", OP_INT, call},
    {"ret", NO_OP, ret},
    {"connect", NO_OP, connect},
    {"disconnect", NO_OP, disconnect},
    {0, 0},
};

void vuln_function(FILE *, struct cmd_instruction_t *);

int main(int ac, char *av[])
{
    FILE *input = stdin;
    struct cmd_instruction_t insn[1000];
    printf("Simple Virtual Machine helping you to understand BOF/ROP/etc...\n");
    if (ac > 1)
        input = fopen(av[1], "r");
    while (!feof(input))
    {
        vuln_function(input, insn);
        exec_instructions(insn);
        memset(insn, 0, sizeof (insn));
    }
    fclose(input);
}
