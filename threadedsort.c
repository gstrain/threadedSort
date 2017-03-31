#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *array;
int size;
int *array2;

struct thread_info {
	int start;
	int mid;
	int num;
};

void *merge(void *param) {
	struct thread_info *data = param;
	int i = 0;
	int j = data->mid;
	int k = 0;
	while (i < data->mid && j < data->num) {
		if (array[i] <= array[j]) {
			array2[k] = array[i];
			i++;
		} else {
			array2[k] = array[j];
			j++;
		}
		k++;
	}
	if (i < data->mid) {
		for (int p = i; p < data->mid; p++) {
			array2[k++] = array[p];
		}
	} else {
		for (int p = j; p < data->num; p++) {
			array2[k++] = array[p];
		}
	}
	pthread_exit(NULL);
	return NULL;
}

int cmpfunc(const void * a, const void * b) {
		return ( *(int*)a - *(int *)b );
}

// Thread method call
void *quicksort(void *param) {
	struct thread_info *data = param;
	qsort(array + data->start, data->num, sizeof(int), cmpfunc);
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char *argv[]) {
	size = 10;
	array = (int *)malloc(size * sizeof(int));

	pthread_t t1, t2, t3;
	pthread_attr_t attr;

	FILE *input;
	input = fopen(argv[1], "r");

	FILE *output;
	output = fopen(argv[2], "w");

	if (argc == 3) {
		if (input != NULL && output != NULL) {
			int i = 0;
			int num;
			while (fscanf(input, "%d", &num) > 0) {
				if (i >= size) {
					size += 1;
					array = (int *) realloc(array, size * sizeof(int));
				}
				array[i++] = num;
			}
			fclose(input);

			array2 = (int *)malloc(size * sizeof(int));

			struct thread_info thread1;
			struct thread_info thread2;

			thread1.start = 0;
			thread1.num = size / 2;
			thread2.start = size/2;
			thread2.num = size / 2;
			thread1.mid = NULL;
			thread2.mid = NULL;

			pthread_attr_init(&attr);
			pthread_create(&t1, &attr, quicksort, (void *) &thread1);
			pthread_create(&t2, &attr, quicksort, (void *) &thread2);
			pthread_join(t1, NULL);
			pthread_join(t2, NULL);

			struct thread_info mthread;
			mthread.start = 0;
			mthread.num = size;
			mthread.mid = size / 2;

			pthread_create(&t3, &attr, merge, (void *) &mthread);
			pthread_join(t3, NULL);


			//print the final array to the file
			for (i = 0; i < size; i++) {
				fprintf(output, "%d\n", array2[i]);
			}
			free(array);
			free(array2);
			fclose(output);
		} else {
			printf("Please enter valid input and output files.\n");
			return -1;
		}
	} else {
		printf("Enter text files to match format: ./sort input.txt output.txt\n");
		return -1;
	}

}
