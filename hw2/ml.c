/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */

#include <stdio.h>
#include <stdlib.h>

// all methods declarations
double **multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double **transposeMatrix(double **mat, int row, int col);
double **inverseMatrix(double **matA, int dimension);

// main method starts here
int main(int argc, char **argv)
{
    FILE *fpTest;
    FILE *fpTrain;
    fpTrain = fopen(argv[1], "r");
    fpTest = fopen(argv[2], "r");

    int attrNum, trainNum, testNum;
    fscanf(fpTrain, "%d\n", &attrNum);
    fscanf(fpTrain, "%d\n", &trainNum);
    fscanf(fpTest, "%d\n", &testNum);
    double **x = (double **)calloc(trainNum, sizeof(double *));
    int i, j;
    int attrPlus = attrNum + 1;
    for (i = 0; i < trainNum; i++)
    {
        x[i] = calloc(attrPlus, sizeof(double));
    }
    double **y = (double **)calloc(trainNum, sizeof(double *));
    for (i = 0; i < trainNum; i++)
    {
        y[i] = (double *)calloc(1, sizeof(double));
    }
    // double** firstInverse = (double**)calloc(attrPlus, sizeof(double*));
    // for(int i = 0; i < attrPlus;i++){
    //     firstInverse[i] = (double*)calloc(trainNum, sizeof(double));
    // }

    double one = 1.0;
    for (i = 0; i < trainNum; i++)
    {
        x[i][0] = one;
    }
    for (i = 0; i < trainNum; i++)
    {
        for (j = 1; j < attrPlus + 1; j++)
        {

            if (j % attrPlus == 0)
            {
                fscanf(fpTrain, "%lf\n", &y[i][0]);
            }
            else
            {
                fscanf(fpTrain, "%lf", &x[i][j]);
                fscanf(fpTrain, ",");
            }
        }
    }
    double **testX = (double **)calloc(testNum, sizeof(double *));
    for (int i = 0; i < testNum; i++)
    {
        testX[i] = (double *)calloc(attrPlus, sizeof(double));
    }
    for (i = 0; i < testNum; i++)
    {
        for (j = 0; j < attrPlus; j++)
        {
            if (j == 0)
            {
                testX[i][0] = one;
                continue;
            }
            fscanf(fpTest, "%lf", &testX[i][j]);
            fscanf(fpTest, ",");
        }
        fscanf(fpTest, "\n");
    }

    double **firstTran = transposeMatrix(x, trainNum, attrPlus);
    double **firstMulti = multiplyMatrix(firstTran, x, attrPlus, trainNum, trainNum, attrPlus);
    double **firstInverse = inverseMatrix(firstMulti, attrPlus);
    double **secondMulti = multiplyMatrix(firstInverse, firstTran, attrPlus, attrPlus, attrPlus, trainNum);
    int o = 1;
    double **w = multiplyMatrix(secondMulti, y, attrPlus, trainNum, trainNum, 1);
    double **result = multiplyMatrix(testX, w, testNum, attrPlus, attrPlus, o);
    for (i = 0; i < testNum; i++)
    {
        printf("%0.0f\n", result[i][0]);
    }

    //printf("result:\n");
    //begin freeing
    for (i = 0; i < trainNum; i++)
    {
        free(x[i]);
        free(y[i]);
    }
    free(x);
    free(y);
    for (i = 0; i < testNum; i++)
    {
        free(testX[i]);
    }
    free(testX);

    for (i = 0; i < attrPlus; i++)
    {
        free(firstInverse[i]);
        free(firstTran[i]);
        free(firstMulti[i]);
        free(secondMulti[i]);
        free(w[i]);
    }
    free(firstInverse);
    free(firstTran);
    free(firstMulti);
    free(secondMulti);
    free(w);

    for (i = 0; i < testNum; ++i)
    {
        free(result[i]);
    }
    free(result);

    fclose(fpTrain);
    fclose(fpTest);

    return 0;
}

double **multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
    if (c1 != r2)
    {
        printf("error: cannot multiply, matrix length not match\n");
        return NULL;
    }
    double **result = calloc(r1, sizeof(double *));

    int i, j, k;
    for (i = 0; i < r1; i++)
    {
        result[i] = (double *)calloc(c2, sizeof(double));
    }
    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {
            for (k = 0; k < c1; k++)
            {
                result[i][j] = result[i][j] + matA[i][k] * matB[k][j];
            }
        }
    }

    return result;
}

double **transposeMatrix(double **mat, int row, int col)
{

    double **matTran = calloc(col, sizeof(double *));

    int i, j;
    for (i = 0; i < col; i++)
    {
        matTran[i] = (double *)calloc(row, sizeof(double));
    }
    //printf("I'm here\n");
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            matTran[j][i] = mat[i][j];
        }
    }

    return matTran;
}

void toOne(double **table, int row, int col, int n)
{
    int i;
    double a = table[row][col];
    for (i = 0; i < n - col; i++)
    {
        table[row][col + i] = table[row][col + i] / a;
    }
}
void toZero(double **table, int row, int col, int n)
{
    int i;
    double temp = table[row][col];
    for (i = col; i < n; i++)
    {
        table[row][i] = table[row][i] - (temp * table[col][i]);
    }
}

double **inverseMatrix(double **matA, int dimension)
{

    double **matI = calloc(dimension, sizeof(double *));

    int i, j;
    for (i = 0; i < dimension; i++)
    {
        matI[i] = (double *)calloc(dimension, sizeof(double));
    }
    double **result = (double **)calloc(dimension, sizeof(double *));
    for (i = 0; i < dimension; i++)
    {
        result[i] = (double *)calloc(dimension * 2, sizeof(double));
    }
    for (i = 0; i < dimension; i++)
    {
        for (j = 0; j < dimension * 2; j++)
        {
            if (j >= dimension)
            {
                if (j - dimension == i)
                {
                    result[i][j] = 1;
                }
                else
                {
                    result[i][j] = 0;
                }
            }
            else
            {
                result[i][j] = matA[i][j];
            }
        }
    }
    /*///////////////////////////////////////////////////////////////
    for(i = 0;i <dimension;i++){
        for(j = 0; j <dimension * 2; j++){
            printf("%lf\t",result[i][j]);
        }
        printf("\n");
    }
*/
    ///////////////////////////////////////////////////////////////
    for (i = 0; i < dimension; i++)
    {
        for (j = 0; j < dimension; j++)
        {
            if (i == j)
            {
                toOne(result, i, j, dimension * 2);
                break;
            }
            if (i > j)
            {
                toZero(result, i, j, dimension * 2);
            }
        }
    }

    for (i = 0; i < dimension; i++)
    {
        for (j = 0; j < dimension; j++)
        {
            if (i < j)
            {
                toZero(result, i, j, dimension * 2);
            }
        }
    } /*
    for(i = 0;i <dimension;i++){
        for(j = 0; j <dimension * 2; j++){
            printf("%lf\t",result[i][j]);
        }
        printf("\n");
    }*/
    for (i = 0; i < dimension; i++)
    {
        for (j = 0; j < dimension * 2; j++)
        {
            if (j >= dimension)
            {
                matI[i][j - dimension] = result[i][j];
            }
        }
    }
    for (i = 0; i < dimension; ++i)
    {
        free(result[i]);
    }
    free(result);

    return matI;
}
