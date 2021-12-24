
#include "ex3_q1.h"

int main(){

    main_thread_id = (int)pthread_self();
    char buffer[MAX_BUFFER];
    pthread_t init_threads[N], mul_threads[M], fac_threads[M];
    int* irets[max(N,M)]; 

    pthread_mutex_init(&mult_lock, NULL);
    pthread_cond_init (&init_complete, NULL);

    pthread_mutex_init(&fac_lock, NULL);
    pthread_cond_init (&mult_complete, NULL);

    // initialize mat using the initializers threads
    for (int i = 0; i < N; i++){
        int* pi = &i;
        irets[i] = pthread_create(&init_threads[i], NULL, set_row_in_mat, (void*)pi);
    }
    atomic_print(INIT_CREATED);
    validate(irets,N);

    // for each column 0<=j<M in the matrix, use a thread to find & multiply the largest numbers in the column,
    // and store the product in product_arr at index j
    for (int i = 0; i < M ; i++){
        int* pi = &i;
        irets[i] = pthread_create(&mul_threads[i], NULL, mul_max_nums_in_col, (void*)pi);
    }

    atomic_print(MULT_CREATED);
    validate(irets,M);


    //for each value in index j of product_arr, factorize the value and write it to stdout
        for (int i = 0; i < M ; i++){
        int* pi = &i;
        irets[i] = pthread_create(&fac_threads[i], NULL, factorize, (void*)pi);
    }

    atomic_print(FACT_CREATED);
    validate(irets,M);

    pthread_cond_wait(&init_complete;
    atomic_print(INIT_TERMINATED);

    pthread_cond_wait(&mult_complete);
    atomic_print(MULT_TERMINATED);

    for (int i = 0; i < M; i++){
        pthread_join(fac_threads[i], NULL);
    }
    atomic_print(FACT_TERMINATED);

    return 0;
}

