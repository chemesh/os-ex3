
#ifndef EX3_Q1_H
#define EX3_Q1_H

#include "ex3_q1_given.h"

/*********************** DEFENITIONS ************************/
#define MAX_BUFFER 1024
#define max(a,b) (((a) > (b)) ? (a) : (b)) 

/*********************** GLOBALS ************************/
int mat[N][M];          
int product_arr[M];
int init_count, mult_count;

pthread_mutex_t mtx1;
pthread_mutex_t* mat_mtx;

pthread_mutex_t mtx2;
pthread_mutex_t* mult_mtx;

pthread_mutex_t mtx3;
pthread_mutex_t* prime_mtx;

pthread_mutex_t mtx4;
pthread_mutex_t* print_mtx;

pthread_mutex_t mtx5;
pthread_mutex_t* handle_mtx;

pthread_mutex_t mult_lock;
pthread_cond_t init_complete;

pthread_mutex_t fac_lock;
pthread_cond_t mult_complete;

/*********************** FUNCTION PROTOTYPES ************************/
/// all functions are  implemented in utils.h
// logic 

void* set_row_in_mat(void* p_row);
void* mul_max_nums_in_col(void* p_col);
void* factorize(void* p_index);
//int find_max_in_col(int col);
void find2MaxNums(int col, int* max1, int* max2);
// misc.

void validate(int* irets, int t_count);
void atomic_print(const char* msg);
void t_list_join(pthread_t* t_list, int t_list_size);

#endif //EX3_Q1_H