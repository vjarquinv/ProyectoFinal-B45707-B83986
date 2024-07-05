
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo.h>
#include <math.h>
#include "pred_dem.h"

int main() {
    // Se define la primera lista de archivos por leer
    const char *filenames1[] = {"G-06112023.csv", "G-07112023.csv", "G-08112023.csv", "G-09112023.csv", "G-10112023.csv", "G-11112023.csv", "G-12112023.csv"};
    int num_gen = sizeof(filenames1) / sizeof(filenames1[0]);

    // Arreglo de estructuras para almacenar los datos de los archivos
    fileData *files_gen = malloc(num_gen * sizeof(fileData));

    // Se llama la funcion para leer multiples archivos CSV
    read_multiple_csv(filenames1, num_gen, files_gen);

    // Definir la primera matriz 24x7
    double matrix_gen[24][7] = {0};

    // Llenar la primera matriz con los datos de generacion
    data_matrix(files_gen, num_gen, matrix_gen);

    // Se define la segunda lista de archivos por leer
    const char *filenames2[] = {"D-06112023.csv", "D-07112023.csv", "D-08112023.csv", "D-09112023.csv", "D-10112023.csv", "D-11112023.csv", "D-12112023.csv"};
    int num_dem= sizeof(filenames2) / sizeof(filenames2[0]);

    // Arreglo de estructuras para almacenar los datos de los archivos
    fileData *files_dem = malloc(num_dem * sizeof(fileData));

    // Se llama la funcion para leer multiples archivos CSV
    read_multiple_csv(filenames2, num_dem, files_dem);

    // Definir la segunda matriz 24x7
    double matrix_dem[24][7] = {0};

    // Llenar la primera matriz con los datos de generacion
    data_matrix(files_dem, num_dem, matrix_dem);

    double predictions[24] = {0};
    predict_dem(matrix_gen, matrix_dem, predictions);

    // Leer el archivo "D-13112023.csv" que es la demanda real
    const char *file_dem_real = "D-13112023.csv";
    dataGen *data_real = NULL;
    int count_real = 0;

    read_csv(file_dem_real, &data_real, &count_real);


    // Abrir archivo de salida para hacer prueba que se realiza el registro de los datos
    FILE *test_file = fopen("datos_demanda.csv", "w");
    if (test_file == NULL) {
        perror("Error abriendo el archivo de salida");
        free(files_gen);
        free(files_dem);
        free(data_real);
        return 1;
    }

    // Escribir la segunda matriz en el archivo de salida
    for (int hour = 0; hour < 24; hour ++) {
            fprintf(test_file, "%02d, %f, %f\n", hour, data_real[hour].dataMW,predictions[hour]);
    }
    fclose(test_file); // Cerrar el archivo de salida

    // Liberar memoria

    for (int i = 0; i < num_gen; i++) {
        free(files_gen[i].data);
    }
    free(files_gen);

    for (int i = 0; i < num_dem; i++) {
        free(files_dem[i].data);
    }
    free(files_dem);
    free(data_real);

    // Se llaman el puntero de los datos y el tamaño 
    DataDem *data;
    size_t size;

    // Se llama la funcion para leer el archivo
    read_dem_csv("datos_demanda.csv", &data, &size);
    // Se llama la funcion que grafica
    plot_graph(data, size);

    free(data);

    return EXIT_SUCCESS;

}