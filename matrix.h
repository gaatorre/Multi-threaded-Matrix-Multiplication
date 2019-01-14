#ifndef MATRIX_H
#define MATRIX_H
#include <stdint.h>

#define MAX 50

typedef struct
{
  uint32_t *grid;
  uint32_t size;
} matrix;

matrix * matrixCreate(uint32_t);
void matrixDestory(matrix *);
void matrixFill(matrix *);
uint32_t matrixDotProduct(uint32_t *, uint32_t *, uint32_t);
uint8_t matrixMultiply(matrix *, matrix *, matrix *, uint32_t, uint32_t);
void matrixPrint(matrix *);

#endif
