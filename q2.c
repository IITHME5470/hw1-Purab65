#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOLERANCE 1e-10

double* compute_eigen_check(int size, double** matrix, double* vector) {
    // Verify if the vector is an eigenvector of the matrix
    int row, col, index;
    double* matrix_vector_product = (double*)malloc(size * sizeof(double));
    if (!matrix_vector_product) {
        fprintf(stderr, "Memory allocation failed for matrix_vector_product.\n");
        return NULL;
    }

    for ( row = 0; row < size; row++) {
        double row_sum = 0;
        for ( col = 0; col < size; col++) {
            row_sum += matrix[row][col] * vector[col];
        }
        matrix_vector_product[row] = row_sum;
    }

    for ( index = 0; index < size - 1; index++) {
        if (fabs(matrix_vector_product[index] * vector[index + 1] - matrix_vector_product[index + 1] * vector[index]) > TOLERANCE) {
            free(matrix_vector_product);
            return NULL;
        }
    }

    return matrix_vector_product;
}

double** load_matrix_from_file(int dimension, const char* filepath) {
    double** matrix = (double**)malloc(dimension * sizeof(double*));
    int i,j;
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error opening matrix file %s.\n", filepath);
        free(matrix);
        return NULL;
    }

    printf("Loading matrix from file %s\n", filepath);
    for (i = 0; i < dimension; i++) {
        matrix[i] = (double*)malloc(dimension * sizeof(double));
        for ( j = 0; j < dimension; j++) {
            fscanf(file, "%lf", &matrix[i][j]);
            fgetc(file); // Skip delimiter
        }
    }
    fclose(file);
    return matrix;
}

double* load_vector_from_file(int dimension, const char* filepath) {
    double* vector = (double*)malloc(dimension * sizeof(double));
    int i;
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error opening vector file %s.\n", filepath);
        free(vector);
        return NULL;
    }

    for ( i = 0; i < dimension; i++) {
        fscanf(file, "%lf", &vector[i]);
        fgetc(file); // Skip delimiter
    }
    fclose(file);
    return vector;
}

int main() {
    // Open and read the matrix dimension from the input file
    FILE* input_file = fopen("./inputfiles/input.in", "r");
    if (!input_file) {
        fprintf(stderr, "Error opening input file ./inputfiles/input.in.\n");
        return EXIT_FAILURE;
    }

    int dimension;
    if (fscanf(input_file, "%d", &dimension) != 1 || dimension <= 0) {
        fprintf(stderr, "Invalid dimension specified in input file.\n");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    fclose(input_file);

    // Load matrix
    char filepath[50];
    sprintf(filepath, "./inputfiles/mat_%06d.in", dimension);
    double** matrix = load_matrix_from_file(dimension, filepath);
    if (!matrix) {
        return EXIT_FAILURE;
    }
    int vector_id;
    int i;
    // Evaluate each vector
    const int vector_count = 4;
    for ( vector_id = 1; vector_id <= vector_count; vector_id++) {
        sprintf(filepath, "./inputfiles/vec_%06d_%06d.in", dimension, vector_id);
        double* vector = load_vector_from_file(dimension, filepath);

        double* result = compute_eigen_check(dimension, matrix, vector);
        if (result) {
            double eigenvalue = 0;
            for ( i = 0; i < dimension; i++) {
                if (vector[i] != 0.0) {
                    eigenvalue = result[i] / vector[i];
                    break;
                }
            }
            printf("vec_%06d_%06d.in : Yes : %lf\n", dimension, vector_id, eigenvalue);
            free(result);
        } else {
            printf("vec_%06d_%06d.in : Not an eigenvector\n", dimension, vector_id);
        }

        free(vector);
    }


    // Free matrix memory
    for ( i = 0; i < dimension; i++) {
        free(matrix[i]);
    }
    free(matrix);
    

    return 0;
}
