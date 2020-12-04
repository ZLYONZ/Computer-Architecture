#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Directive
{
    char gate[17];
    int num;
    int sum;
    int *input;
    int *output;
    int *selector;
};

// Increment input
int increInput(int *arr, int count)
{
    int i;
    for (i = count + 1; i >= 2; i--)
    {
        arr[i] = !arr[i];
        if (arr[i] == 1)
        {
            return 1;
        }
    }
    return 0;
}

void NOT(int *val, int IN1, int OUT1)
{
    val[OUT1] = !val[IN1];
}

void AND(int *val, int IN1, int IN2, int OUT1)
{
    val[OUT1] = val[IN1] && val[IN2];
}

void OR(int *val, int IN1, int IN2, int OUT1)
{
    val[OUT1] = val[IN1] || val[IN2];
}

void NAND(int *val, int IN1, int IN2, int OUT1)
{
    val[OUT1] = !(val[IN1] && val[IN2]);
}

void NOR(int *val, int IN1, int IN2, int OUT1)
{
    val[OUT1] = !(val[IN1] || val[IN2]);
}

void XOR(int *val, int IN1, int IN2, int OUT1)
{
    val[OUT1] = val[IN1] ^ val[IN2];
}

void PASS(int *val, int IN1, int OUT1)
{
    val[OUT1] = val[IN1];
}

void MUX(int *val, int num, int *IN1, int *IN2, int OUT1)
{
    int sum = 0;
    int i;
    for (i = 0; i < num; i++)
    {
        sum += val[IN2[i]] * pow(2, num - i - 1);
    }
    val[OUT1] = val[IN1[sum]];
}

void DECODER(int *val, int num, int *IN1, int *OUT1)
{
    int sum = 0;
    int i;
    for (i = 0; i < pow(2, num); i++)
    {
        val[OUT1[i]] = 0;
    }
    for (i = 0; i < num; i++)
    {
        sum += val[IN1[i]] * pow(2, num - i - 1);
    }
    val[OUT1[sum]] = 1;
}

// Print names
void printName(int size, char **arr)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%s\n", arr[i]);
    }
}

// Index of
int indexOf(int size, char **arr, char *var)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (strcmp(arr[i], var) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Reset values
void resetVal(int size, int *arr)
{
    int i;
    for (i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
    arr[1] = 1;
}

// Main method 
int main(int argc, char **argv)
{
    if (argc - 1 != 1)
    {
        printf("Invalid number\n");
        return 0;
    }

    //Get file, return if invalid path
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Invalid file\n");
        return 0;
    }

    //Make steps of circuit with struct directive
    struct Directive *steps = NULL;
    int size = 2;
    int inputCount = 0;
    int outputCount = 0;
    int stepCount = 0;
    int totalCount = 0;
    char direct[17];
    char **name;
    int *val = malloc(size * sizeof(int));

    //get INPUT info
    fscanf(file, " %s", direct);
    fscanf(file, "%d", &inputCount);

    size += inputCount;
    name = malloc(size * sizeof(char *));
    name[0] = malloc(2 * sizeof(char));
    name[0] = "0\0";
    name[1] = malloc(2 * sizeof(char));
    name[1] = "1\0";

    int i;
    for (i = 0; i < inputCount; i++)
    {
        name[i + 2] = malloc(17 * sizeof(char));
        fscanf(file, "%*[: ]%16s", name[i + 2]);
    }

    //get OUTPUT info
    fscanf(file, " %s", direct);
    fscanf(file, "%d", &outputCount);
    size += outputCount;
    name = realloc(name, size * sizeof(char *));
    for (i = 0; i < outputCount; i++)
    {
        name[i + inputCount + 2] = malloc(17 * sizeof(char));
        fscanf(file, "%*[: ]%16s", name[i + inputCount + 2]);
    }

    //get STEPS
    while (!feof(file))
    {
        int numInputs = 2, numOutputs = 1;
        struct Directive step;
        int sc = fscanf(file, " %s", direct);
        if (sc != 1)
        {
            break;
        }
        stepCount++;
        step.num = 0;
        step.sum = 0;
        strcpy(step.gate, direct);

        if (strcmp(direct, "NOT") == 0)
        {
            numInputs = 1;
        }
        if (strcmp(direct, "PASS") == 0)
        {
            numInputs = 1;
        }
        if (strcmp(direct, "DECODER") == 0)
        {
            fscanf(file, "%d", &numInputs);
            step.num = numInputs;
            numOutputs = pow(2, numInputs);
        }
        if (strcmp(direct, "MULTIPLEXER") == 0)
        {
            fscanf(file, "%d", &numInputs);
            step.sum = numInputs;
            numInputs = pow(2, numInputs);
        }

        step.input = malloc(numInputs * sizeof(int));
        step.output = malloc(numOutputs * sizeof(int));
        step.selector = malloc(step.sum * sizeof(int));

        char v[17];
        for (i = 0; i < numInputs; i++)
        {
            fscanf(file, "%*[: ]%16s", v);
            step.input[i] = indexOf(size, name, v);
        }

        for (i = 0; i < step.sum; i++)
        {
            fscanf(file, "%*[: ]%16s", v);
            step.selector[i] = indexOf(size, name, v);
        }

        for (i = 0; i < numOutputs; i++)
        {
            fscanf(file, "%*[: ]%16s", v);
            int idx = indexOf(size, name, v);
            if (idx == -1)
            {
                size++;
                totalCount++;
                name = realloc(name, size * sizeof(char *));
                name[size - 1] = malloc(17 * sizeof(char));
                strcpy(name[size - 1], v);
                step.output[i] = size - 1;
            }
            else
            {
                step.output[i] = idx;
            }
        }

        //add step to list of steps
        if (!steps)
        {
            steps = malloc(sizeof(struct Directive));
        }
        else
        {
            steps = realloc(steps, stepCount * sizeof(struct Directive));
        }
        steps[stepCount - 1] = step;
    }

    // initialize values array
    val = malloc(size * sizeof(int));
    resetVal(size, val);

    while (1 < 2)
    {
        //print inputs
        for (i = 0; i < inputCount; i++)
        {
            printf("%d ", val[i + 2]);
        }

        //run through steps, calculate outputs
        for (i = 0; i < stepCount; i++)
        {
            struct Directive step = steps[i];
            if (strcmp(step.gate, "NOT") == 0)
            {
                NOT(val, step.input[0], step.output[0]);
            }
            if (strcmp(step.gate, "AND") == 0)
            {
                AND(val, step.input[0], step.input[1], step.output[0]);
            }
            if (strcmp(step.gate, "OR") == 0)
            {
                OR(val, step.input[0], step.input[1], step.output[0]);
            }
            if (strcmp(step.gate, "NAND") == 0)
            {
                NAND(val, step.input[0], step.input[1], step.output[0]);
            }
            if (strcmp(step.gate, "NOR") == 0)
            {
                NOR(val, step.input[0], step.input[1], step.output[0]);
            }
            if (strcmp(step.gate, "XOR") == 0)
            {
                XOR(val, step.input[0], step.input[1], step.output[0]);
            }
            if (strcmp(step.gate, "PASS") == 0)
            {
                PASS(val, step.input[0], step.output[0]);
            }
            if (strcmp(step.gate, "DECODER") == 0)
            {
                DECODER(val, step.num, step.input, step.output);
            }
            if (strcmp(step.gate, "MULTIPLEXER") == 0)
            {
                MUX(val, step.sum, step.input, step.selector, step.output[0]);
            }
        }

        //print outputs
        for (i = 0; i < outputCount; i++)
        {
            printf("%d ", val[inputCount + i + 2]);
        }
        printf("\n");

        if (!increInput(val, inputCount))
        {
            break;
        }
    }

    return 0;
}