
#include "ex3_q1_given.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))

int mat[N][M];          
int product_arr[M];
int main_thread_id;     //id of the main thread

// constants and global variables
pthread_mutex_t mtx1;
pthread_mutex_t* mat_mtx = &mtx1;

pthread_mutex_t mtx2;
pthread_mutex_t* mult_mtx = &mtx2;

pthread_mutex_t mtx3;
pthread_mutex_t* prime_mtx = &mtx3;

void set_row_in_mat(void* p_row);

int main(){

    main_thread_id = (int)pthread_self();

    pthread_t init_threads[N], mul_threads[M], factor_threads[M];
    int* irets[max(N,M)]; 


    // initialize mat using the initializers threads
    for (int i = 0; i < N; i++){
        int* pi = &i;
        irets[i] = pthread_create(&init_threads[i], NULL, set_row_in_mat, (void*)pi);
    }
    // wait for all initailizers threads to finish
    for (int i = 0; i < N; i++){
        pthread_join(init_threads[i], NULL);
    }


}

void set_row_in_mat(void* p_row) {

    int row = *((int*)p_row);
    for (int i = 0; i <= row; ++i) {
        int value;
        do {
            pthread_mutex_lock(mat_mtx);
            value = get_random_in_range();
            pthread_mutex_unlock(mat_mtx);
        } while (is_prime(value));
        mat[row][i] = value;
    }

    pthread_exit(NULL);
}