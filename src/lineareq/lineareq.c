//hdr
#ifndef LINEAREQ_H
#define LINEAREQ_H

int solve_linear(double mat[], double out[], int row, int col);

void matrix_swap(double mat[], int col, int row1, int row2);

void matrix_add(double mat[], int col, int row1, double val1, int row2, double val2);

#endif

//src
#ifndef LINEAREQ_C

int solve_linear(double mat[], double out[], int row, int col)
{
#define mat_at(x,y) mat[col * y + x]
#define mat_swap(x,y) matrix_swap(mat, col, x, y)
#define mat_add(x,v1,y,v2) matrix_add(mat, col, x, v1, y, v2);

    return mat_at(0,1);
}

void matrix_swap(double mat[], int col, int row1, int row2)
{
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
    for(int i = 0; i < col; i++)
    {
        mat[col * row1 + i] *= val1;
        mat[col * row1 + i] += mat[col * row2 + i] * val2;
    }
}


#endif
