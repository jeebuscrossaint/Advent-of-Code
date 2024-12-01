#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

__global__ void calculateDistances(int *left, int *right, int *distances,
                                   int n) {
  int idx = threadIdx.x + blockDim.x * blockIdx.x;
  if (idx < n) {
    distances[idx] = abs(left[idx] - right[idx]);
  }
}

void sort(int *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

void checkCudaError(cudaError_t err, const char *msg) {
  if (err != cudaSuccess) {
    fprintf(stderr, "CUDA Error: %s: %s\n", msg, cudaGetErrorString(err));
    exit(EXIT_FAILURE);
  }
}

int main() {
  FILE *file = fopen("input.txt", "r");
  if (file == NULL) {
    printf("Error opening file\n");
    return 1;
  }

  int left[1000], right[1000];
  int n = 0;

  while (fscanf(file, "%d %d", &left[n], &right[n]) != EOF) {
    n++;
  }
  fclose(file);

  // Debugging: Print the input arrays
  printf("Left array:\n");
  for (int i = 0; i < n; i++) {
    printf("%d ", left[i]);
  }
  printf("\nRight array:\n");
  for (int i = 0; i < n; i++) {
    printf("%d ", right[i]);
  }
  printf("\n");

  sort(left, n);
  sort(right, n);

  // Debugging: Print the sorted arrays
  printf("Sorted left array:\n");
  for (int i = 0; i < n; i++) {
    printf("%d ", left[i]);
  }
  printf("\nSorted right array:\n");
  for (int i = 0; i < n; i++) {
    printf("%d ", right[i]);
  }
  printf("\n");

  int *d_left, *d_right, *d_distances;
  int *distances = (int *)malloc(n * sizeof(int));

  checkCudaError(cudaMalloc((void **)&d_left, n * sizeof(int)),
                 "Allocating d_left");
  checkCudaError(cudaMalloc((void **)&d_right, n * sizeof(int)),
                 "Allocating d_right");
  checkCudaError(cudaMalloc((void **)&d_distances, n * sizeof(int)),
                 "Allocating d_distances");

  checkCudaError(
      cudaMemcpy(d_left, left, n * sizeof(int), cudaMemcpyHostToDevice),
      "Copying left to d_left");
  checkCudaError(
      cudaMemcpy(d_right, right, n * sizeof(int), cudaMemcpyHostToDevice),
      "Copying right to d_right");

  int blockSize = 256;
  int numBlocks = (n + blockSize - 1) / blockSize;
  calculateDistances<<<numBlocks, blockSize>>>(d_left, d_right, d_distances, n);
  checkCudaError(cudaGetLastError(), "Kernel launch");
  checkCudaError(cudaDeviceSynchronize(), "Kernel execution");

  checkCudaError(cudaMemcpy(distances, d_distances, n * sizeof(int),
                            cudaMemcpyDeviceToHost),
                 "Copying distances to host");

  // Debugging: Print the distances array
  printf("Distances array:\n");
  for (int i = 0; i < n; i++) {
    printf("%d ", distances[i]);
  }
  printf("\n");

  int totalDistance = 0;
  for (int i = 0; i < n; i++) {
    totalDistance += distances[i];
  }

  printf("Total Distance: %d\n", totalDistance);

  cudaFree(d_left);
  cudaFree(d_right);
  cudaFree(d_distances);
  free(distances);

  return 0;
}
