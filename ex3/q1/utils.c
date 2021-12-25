#include "ex3_q1.h"


void validate(int* irets, int t_count){
    // check for failure in threads execution and exit if happened
    for (int i = 0; i < t_count ; i++){ 
        if (irets[i] != 0){
            perror("Error in thread creation: ");
            exit(-1);
        }
    }
}

void atomic_print(const char* msg){
    pthread_mutex_lock(print_mtx);
    print_msg(msg);
    pthread_mutex_unlock(print_mtx);
}


void* set_row_in_mat(void* p_row) {

    int static p_count = 0;
    int last = 0, row = *((int*)p_row);
    char buffer[MAX_BUFFER];

    //print starting message
    create_msg(buffer,INITIALIZER,(int)pthread_self(),STARTING);
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
    p_count++;
    if (p_count == M - 1){
        last = 1;
        //send signal to wake up mult_threads
        pthread_cond_broadcast(&init_complete);
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

    create_msg(buffer,MULTIPLIER,(int)pthread_self(),STARTING);
    atomic_print(buffer);

    // wait for init threads to finish
    pthread_cond_wait(&init_complete, &mult_lock);





    create_msg(buffer,MULTIPLIER,(int)pthread_self(),DONE);
    atomic_print(buffer);

    pthread_exit(NULL);

}

void* factorize(void* p_index){

    pthread_exit(NULL);
}