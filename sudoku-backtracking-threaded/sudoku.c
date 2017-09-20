#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "stack.h"
#include "semaphore.h"

struct myData{
	unsigned int start_value;
	unsigned int start_ind;
	unsigned int size;
};

int n_threads = 0;

sem_t semaphore;
sem_t semaphoreP;
sem_t semaphoreCont;
Stack s;

unsigned int max_threads = 4;
unsigned int solutions = 0;
pthread_mutex_t s_mutex;
/*
int sudo[] = {
		0,3,0,0,6,0,0,0,5,
		0,0,0,0,5,0,0,0,0,
		0,0,6,1,0,7,9,0,0,
		0,0,5,6,3,9,4,0,0,
		0,0,0,0,0,0,0,0,0,
		9,0,2,0,4,0,0,3,7,
		0,5,0,8,0,3,0,7,0,
		0,0,7,0,0,0,0,0,0,
		4,0,0,0,0,0,0,0,0
	};*/
/*
int sudo[] = {
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		
	};*//*int sudo[] = {
		0,0,0,0,0,3,0,1,7,
		0,1,5,0,0,9,0,0,8,
		0,6,0,0,0,0,0,0,0,
		1,0,0,0,0,7,0,0,0,
		0,0,9,0,0,0,2,0,0,
		0,0,0,5,0,0,0,0,4,
		0,0,0,0,0,0,0,2,0,
		5,0,0,6,0,0,3,4,0,
		3,4,0,2,0,0,0,0,0
		
	};*/
	int sudo[] = {
	 1, 3, 8,15, 9, 0,16, 0, 0, 6, 0, 0,13, 7, 0, 0,
	 0,13, 6, 0, 0,15, 0, 0, 7, 0, 0, 4, 8, 0, 0, 0,
	 2, 0, 0, 0, 0, 0, 0,13,15, 0,10, 0, 0, 3, 9, 0,
	 7, 0, 5, 0, 6, 3, 0, 0, 0,12, 0, 0, 0, 0,14,11,

	 0, 5, 0, 0, 0,11, 8, 0, 0, 0, 0,15, 3, 0, 0, 0,
	 13, 6, 9, 1,10, 8, 5, 7,11, 4, 2, 3,12,14, 15, 16,
	 0, 0,16, 0, 0, 0, 0, 3,12, 0, 0, 8, 0, 6, 7, 0,
	 3, 0, 0,10, 0, 6, 0, 0, 0,13, 1, 0, 0, 0, 0, 8,

	12, 0, 7, 0, 0, 5, 2, 0, 0, 0, 8, 0, 0,15, 1, 0,
	 0, 0, 1, 9, 4, 0, 0, 0, 0,15, 0, 0, 7, 0, 0, 0,
	 0, 8, 2, 0, 3, 0, 0,16, 0,14, 7, 0, 0, 0, 0, 5,
	 0, 7, 0, 0, 0, 0,15, 6, 1, 0, 0,10, 0,12, 8, 0,

	 8, 0, 0,13,16, 0, 0,11, 0, 0,15, 0, 0, 0, 4, 7,
	 0, 9, 0, 0, 0, 0, 7, 0, 0,11, 0,16, 14, 0, 0, 0,
	 0,11, 3, 0,15,12, 0, 0, 8, 0, 0, 0, 0,10, 0, 0,
	 1, 0, 4, 0, 0, 0, 3, 0, 0, 2, 0, 6,16, 0, 0,15
};
/*
	int sudo[] = {
	 0, 0, 0,15, 9, 0,16, 0, 0, 6, 0, 0,13, 7, 0, 0,
	 0,13, 6, 0, 0,15, 0, 0, 7, 0, 0, 4, 8, 0, 0, 0,
	 2, 0, 0, 0, 0, 0, 0,13,15, 0,10, 0, 0, 3, 9, 0,
	 7, 0, 5, 0, 6, 3, 0, 0, 0,12, 0, 0, 0, 0,14,11,

	 0, 5, 0, 0, 0,11, 8, 0, 0, 0, 0,15, 3, 0, 0, 0,
	 0, 6, 0, 1,10, 0, 5, 0,11, 0, 2, 0,12,14, 0, 0,
	 0, 0,16, 0, 0, 0, 0, 3,12, 0, 0, 8, 0, 6, 7, 0,
	 3, 0, 0,10, 0, 6, 0, 0, 0,13, 1, 0, 0, 0, 0, 8,

	12, 0, 0, 0, 0, 5, 2, 0, 0, 0, 8, 0, 0,15, 1, 0,
	 0, 0, 1, 9, 4, 0, 0, 0, 0,15, 0, 0, 7, 0, 0, 0,
	 0, 8, 2, 0, 3, 0, 0,16, 0,14, 7, 0, 0, 0, 0, 5,
	 0, 7, 0, 0, 0, 0,15, 6, 1, 0, 0,10, 0,12, 8, 0,

	 8, 0, 0,13,16, 0, 0,11, 0, 0,15, 0, 0, 0, 4, 7,
	 0, 9, 0, 0, 0, 0, 7, 0, 0,11, 0,16, 14, 0, 0, 0,
	 0,11, 3, 0,15,12, 0, 0, 8, 0, 0, 0, 0,10, 0, 0,
	 1, 0, 4, 0, 0, 0, 3, 0, 0, 2, 0, 6,16, 0, 0,15
};*/

/*	int sudo[] = {
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0,0, 0, 0,
};*/

/*
int sudo[] = {
 0, 1 ,3 ,4, 5, 6, 7, 8,  9, 10, 11, 12,  13, 14, 15, 16, 
 5 ,6 ,7 ,8,1 ,2 ,3 ,4  ,13 ,14 ,15 ,16 , 9 ,10, 11, 12 ,
 9 ,10 ,11 ,12 , 13, 14 ,15, 16,  1 ,2 ,3 ,4 , 5, 6, 7, 8 ,
 13, 14, 15, 16,  9, 10 ,11 ,12  ,5, 6, 7 ,8  ,1 ,2 ,3 ,4 ,
 0 ,2, 4, 3 , 6 ,5, 8, 7 , 10, 9 ,12 ,11 ,14 ,13 ,16 ,15, 
 6 ,5, 8, 7 , 2 ,1, 4, 3  ,14 ,13 ,16 ,15 ,10, 9 ,12, 11 ,
 10, 9 ,12 ,11 , 14, 13, 16, 15, 2, 1, 4, 3,  6, 5, 8, 7, 
 0, 13, 16, 15,  10, 9 ,12 ,11, 6, 5, 8, 0,  2 ,1, 4 ,3, 
 3 ,4 ,1, 2 , 7 ,8, 5 ,6,  11, 0 ,9 ,10, 15, 16, 13, 14, 
 7 ,8 ,5, 6 , 3 ,4, 1 ,0 , 15, 16, 13, 14, 11, 12, 9, 10 ,
 0, 12, 9 ,10 , 15 ,16 ,13, 14 , 3 ,4 ,1 ,2 ,7 ,8 ,5 ,6, 
 15, 16, 13, 14,  11 ,12 ,9 ,0  ,7 ,8 ,5 ,6 ,3 ,4,1, 2, 
 4 ,3 ,2, 1 , 8 ,7 ,6, 0,  12, 11 ,10, 9 , 16, 15, 14 ,13, 
 8 ,7 ,6, 5 , 4 ,0 ,2 ,13 , 16 ,15 ,14 ,1  ,12 ,11 ,10, 9 ,
 0, 15, 10, 9  ,16 ,11 ,14 ,1 , 4 ,7 ,6 ,13 , 8, 3 ,2 ,5 ,
 16, 11, 14, 13 , 12 ,15 ,10 ,9  ,8 ,3 ,2 ,5 , 4, 7, 6, 1 
};

*/

bool valid_in_row(int* array, int x, int index, unsigned int size_sq){
	for(int i = 0; i<size_sq; i++){
		if(array[(index/size_sq)*size_sq + i] == x){
			return false;
		}
	}
	return true;
}

bool valid_in_col(int* array, int x, int index, unsigned int size_sq){
	for(int i = 0; i<size_sq; i++){
		if(array[index%size_sq + i*size_sq] == x){
			return false;
		}
	}
	return true;
}

bool valid_in_sq(int* array, int x, int index, unsigned int size){
	unsigned int size_sq = size*size;
	unsigned int size_cb = size*size_sq;
	for(int i = 0; i<size_sq; i++){
		if(array[((index/size_cb)*size_cb) + ((index%size_sq)/size)*size + i%size + (i/size)*size_sq] == x){
			return false;
		}
	}
	return true;
}

void print_sudoku(int* array, unsigned int size){
	sem_wait(&semaphoreP);
	unsigned int size_sq = size*size;
	unsigned int size_cb = size*size_sq;
	unsigned int size_qd = size*size_cb;
	for(int i=0; i<size_qd; i++) {
			if(i%size_cb == 0){
				printf("|\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
			} else {
				if(i%size_sq == 0){
					printf("|\n");
				}
			}
			if(i%size == 0){
				printf("| ");
			}
			printf("%02i ",array[i]);
		}
	printf("|\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	sem_post(&semaphoreP);
}

void deep_copy(int size, int array[], int array_from[]){
	for(int i=0;i<size;i++){
		array[i] = array_from[i];
	}						
}

void* sudoku_solver(void* args){
	
	Data *stack = (Data*)args;
	unsigned int start_value = stack->start_value;
	unsigned int start_ind = stack->start_ind;
	unsigned int size = stack->size;
	unsigned int size_sq = size*size;
	unsigned int size_cb = size*size_sq;
	unsigned int size_qd = size*size_cb;
	int* sudo_r = malloc(sizeof(int) * size_qd);
	deep_copy(size_qd, sudo_r,stack->state);
	unsigned int ind = start_ind+1;

	

	while((stack->state)[ind]!=0){
		ind++;
	}
		printf("%u \n", ind);
	if(ind >= size_qd-1){
		sem_wait(&semaphoreCont);
		solutions++;
		fflush(stdout);
		printf("%u!!!\n", solutions);
		sem_post(&semaphoreCont);
		sem_post(&semaphore);
		pthread_exit(NULL);
	}

	sudo_r[start_ind] = start_value;

	//print_sudoku(sudo_r, size);
	//printf("Created thread, ind: %u - value: %u\n", start_ind, start_value);

	Data* temp = malloc(sizeof(Data) * size_sq);
	int* temp_sudoku = malloc(sizeof(int) * size_qd);
	deep_copy(size_qd, temp_sudoku,sudo_r);	
	for(int n = 1; n<size_sq+1; n++){
		if(valid_in_row(sudo_r, n, ind, size_sq) & 
			valid_in_col(sudo_r, n, ind, size_sq) & 
			valid_in_sq(sudo_r, n, ind, size)){
			
			temp[n-1].state = temp_sudoku;
			temp[n-1].size = size;
			temp[n-1].start_value = n;
			temp[n-1].start_ind = ind;
			stackpush(&s, &temp[n-1]);
			//printf("%u!\n", s.size);
			fflush(stdout);

		}
	}
	sem_post(&semaphore);
	pthread_exit(NULL);
}

int main(){
	stackinit(&s);
	unsigned int sudo_size = 4;
	sem_init(&semaphore, 0, 1);
	sem_init(&semaphoreCont, 0, 1);
	sem_init(&semaphoreP, 0, 1);
	pthread_mutex_init(&s_mutex, NULL);
	Data datas[sudo_size*sudo_size];
	//printf("Creating threads\n");
	fflush(stdout);
	
	for(int n = 1; n<(sudo_size*sudo_size)+1; n++){
		if(valid_in_row(sudo, n, 0, sudo_size*sudo_size) & 
			valid_in_col(sudo, n, 0, sudo_size*sudo_size) & 
			valid_in_sq(sudo, n, 0, sudo_size)){
			datas[n-1].state = sudo;
			datas[n-1].size = sudo_size;
			datas[n-1].start_value = n;
			datas[n-1].start_ind = 0;
			fflush(stdout);
			stackpush(&s, &datas[n-1]);
		}
	}
	//printf("Done creating initial set!\n");
	fflush(stdout);
	pthread_t* thread_ptr = malloc(sizeof(pthread_t));
	fflush(stdout);
	do {
		sem_wait(&semaphore);
		pthread_t temp_thread;
		thread_ptr = &temp_thread;
		pthread_create(thread_ptr, NULL, sudoku_solver, (void*)stackpop(&s));
		pthread_join(temp_thread, NULL);
		//printf("%u!\n", s.size);
		fflush(stdout);
	} while(!(s.size == 0));
	printf("%u!\n", solutions);
	fflush(stdout);
	pthread_mutex_destroy(&s_mutex);
}









