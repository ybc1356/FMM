#include "fmm.h"

void add_mat(int size, int* mat1, int* mat2, int* result);
void subtract_mat(int size, int* mat1, int* mat2, int* result);
void fmm(int size, int* mat1, int* mat2, int* result);
void fmm_naive(int size, int* mat1, int* mat2, int* result);
void fmm_naive_unrolled(int size, int* mat1, int* mat2, int* result);


void fmm(int n, int* m1, int* m2, int* result) {
    if (n < 4) {
        fmm_naive( n, m1, m2, result);
        return;
    }

    if (n > 4 && n < 16){
        fmm_naive_unrolled(n, m1, m2, result);
        return;
    }
    // stopping condition for recursive calls
    if (n == 1) {
        result[0] = m1[0] * m2[0];
        return;
    }

   int offset = n >> 1;
   int size = offset;

    // Declaration of submatrices
    int* A1 = &m1[0];
    int* A2 = &m1[offset];
    int* A3 = &m1[(offset * n)];
    int* A4 = &m1[(offset * n) + offset];
    int* B1 = &m2[0];
    int* B2 = &m2[offset];
    int* B3 = &m2[(offset * n)];
    int* B4 = &m2[(offset * n) + offset];

    int* C1 = &result[0];
    int* C2 = &result[offset];
    int* C3 = &result[(offset * n)];
    int* C4 = &result[(offset * n) + offset];
    
    

    int* temp1 = (int*)malloc(size *  size * sizeof(int));
    int* temp2 = (int*)malloc(size *  size * sizeof(int));
    int* M1 = (int*)malloc(size *  size * sizeof(int));
    int* M2 = (int*)malloc(size *  size * sizeof(int));
    int* M3 = (int*)malloc(size *  size * sizeof(int));
    int* M4 = (int*)malloc(size *  size * sizeof(int));
    int* M5 = (int*)malloc(size *  size * sizeof(int));
    int* M6 = (int*)malloc(size *  size * sizeof(int));
    int* M7 = (int*)malloc(size *  size * sizeof(int));

    // get values for the M matrices
    add_mat(size, A1, A4, temp1);
    add_mat(size, B1, B4, temp2);
    fmm(size, temp1, temp2, M1);

    add_mat(size, A3, A4, temp1);    
    fmm(size, temp1, B1, M2);

    subtract_mat(offset, B2, B4, temp1);
    fmm(size, A1, temp1, M3);

    subtract_mat(size, B3, B1, temp1);
    fmm(size, A4, temp1, M4);

    add_mat(size, A1, A2, temp1);
    fmm(size, temp1, B4, M5);

    subtract_mat(size, A3, A1, temp1);
    add_mat(size, B1, B2, temp2);
    fmm(size, temp1, temp2, M6);

    subtract_mat(size, A2, A4, temp1);
    add_mat(size, B3, B4, temp2);
    fmm(size, temp1, temp2, M7);
   

    // Solve for submatrics of result
    add_mat(size, M1, M4, temp1);
    subtract_mat(size, temp1, M5, temp2);
    add_mat(size, temp2, M7, C1);
    add_mat(size, M3, M5, C2);
    add_mat(size, M2, M4, C3);

    subtract_mat(size, M1, M2, temp1);
    add_mat(size, temp1, M3, temp2);
    add_mat(size, temp2, M6, C4);
    free(temp1);
    free(temp2);
    free(M1);
    free(M2);
    free(M3);
    free(M4);
    free(M5);
    free(M6);
    free(M7);
    return;
}

void add_mat(int n, int* m1, int* m2, int* result) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
           result[i * n + j] = m1[i * n + j] + m2[ i * n + j];
        }
    }
    return;
}

void subtract_mat (int n, int* m1, int* m2, int* result) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
            result[i * n + j] = m1[i * n + j] - m2[i * n + j];
        }
    }
    return;
}

void fmm_naive(int n, int* m1, int* m2, int* result) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i * n + j] = 0;  // result[i][j] = 0
            for (int k = 0; k < n; k++) 
                result[i * n + j] += m1[i * n + k] * m2[k * n + j];  // result[i][j] += m1[i][k] * m2[k][j]
        }
    }
}
void fmm_naive_unrolled(int n, int* m1, int* m2, int* result) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i * n + j] = 0;  // result[i][j] = 0
            for (int k = 0; k < n; k++) 
                result[i * n + j] += m1[i * n + k] * m2[k * n + j]
                                    + m1[i * n + k + 1] * m2[(k+ 1) * n + j]
                                    + m1[i * n + k + 2] * m2[(k+2) * n + j]
                                    +m1[i * n + k + 3] * m2[(k+ 3) * n + j];  // result[i][j] += m1[i][k] * m2[k][j]
                
        }
    }
}
