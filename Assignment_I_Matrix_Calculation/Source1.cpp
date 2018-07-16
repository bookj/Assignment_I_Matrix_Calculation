/******************************************************************************
* FILE: Assignment I - Matrix Calculation Using OpenMP
* PATTERN : 2 - block, block
* DESCRIPTION :
* A[i][j] = B[i][j] * C[i][j];
* AUTHOR : Nattapon Jutamas 5710110137
* *****************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 30			// number of rows and column in matrix
#define BLOCK_SIZE 3	// size of the block

void sub_block(int a[N][N], int b[N][N], int c[N][N], int m[N][N], int tid, int rstart, int cstart) {
	for (int row = rstart; (row < rstart + BLOCK_SIZE) && (row < N); row++) {
		for (int col = cstart; (col < cstart + BLOCK_SIZE) && (col < N); col++) {
			printf("tid:%d process at index(%d,%d)\n", tid, row, col);
			a[row][col] = b[row][col] * c[row][col];
			m[row][col] = tid;
		}
	}
}

int main()
{
	int A[N][N];	// result matrix C
	int B[N][N];	// matrix B to be multiplied
	int C[N][N];	// matrix C to be multiplied
	int matrix_mask[N][N];
	int i, j;

	printf("The equation is A[i][j] = B[i][j] * C[i][j]\n");
	printf("N = %d, Size of the block = %d\n", N, BLOCK_SIZE);

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

	printf("Parallel Process (block, block)\n");
#pragma omp parallel
	{
		int nthreads = omp_get_num_threads();
		int tid = omp_get_thread_num();
		int nblocks;
		int remain_block = N % BLOCK_SIZE;

		if (remain_block) {
			// 0 = all blocks are same size, otherwise last block size = remain_block
			nblocks = (N / BLOCK_SIZE) + 1;
		}
		else {
			nblocks = N / BLOCK_SIZE;
		}
		
		int row, col;
#pragma omp parallel for
		for (row = 0; row < nblocks; row++) {
			int colstart = tid - ((row * nblocks) % nthreads);
			if (colstart < 0) {
				colstart = colstart + nthreads;
			}
			for (col = colstart; col < nblocks; col = col + nthreads) {
				sub_block(A, B, C, matrix_mask, tid, row * BLOCK_SIZE, col * BLOCK_SIZE);
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
	printf("Domain that each thread process(block, block) :\n");
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

