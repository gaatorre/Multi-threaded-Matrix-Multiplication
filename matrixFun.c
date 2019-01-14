#include "matrix.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100
#define THREADS 30

typedef struct
{
  matrix *m;
  matrix *n;
  matrix *o;
} mGroup;

typedef struct tArgs
{
  uint32_t id;
  uint32_t start;
  uint32_t end;
  mGroup *group;
}tArgs;

mGroup * mGroupCreate(uint32_t size, time_t seed)
{
  mGroup *group = (mGroup *) malloc(sizeof(mGroup));
  srand(seed);
  group->m = matrixCreate(size);
  group->n = matrixCreate(size);
  group->o = matrixCreate(size);
  matrixFill(group->m);
  matrixFill(group->n);

  return group;
}

void mGroupDestroy(mGroup *group)
{
  matrixDestory(group->o);
  matrixDestory(group->n);
  matrixDestory(group->m);
  free(group);
}

void mGroupPrint(mGroup *group)
{
  printf("Matrix one\n");
  matrixPrint(group->m);
  printf("\n");
  printf("Matrix Two\n");
  matrixPrint(group->n);
  printf("\n");
  printf("Matrix Three\n");
  matrixPrint(group->o);
  printf("\n");
  return;
}

uint64_t sequential(mGroup *group, uint32_t size)
{
  time_t start, end;
  double diff;
  time(&start);
  matrixMultiply(group->m, group->n, group->o, 0, size);
  time(&end);
  return diff = difftime(end, start);
  // printf("Sequential time duration: %f\n", diff);
}

void *single(void *info)
{
  tArgs *args = (tArgs *) info;
  // printf("Thread #%u is here\n", args->id);
  matrixMultiply(args->group->m, args->group->n, args->group->o,
                 args->start, args->end);
  pthread_exit(NULL);
}

uint64_t parallels(mGroup *group, uint32_t size)
{
  pthread_t *workers = malloc(THREADS * sizeof(pthread_t));
  tArgs *argument = malloc(THREADS * sizeof(tArgs));
  uint32_t step = size / THREADS;
  if(!step)
  {
    printf("Size needs to be greater than Threads\n");\
    exit(1);
  }

  time_t start, end;
  double diff;
  time(&start);
  // printf("Creating Threads\n");
  for(uint32_t i = 0; i < THREADS; i++)
  {
    argument[i].id = i;
    argument[i].group = group;
    argument[i].start = i * step;
    if(i == THREADS -1)
    {
      argument[i].end = size;
    }
    else
    {
      argument[i].end = argument[i].start + step;
    }
    pthread_create(&workers[i], NULL, single, &argument[i]);
  }

  for(uint32_t i = 0; i < THREADS; i++)
  {
    // printf("Joining Threads\n");
    pthread_join(workers[i], NULL);
  }

  time(&end);
  diff = difftime(end, start);
  // printf("Parrallel time duration: %f\n", diff);
  free(argument);
  free(workers);
  return diff;
}


int main(int argc, char *argv[])
{
  uint32_t size = SIZE;
  time_t seed = time(0);
  uint8_t verbose = 0;
  uint32_t parallel = 0;
  uint8_t compare = 0;
  int c;

  while((c = getopt(argc , argv , "vn:s:pc")) != -1)
  {
    switch(c) {
      case 'n':
        size = atoi(optarg);
        break;
      case 's':
        seed = atoi(optarg);
        break;
      case 'v':
        verbose = 1;
        break;
      case 'p':
        parallel = 1;
        break;
      case 'c':
        compare = 1;
        break;
      default:
        printf("Unsupported flag\n");
        break;
    }
  }

  mGroup *group;

  if(compare)
  {
    printf("Comparing Parrallel and Sequential Methods\n\n");
    for(uint8_t x = 0; x < 6; x++)
    {
      printf("Creating Matrix\n");
      group = mGroupCreate(size, seed);
      printf("Multiplying a %ux%u matrix\n", size, size);
      printf("Sequential method took %lu\n", sequential(group, size * size));
      printf("Parallel method took %lu\n\n", parallels(group, size * size));
      size += size;
      mGroupDestroy(group);
    }
  }
  else if(parallel)
  {
    group = mGroupCreate(size, seed);
    printf("Parallel multiplication is starting...\n");
    parallels(group, size * size);
    mGroupDestroy(group);
  }
  else
  {
    group = mGroupCreate(size, seed);
    printf("Sequential multiplication is starting...\n");
    sequential(group, size * size);
    mGroupDestroy(group);
  }
  if(verbose) {mGroupPrint(group);}

  if(parallel) {pthread_exit(NULL);}
  return 0;
}
