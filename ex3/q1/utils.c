#include "ex3_q1.h"


void validate(int* irets, int t_count){
    // check for failure in threads execution and exit if happened
    for (int i = 0; i < t_count ; i++){ 
        if (irets[i] != 0){
            char buffer[MAX_BUFFER];
            sprintf(buffer,"error in thread %d, id: %d - return value from pthread_create call is %d", (int)pthread_self(), (int)getpid(), irets[i]);
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

int find_max_in_col(int col){
    int max = mat[0][col];
    int index = 0;
    for (int i = 1; i < N; i++){
        if (mat[i][col] > max){
            max = mat[i][col];
            index = i;
        }
    }
    return index;
}


void* set_row_in_mat(void* p_row) {

    int last = 0, row = *((int*)p_row);
    char buffer[MAX_BUFFER];

    //print starting message
    // create_msg(buffer,INITIALIZER,(int)pthread_self(),STARTING);
    create_msg(buffer,INITIALIZER,(int)getpid(),STARTING);
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
    if (init_count == N - 1){
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
    create_msg(buffer,INITIALIZER,(int)pthread_self(),DONE);
    atomic_print(buffer);

    pthread_exit(NULL);
}

void* mul_max_nums_in_col(void* p_col){

    char buffer[MAX_BUFFER];
    int val1, val2, ind1, ind2, col = *(int*)p_col;


    create_msg(buffer,MULTIPLIER,(int)pthread_self(),STARTING);
    atomic_print(buffer);

    // wait for init threads to finish
    // pthread_mutex_lock(&mult_lock);
    while (init_count < N-1){
        pthread_cond_wait(&init_complete, &mult_lock);  
    }
    ind1 = find_max_in_col(col); //serach for the max number in the column
    val1 = mat[ind1][col];
    mat[ind1][col] = -1;    //temporarily remove the max num from mat

    ind2 = find_max_in_col(col); //serach for the 2nd max number in the column
    val2 = mat[ind2][col];
    mat[ind1][col] = val1;

    product_arr[col] = val1 * val2; //write the product to product_arr[j] where j = column index

    pthread_mutex_lock(handle_mtx);
    mult_count++;
    if (mult_count == M - 1) {
        //send signal to wake up fac_threads
        for (int i = 0; i < M; ++i){
            pthread_cond_signal(&mult_complete);
        }
    }
    pthread_mutex_unlock(handle_mtx);

    create_msg(buffer,MULTIPLIER,(int)pthread_self(),DONE);
    atomic_print(buffer);

    pthread_exit(NULL);
}

void* factorize(void* p_index){

    char buffer[MAX_BUFFER];
    int fac1, fac2, prod = product_arr[*(int*)p_index];
    
    // wait for mult threads to finish
    // pthread_mutex_lock(&fac_lock);
    while (mult_count < M-1){
        pthread_cond_wait(&mult_complete, &fac_lock);  
    }

    create_msg(buffer,FACTORIZER,(int)pthread_self(),STARTING);
    atomic_print(buffer);

    set_factors(prod, &fac1, &fac2);
    pthread_mutex_lock(print_mtx);
    write_factors(prod, fac1, fac2);
    pthread_mutex_unlock(print_mtx);

    create_msg(buffer,FACTORIZER,(int)pthread_self(),DONE);
    atomic_print(buffer);

    pthread_exit(NULL);
}