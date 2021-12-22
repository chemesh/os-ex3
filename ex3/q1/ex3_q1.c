
#include "ex3_q1_given.h"


int mat[N][M];          
int product_arr[M];
int main_thread_id;     //id of the main thread

void set_row_in_mat(void* row);

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
        pthread_join(*init_threads[i], NULL);
    }


}
