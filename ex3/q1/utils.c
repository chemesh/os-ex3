#include "ex3_q1.h"


void validate(int* irets, int t_count){
    // check for failure in threads execution and exit if happened
    for (int i = 0; i < t_count ; i++){ 
        if (irets[i] != 0){
            char buffer[MAX_BUFFER];
            sprintf(buffer,"error in thread %d, pid: %d - return value from pthread_create call is %d", (int)pthread_self(), (int)getpid(), irets[i]);
            my_exception(__LINE__, __func__,buffer);
        }
    }
}

void atomic_print(const char* msg){
    pthread_mutex_lock(print_mtx);
    print_msg(msg);
    pthread_mutex_unlock(print_mtx);
}

void t_list_join(pthread_t* t_list, int t_list_size){
    for (int i = 0; i < t_list_size; i++){
        pthread_join(t_list[i], NULL);
    }
}

void* set_row_in_mat(void* p_row) {

    int last = 0, row = *((int*)p_row);
    char buffer[MAX_BUFFER];

    create_msg(buffer,INITIALIZER,row+1,STARTING);
    atomic_print(buffer);

    for (int i = 0; i < M; ++i) {
        int value;
        do {
            pthread_mutex_lock(mat_mtx);
            value = get_random_in_range();
            pthread_mutex_unlock(mat_mtx);
        } while (!is_prime(value));
        mat[row][i] = value;
    }

    // check if this is the last thread that initialize the matrix
    // synchronized
    pthread_mutex_lock(handle_mtx);
    init_count++;
    if (init_count == N){
        last = 1;
        //send signal to wake up mult_threads
        for (int i = 0; i < M; ++i){
            pthread_cond_signal(&init_complete);
        }
    }

    pthread_mutex_unlock(handle_mtx);
    if (last){
        // last thread prints the matrix to STDOUT
        pthread_mutex_lock(print_mtx);
        write_matrix(mat);
        pthread_mutex_unlock(print_mtx);
    }

    // print done message
    create_msg(buffer,INITIALIZER,row+1,DONE);
    atomic_print(buffer);

    pthread_exit(NULL);
}

void find_2_max_nums(int col, int* max1, int* max2)
{
    *max1 = *max2 = 0;
    int curr;
    for (int row = 0; row < N; row++)
    {
        curr = mat[row][col];
        if (curr > *max1)
        {
            *max2 = *max1;
            *max1 = curr;
        }
        else if (curr > *max2)
        {
            *max2 = curr;
        }
    }
}
void* mul_max_nums_in_col(void* p_col){

    char buffer[MAX_BUFFER];
    int val1, val2, col = *(int*)p_col;

    create_msg(buffer,MULTIPLIER,col+1,STARTING);
    atomic_print(buffer);

    // wait for init threads to finish
    pthread_mutex_lock(&mult_lock[col]);
    while (init_count < N){
        pthread_cond_wait(&init_complete, &mult_lock[col]);  
    }
    pthread_mutex_unlock(&mult_lock[col]);


    find_2_max_nums(col, &val1, &val2);
    product_arr[col] = val1 * val2; //write the product to product_arr[j] where j = column index
    // sprintf(buffer, "DEBUG: mult thread: %d, val1: %d, val2: %d",col+1, val1, val2);
    // atomic_print(buffer);

    pthread_mutex_lock(handle_mtx);
    mult_count++;
    if (mult_count == M) {
        //send signal to wake up fac_threads
        for (int i = 0; i < M; ++i){
            pthread_cond_signal(&mult_complete);
        }
    }
    pthread_mutex_unlock(handle_mtx);

    create_msg(buffer,MULTIPLIER,col+1,DONE);
    atomic_print(buffer);

    pthread_exit(NULL);
}

void* factorize(void* p_index){

    char buffer[MAX_BUFFER];
    int fac1, fac2, prod;
    int index = *(int*)p_index;
    
    create_msg(buffer,FACTORIZER,index+1,STARTING);
    atomic_print(buffer);
    
    // wait for mult threads to finish
    pthread_mutex_lock(&fac_lock[index]);
    while (mult_count < M){
        pthread_cond_wait(&mult_complete, &fac_lock[index]);  
    }
    pthread_mutex_unlock(&fac_lock[index]);

    prod = product_arr[index];
    // sprintf(buffer, "DEBUG: fac_thread #%d, PROD: %d", index, prod);
    // atomic_print(buffer);


    set_factors(prod, &fac1, &fac2);
    pthread_mutex_lock(print_mtx);
    write_factors(prod, fac1, fac2);
    pthread_mutex_unlock(print_mtx);
    
    create_msg(buffer,FACTORIZER,index+1,DONE);
    atomic_print(buffer);

    pthread_exit(NULL);
}