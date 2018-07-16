/******************************************************************************
* FILE: Assignment I - Matrix Calculation Using OpenMP
* PATTERN : cyclic, cyclic
* DESCRIPTION :
* A[i][j] = B[i][j] * C[i][j];
* AUTHOR : Nattapon Jutamas 5710110137
* *****************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 31		// number of rows and column in matrix

int main()
{
	int A[N][N];	// result matrix C
	int B[N][N];	// matrix B to be multiplied
	int C[N][N];	// matrix C to be multiplied
	int matrix_mask[N][N];
	int i, j;

	printf("The equation is A[i][j] = B[i][j] * C[i][j]\n");
	printf("N = %d\n", N);

	// Initialize matrices
	// matrix A
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			A[i][j] = 0;
		}
	}
		
	// matrix B
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			B[i][j] = (i + j) % 10;
		}
	}
		
	// matrix C
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			C[i][j] = (i * j) % 10;
		}
	}
	
	// matrix mask
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_mask[i][j] = 9;
		}	
	}

	printf("******************************************************\n");
	printf("Input Matrix B :\n");
	printf("B[%d][%d]\n", N, N);
	for (i = 0; i < N; i++) {
		printf("Row:%02d\n", i);
		for (j = 0; j < N; j++) {
			if (j == 50) {
				printf("\n");
			}
			printf("%2d ", B[i][j]);

		}
		printf("\n");
	}
	printf("******************************************************\n");

	printf("******************************************************\n");
	printf("Input Matrix C :\n");
	printf("C[%d][%d]\n", N, N);
	for (i = 0; i < N; i++) {
		printf("Row:%02d\n", i);
		for (j = 0; j < N; j++) {
			if (j == 50) {
				printf("\n");
			}
			printf("%2d ", C[i][j]);

		}
		printf("\n");
	}
	printf("******************************************************\n");

	printf("Parallel Process (cyclic, cyclic)\n");
#pragma omp parallel
	{
		int nthreads = omp_get_num_threads();
		int tid = omp_get_thread_num();
		int row, col;
#pragma omp parallel for
		for (row = 0; row < N; row++) {
			int colstart = tid - ((row * N) % nthreads);
			if (colstart < 0) {
				colstart = colstart + nthreads;
			}
			for (col = colstart; col < N; col = col + nthreads) {
				
				//printf("tid:%d process at index(%d,%d)\n", tid, row, col);
				A[row][col] = B[row][col] * C[row][col];
				matrix_mask[row][col] = tid;
			}
		}
	}

	// Print results
	printf("End Parallel\n");
	printf("******************************************************\n");
	printf("Result Matrix A :\n");
	printf("A[%d][%d]\n", N, N);
	for (i = 0; i < N; i++) {
		printf("Row:%02d\n", i);
		for (j = 0; j < N; j++) {
			if (j == 50) {
				printf("\n");
			}
			printf("%2d ", A[i][j]);
			
		}
		printf("\n");
	}
	printf("******************************************************\n");

	printf("******************************************************\n");
	printf("Domain that each thread process(cyclic, cyclic) :\n");
	printf("matrix_mask[%d][%d]\n", N, N);
	for (int x = 0; x < N; x++) {
		for (int y = 0; y < N; y++) {
			printf("%d ", matrix_mask[x][y]);
		}
		printf("\n");
	}
	printf("******************************************************\n");

	printf("Done.\n");
	system("PAUSE");
	return 0;
}