#include <stdio.h>
#include <stdlib.h>

// Function to dynamically allocate a 2D array
double** create2DArray(int rows, int cols)
{   
    int i;
    double** matrix = (double**)malloc(rows * sizeof(double*));
    if (!matrix)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < rows; i++)
    {
        matrix[i] = (double*)malloc(cols * sizeof(double));
        if (!matrix[i])
        {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

// Function to release the allocated memory of a 2D array
void free2DArray(double** matrix, int rows)
{   int i;
    for (i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to save the 2D array to a file
void saveArrayToFile(int size, double** matrix, int format_flag)
{  int i, j;
    FILE* file;
    char filename[50];

    if (format_flag == 1)
    {
        snprintf(filename, sizeof(filename), "array_%06d_bin.out", size);
        file = fopen(filename, "wb");
        if (!file)
        {
            perror("Error opening binary file");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < size; i++)
        {
            fwrite(matrix[i], sizeof(double), size, file);
        }
    }
    else
    {
        snprintf(filename, sizeof(filename), "array_%06d_asc.out", size);
        file = fopen(filename, "w");
        if (!file)
        {
            perror("Error opening ASCII file");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                fprintf(file, "%.15e ", matrix[i][j]);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
}

int main()
{int i,j;
    int n;
    double** array;

    // Read the dimension of the array from the input file
    FILE* inputFile = fopen("input_q1.in", "r");
    if (!inputFile)
    {
        perror("Error reading input file");
        return EXIT_FAILURE;
    }

    if (fscanf(inputFile, "%d", &n) != 1)
    {
        printf("Error: Invalid input format.\n");
        fclose(inputFile);
        return EXIT_FAILURE;
    }
    fclose(inputFile);

    // Allocate memory for the 2D array
    array = create2DArray(n, n);

    // Fill the 2D array with sample data
    for (i = 0; i < n; i++)
    {
        for ( j = 0; j < n; j++)
        {
            array[i][j] = (double)(i + j);
        }
    }

    // Save the array in binary format
    saveArrayToFile(n, array, 1);
    printf("Binary file saved successfully.\n");

    // Save the array in ASCII format
    saveArrayToFile(n, array, 0);
    printf("ASCII file saved successfully.\n");

    // Free allocated memory
    free2DArray(array, n);

    return 0;
}
