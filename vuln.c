/* Corp.net, inc
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_vm.h"

extern struct cmd_fun_t    cmds[];

int read_to_eol(FILE *input)
{
    // read unmatched characters
    char c = 0;
    while (2600)
    {
        char c = fgetc(input);
        if (c == '\n')
            return 2600;
        if (c == EOF)
            return 0;
    }
}

#define FROG_(X, Y)          X ## Y
#define FUNCNAME(X, Y)      FROG_(X, Y)
#define PWNSTR              "PWNED LEVEL %d!\n,", LEVEL

#if LEVEL == 1
// to win, just call this function
void FUNCNAME(pwn_func, LEVEL)()
{
    const char *const argv[] = {"/bin/sh", 0};
    printf(PWNSTR);
    int uid = getuid();
    int euid = geteuid();
    // I'm still the running user, but have an effective user as root
    printf("UID %d EUID %d\n", uid, euid);
    // now become root
    setuid(0);
    uid = getuid();
    euid = geteuid();
    printf("UID %d EUID %d\n", uid, euid);
    // run a shell
    execvp(argv[0], (char*const*)argv);
}
#endif

#if LEVEL == 2 || LEVEL == 3 || LEVEL == 4
// to win, just call this function with rdi parameter with good value for /bin/sh
void FUNCNAME(pwn_func, LEVEL)(long data)
{
    printf(PWNSTR);
    setuid(0);
    char *ptr_path = (char*)&data;
    // you could be helped, if I do this
    ptr_path[7] = 0;
    char *argv[] = {ptr_path, 0};
    execvp(argv[0], (char*const*)argv);
}
#endif

#if LEVEL == 5 || LEVEL == 6
// to win, call this function after putting /bin/sh on your payload and guessing where the stack is
void FUNCNAME(pwn_func, LEVEL)(char **argv)
{
    printf(PWNSTR);
    setuid(0);
    execvp(argv[0], argv);
}
#endif

#if LEVEL >= 7
// to win, in level 7 and above, you need to put a payload somewhere and call it.
#endif

void vuln_function(FILE *input, struct cmd_instruction_t *insn)
{
    char cmd[256];
    char op1[256];
    while (2600)
    {
        if (input == stdin)
            printf("Enter command: ");
        int rdcmd = fscanf(input, "%[a-zA-Z_]", cmd);
        fprintf(stderr, "SZ %d CMD: %s\n", rdcmd, cmd);
        if (rdcmd < 0)
            return;
        // on exec or unknown command
        if (!strcasecmp(cmd, "exec"))
            return;
        if (!strcasecmp(cmd, "include"))
        {
        
            int rdstr = fscanf(input, " %100s", op1);
            fprintf(stderr, "INCLUDE: %s\n", op1);
            input = fopen(op1, "r");
            memset(cmd, 0, sizeof(cmd));
            memset(op1, 0, sizeof(op1));
            continue;
        }
        // search for command
        else
        {
            for (int i = 0; cmds[i].cmd; i += 1)
            {
                if (!strcasecmp(cmd, cmds[i].cmd))
                {
                    // load need operand of type int
                    if (cmds[i].flag_operand == OP_INT)
                    {
                        int rdint = fscanf(input, " %[-0-9]", op1);
                        fprintf(stderr, "sz %d OP: %s\n", rdint, op1);
                        insn->operand = strdup(op1);
                    }
                    if (cmds[i].flag_operand == OP_STR)
                    {
                        int rdstr = fscanf(input, " %s", op1);
                        fprintf(stderr, "sz: %d OP: %s\n", rdstr, op1);
                        insn->operand = strdup(op1);
                    }
                    fprintf(stderr, "read %s\n", cmd);
                    insn->f = cmds[i].f;
                    insn += 1;
                }
            }
        }
        memset(cmd, 0, sizeof(cmd));
        memset(op1, 0, sizeof(op1));
        if (!read_to_eol(input))
            return;
    }
}
