#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_LENGTH 100

#define DELIM " \n\t"

// define a command structure
struct command_t
{
    int index;
    char instruction[MAX_LENGTH];
};

// some register of CPU
struct cpu_t
{
    int pc;
    short ax;
    short bx;
    short cx;
    short dx;
};

// find the register according to name
short * find_register(struct cpu_t * pcpu, const char * name)
{
    if (strcmp(name, "ax") == 0)
    {
        return &pcpu->ax;
    }
    if (strcmp(name, "bx") == 0)
    {
        return &pcpu->bx;
    }
    if (strcmp(name, "cx") == 0)
    {
        return &pcpu->cx;
    }
    if (strcmp(name, "dx") == 0)
    {
        return &pcpu->dx;
    }
    return NULL;
}

// read the asm file
void read_file(const char * filename, struct command_t commands[ ], int * ptr_cmd_cnt)
{
    FILE * fp = fopen(filename, "r");
    assert(fp);

    (*ptr_cmd_cnt) = 0;

    char buffer[MAX_LENGTH];
    while (fgets(buffer, MAX_LENGTH, fp))
    {
        if (buffer[0] == '#')
        {
            strcpy(commands[(*ptr_cmd_cnt)].instruction, "");
        }
        else
        {
            strcpy(commands[(*ptr_cmd_cnt)].instruction, buffer);
        }
        (*ptr_cmd_cnt)++;
    }
    fclose(fp);
}

// read command
void process_read(struct cpu_t * pcpu)
{
    char * p;
    p = strtok(NULL, DELIM);
    assert(p);
    short * preg = find_register(pcpu, p);
    scanf("%hd", preg);
}

// print command
void process_print(struct cpu_t * pcpu)
{
    char * p;
    p = strtok(NULL, DELIM);
    assert(p);
    short * preg = find_register(pcpu, p);

    short value;
    if (preg != NULL)
    {
        value = *preg;
    }
    else
    {
        value = atoi(p);
    }
    printf("%hd", value);
    //printf("\n");
}

// move command
void process_move(struct cpu_t * pcpu)
{
    char * p1;
    p1 = strtok(NULL, DELIM);
    assert(p1);
    short * src = find_register(pcpu, p1);

    char * p2;
    p2 = strtok(NULL, DELIM);
    assert(p2);
    short * dst = find_register(pcpu, p2);

    // y must be a register
    assert(dst);

    int value;
    if (src)
    {
        value = *src;
    }
    else
    {
        value = atoi(p1);
    }

    *dst = value;
}

// arithmetic  command
void process_arithmetic(struct cpu_t * pcpu, const char * type)
{
    char * p1;
    p1 = strtok(NULL, DELIM);
    assert(p1);
    short * src = find_register(pcpu, p1);

    char * p2;
    p2 = strtok(NULL, DELIM);
    assert(p2);
    short * dst = find_register(pcpu, p2);

    // y must be a register
    assert(dst);

    int value;
    if (src)
    {
        value = *src;
    }
    else
    {
        value = atoi(p1);
    }

    if (strcmp(type, "add") == 0)
    {
        *dst = value + *dst;
    }
    else if (strcmp(type, "sub") == 0)
    {
        // Subtract x from y, place result in y
        *dst = *dst - value;
    }
    else if (strcmp(type, "mul") == 0)
    {
        *dst = value * *dst;
    }
    else if (strcmp(type, "div") == 0)
    {
        *dst = value / *dst;
    }
    else
    {
        assert(0);
    }
}

//  jmp command
void process_jmp(struct cpu_t * pcpu, const char * type)
{
    char * p;
    p = strtok(NULL, DELIM);
    assert(p);
    int pc = atoi(p);

    if (strcmp(type, "jmp") == 0)
    {
        pcpu->pc = pc - 1;
        return;
    }

    char * p1;
    p1 = strtok(NULL, DELIM);
    assert(p1);
    short * src = find_register(pcpu, p1);
    int value1;
    if (src)
    {
        value1 = *src;
    }
    else
    {
        value1 = atoi(p1);
    }

    char * p2;
    p2 = strtok(NULL, DELIM);
    assert(p2);
    short * dst = find_register(pcpu, p2);
    int value2;
    if (dst)
    {
        value2 = *dst;
    }
    else
    {
        value2 = atoi(p2);
    }

    if (strcmp(type, "je") == 0)
    {
        if (value1 == value2)
        {
            pcpu->pc = pc - 1;
        }
    }
    if (strcmp(type, "jne") == 0)
    {
        if (value1 != value2)
        {
            pcpu->pc = pc - 1;
        }
    }
    if (strcmp(type, "jg") == 0)
    {
        if (value1 > value2)
        {
            pcpu->pc = pc - 1;
        }
    }
    if (strcmp(type, "jge") == 0)
    {
        if (value1 >= value2)
        {
            pcpu->pc = pc - 1;
        }
    }
    if (strcmp(type, "jl") == 0)
    {
        if (value1 < value2)
        {
            pcpu->pc = pc - 1;
        }
    }
    if (strcmp(type, "jle") == 0)
    {
        if (value1 <= value2)
        {
            pcpu->pc = pc - 1;
        }
    }
}

int main(int argc, char *argv[])
{
    assert(argc == 2);
    const char * filename;
    //filename = "ex1.asm";
    //filename = "ex2.asm";
    //filename = "b.sh";
    filename = argv[1];

    struct command_t commands[100];
    int cmd_cnt;
    struct cpu_t cpu;
    struct cpu_t * pcpu = &cpu;

    // initialize the CPU reg
    memset(pcpu, 0, sizeof(struct cpu_t));

    // read the asm file
    read_file(filename, commands, &cmd_cnt);

    char * type;

    char buffer[MAX_LENGTH];
    while (cpu.pc < cmd_cnt)
    {
        strcpy(buffer, commands[cpu.pc].instruction);
        type = strtok(buffer, DELIM);
        if (type == NULL)
        {
        }
        else if (strcmp(type, "read") == 0)
        {
            process_read(pcpu);
        }
        else if (strcmp(type, "print") == 0)
        {
            process_print(pcpu);
        }
        else if (strcmp(type, "mov") == 0)
        {
            process_move(pcpu);
        }
        else if (
            strcmp(type, "add") == 0 ||
            strcmp(type, "sub") == 0 ||
            strcmp(type, "mul") == 0 ||
            strcmp(type, "div") == 0)
        {
            process_arithmetic(pcpu, type);
        }
        else if (
            strcmp(type, "jmp") == 0 ||
            strcmp(type, "je") == 0 ||
            strcmp(type, "jne") == 0 ||
            strcmp(type, "jg") == 0 ||
            strcmp(type, "jge") == 0 ||
            strcmp(type, "jl") == 0 ||
            strcmp(type, "jle") == 0)
        {
            process_jmp(pcpu, type);
        }
        else
        {
            assert(0);
        }
        cpu.pc = cpu.pc + 1;
    }

    //printf("ax = %hd\n", cpu.ax);
    //printf("bx = %hd\n", cpu.bx);
    //printf("cx = %hd\n", cpu.cx);
    //printf("dx = %hd\n", cpu.dx);

    return 0;
}


