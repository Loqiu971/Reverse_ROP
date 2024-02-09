/* Corp.net, inc
 */

#ifndef __SIMPLE_H
#define __SIMPLE_H

/*
 * State of virtual processor
 * */
struct cmd_state_t
{
    // 3 registers for generic usage
    int     a;
    int     b;
    int     c;
    // source index register
    int     si;
    // destination index register
    int     di;
    // instruction pointer register
    int     ip;
    // stack pointer register
    int     sp;
    // flags register
    int     flags;
    // RAM
    char    mem[10000];
    // STACK memory
    int     stack[100];
    // 
    char    *operand;
};

/*
 * Map instruction name store in `cmd` field to function pointer store in `f`.
 * */
struct cmd_fun_t
{
    // command name
    char    *cmd;
#define NO_OP   0
#define OP_INT  1
#define OP_STR  2
    // operand type
    int     flag_operand;
    // function to execute
    void    (*f)(struct cmd_state_t*);
};

struct cmd_instruction_t
{
    void    (*f)(struct cmd_state_t*);
    char *operand;
};

#endif
