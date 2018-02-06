//hdr
#ifndef LINEAREQ_H
#define LINEAREQ_H

int solve_linear(double mat[], double out[], int row, int col);

void matrix_swap(double mat[], int col, int row1, int row2);

void matrix_add(double mat[], int col, int row1, double val1, int row2, double val2);

void matrix_print(double mat[], int row, int col);

#endif

//src
#ifndef LINEAREQ_C

#include <stdio.h>

//return 1 linear equation yiels 1 set solution
//return 0 linear equation yiels multiple solutions
int solve_linear(double mat[], double out[], int row, int col)
{
#define mat_at(x,y) mat[col * y + x]
#define mat_swap(x,y) matrix_swap(mat, col, x, y)
#define mat_add(x,v1,y,v2) matrix_add(mat, col, x, v1, y, v2);

    int rownum = 0; 
    int colnum = 0;

    while(colnum < col)
    {
        if(colnum < col - 1)
        {
            while(!mat_at(colnum, rownum))
            {
                rownum++; 
                if(rownum == row - 1)
                    return 0;
            }

            mat_swap(colnum, rownum);

            if(!mat_at(colnum, colnum))
                return 0;
        }

        rownum = colnum;
        rownum++;
        while(rownum < row)
        {
            if(mat_at(colnum, rownum))
            {
                mat_add(rownum, mat_at(colnum,colnum), colnum, -1 * mat_at(colnum, rownum));
            }
            rownum++;
        }

        colnum++;
        rownum = colnum;
    }

    //solve eq
    for(int i = row-1; i >= 0; i--)
    {
        out[i] = mat_at(col - 1,i);

        for(int n = 1; n < row - i; n++)
        {
            out[i] -= mat_at(i+n,i) * out[i+n];
        }

        out[i] /= mat_at(i,i);
    }

    return 1;
}

void matrix_swap(double mat[], int col, int row1, int row2)
{
    if(row1 == row2)
        return;

    double tempvec[col];

    for(int i = 0; i < col; i++)
        tempvec[i] = mat[col * row1 + i];

    for(int i = 0; i < col; i++)
        mat[col * row1 + i] = mat[col * row2 + i];

    for(int i = 0; i < col; i++)
        mat[col * row2 + i] = tempvec[i];
}

void matrix_add(double mat[], int col, int row1, double val1, int row2, double val2)
{
    if(row1 == row2)
        return;

    for(int i = 0; i < col; i++)
    {
        mat[col * row1 + i] *= val1;
        mat[col * row1 + i] += mat[col * row2 + i] * val2;
    }
}

void matrix_print(double mat[], int row, int col)
{
    printf("Matrix %i x %i\n", row, col);
    for(int y = 0; y < row; y++)
    {
        printf("| ");
        for(int x = 0; x < col; x++)
        {
            printf("%f ", mat[col * y + x]);
        }
        printf(" |\n");
    }
}

#endif
