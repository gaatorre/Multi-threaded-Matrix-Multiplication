#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

matrix * matrixCreate(uint32_t size)
{
  matrix *m;
  m = (matrix *) malloc(sizeof(matrix));
  m->size = size;
  m->grid = (uint32_t *) malloc(size * size * sizeof(uint32_t *));

  return m;
}

void matrixDestory(matrix *m)
{
  free(m->grid);
  free(m);
  return;
}

// Optimize!!!!
void matrixFill(matrix *m)
{
  uint32_t size = m->size * m->size;
  for(uint32_t x = 0; x < size; x++)
  {
    m->grid[x] = rand() & 25;
    // m->grid[x] = x;
  }
  return;
}

uint32_t matrixDotProduct(uint32_t *row, uint32_t *col, uint32_t size)
{
  uint32_t sum = 0;
  for(uint32_t r = 0, c = 0; r < size; r++, c += size)
  {
    // printf("row[%u] = %u\n", r, row[r]);
    // printf("col[%u] = %u\n", c, col[c]);
    sum += row[r] * col[c];
  }
  // printf("Sum is %u\n", sum);
  return sum;
}

uint8_t matrixMultiply(matrix *m, matrix *n, matrix *o, uint32_t start, uint32_t end)
{
  uint32_t size = m->size;

  for(uint32_t x = start; x < end; x++)
  {
    uint32_t rowPosition = (x / size) * size;
    uint32_t colPosition = x % size;
    o->grid[x] = matrixDotProduct(m->grid + rowPosition, n->grid + colPosition, size);
  }
  return 1;
}
void matrixPrint(matrix *m)
{
  uint32_t size = m->size * m->size;

  for(uint32_t x = 0; x < size; x++)
  {
    if((x+1) % (m->size))
      printf("%5u ", m->grid[x]);
    else
      printf("%5u\n", m->grid[x]);
  }
  return;
}
