
#include "ex3_q1_given.h"

/*********************** CONSTS ************************/
#define MAX_BUFFER 1024

/*********************** MACROS ************************/
#define max(a,b) (((a) > (b)) ? (a) : (b)) 

/*********************** GLOBALS ************************/
int mat[N][M];          
int product_arr[M];
int main_thread_id;     //id of the main thread

pthread_mutex_t mtx1;
pthread_mutex_t* mat_mtx = &mtx1;

pthread_mutex_t mtx2;
pthread_mutex_t* mult_mtx = &mtx2;

pthread_mutex_t mtx3;
pthread_mutex_t* prime_mtx = &mtx3;

pthread_mutex_t mtx4;
pthread_mutex_t* print_mtx = &mtx4;

pthread_mutex_t mtx5;
pthread_mutex_t* handle_mtx = &mtx5;

pthread_mutex_t mult_lock;
pthread_cond_t init_complete;

pthread_mutex_t fac_lock;
pthread_cond_t mult_complete;

/*********************** FUNCTION PROTOTYPES ************************/
// logic 

void set_row_in_mat(void* p_row);
void mul_max_nums_in_col(void* p_col);
void factorize(void* p_index);

// misc.

void validate(int* irets, int t_count);
void atomic_print(const char* msg);