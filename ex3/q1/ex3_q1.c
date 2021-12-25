
#include "ex3_q1.h"

int main(){

    mat_mtx = &mtx1;
    mult_mtx = &mtx2;
    prime_mtx = &mtx3;
    print_mtx = &mtx4;
    handle_mtx = &mtx5;
    init_count = 0;
    mult_count = 0;

    pthread_t init_threads[N], mul_threads[M], fac_threads[M];
    int irets[max(N,M)];
    // char buffer[MAX_BUFFER];

    pthread_mutex_init(&mult_lock, NULL);
    pthread_cond_init (&init_complete, NULL);

    pthread_mutex_init(&fac_lock, NULL);
    pthread_cond_init (&mult_complete, NULL);

    // initialize mat using the initializers threads
    int rows[N];
    for (int i = 0; i < N; i++){
        rows[i] = i;
        irets[i] = pthread_create(&init_threads[i], NULL, set_row_in_mat, (void*)(rows+i));
    }
    atomic_print(INIT_CREATED);
    validate(irets,N);

    // for each column 0<=j<M in the matrix, use a thread to find & multiply the largest numbers in the column,
    // and store the product in product_arr at index j
    int cols[M];
    for (int i = 0; i < M ; i++){
        cols[i] = i;
        irets[i] = pthread_create(&mul_threads[i], NULL, mul_max_nums_in_col, (void*)(cols+i));
    }

    atomic_print(MULT_CREATED);
    validate(irets,M);


    //for each value in index j of product_arr, factorize the value and write it to stdout
        for (int i = 0; i < M ; i++){
            char buffer[MAX_BUFFER];
            sprintf(buffer, "COLS [i] IS : %d", cols[i]);
            atomic_print(buffer);
        irets[i] = pthread_create(&fac_threads[i], NULL, factorize, (void*)(cols+i));
    }

    atomic_print(FACT_CREATED);
    validate(irets,M);

    t_list_join(init_threads, N);
    atomic_print(INIT_TERMINATED);

    t_list_join(mul_threads, M);
    atomic_print(MULT_TERMINATED);

    // for (int i = 0; i < M; i++){
    //     pthread_join(fac_threads[i], NULL);
    // }
    // atomic_print(FACT_TERMINATED);

    exit(0);
}



