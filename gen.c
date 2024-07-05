// Proyecto Final -- Datos Generacion y Demanda -- B45707 - B83986

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pred_dem.h"


#define ROWS 24
#define COLS 7


// Funcion para leer un archivo CSV
void read_csv(const char *filename, dataGen **data, int *count) {
    // Se abre el archivo para leerlo
    FILE *file = fopen(filename, "r");
    // Verifica que fopen es igual a NULL hubo un error abriendo el archivo
    if (file == NULL) {
        fprintf(stderr, "Error abriendo el archivo: %s\n", filename);
        return;
    }
    // Para leer cada linea del archivo
    char line[256];
    // Se inicializa el tamaño del array
    int capacity = 100;
    // Se almacena memoria 
    *data = malloc(capacity * sizeof(dataGen));
    *count = 0;

    // Se lee cada linea del archivo con fgets
    while (fgets(line, sizeof(line), file)) {
        // Si count alcanza la capacidad la duplica
        if (*count >= capacity) {
            capacity *= 2;
            // Con realloc se aumenta el array de data
            *data = realloc(*data, capacity * sizeof(dataGen));
        }
        // Se divide la linea con strtok
        char *token = strtok(line, ",");
        // La primera parte de la linea se guarda en hour
        strcpy((*data)[*count].hour, token);

        token = strtok(NULL, ",");
        // La segunda parte de la linea se guarda en dataMW y se convierte en double
        (*data)[*count].dataMW = atof(token);

        (*count)++; // Se incrementa hasta leer todo el archivo
    }
    fclose(file); // Se cierra el archivo
}

// Funcion para leer multiples archivos CSV y almacenarlos
void read_multiple_csv(const char **filenames, int num_files, fileData *files_data) {
    // Bucle para recorrer los archivos en el array
    for (int i = 0; i < num_files; i++) {
        // Se inicializa la estructura
        files_data[i].data = NULL;
        files_data[i].count = 0;
        // Guarda el nombre del archivo actual con su data correspondiente
        strcpy(files_data[i].filename, filenames[i]);
        // llama la funcion que lee archivos CSV
        read_csv(filenames[i], &files_data[i].data, &files_data[i].count);
    }
}

// Funcion para llenar matriz con la segunda columna de los datos de generacion
void data_matrix(const fileData *files_data, int num_files, double matrix[ROWS][COLS]){
    // Se recorre las columnas de los archivos
    for (int col = 0; col < num_files; col++) {
        // Se recorre las filas de la matriz 
        for (int row = 0; row < ROWS; row++) {
            // Se verifica si hay datos suficientes en el archivo
            if (row < files_data[col].count) {
                // Se guarda el valor de MW en la matriz
                matrix[row][col] = files_data[col].data[row].dataMW;
            }
            else{
                // En el caso que hayan datos insuficientes lo rellena con cero
                matrix[row][col] = 0.0;
            }
        }
    }
}

// Funcion para la regresion lineal
void linear_reg(double *x, double *y, int n, double *a, double *b){
    // Se declaran las variables para las sumatorias
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
    // Se recorre los datos dados y acumula los datos de las sumatorias
    for(int i = 0; i < n; i++){
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i]*y[i];
        sum_x2 += x[i]*x[i];
    }
    // Se calcula el coeficiente b
    *b = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    // Se calcula el coeficiente a
    *a = (sum_y - (*b) * sum_x) / n;
}

// Funcion para predecir la demanda 
void predict_dem(double matrix_gen[ROWS][COLS], double matrix_dem[ROWS][COLS], double predictions[ROWS]){
    // Se recorren las 24 horas
    for(int hour = 0; hour < 24; hour++){
        // Se inicializa los coeficientes a y b
        double a, b;
        // Arrays temporales para almacenar los datos de generacion y demanda en la hora actual
        double gen[COLS], dem[COLS];
        // Se extraen los datos de la matrices originales 
        for (int day = 0; day < 7; day++){
            gen[day] = matrix_gen[hour][day];
            dem[day] = matrix_dem[hour][day];
        }
        /*Se calcula la regresion lineal pasando los arrays de generacion y demanda, la cantidad de dias
        y los punteros *a y *b para calcular los coeficientes */ 
        linear_reg(gen, dem, 7, &a, &b);
        /*Se calcula la demanda con la formula de la recta de regresion , donde se usa la matriz_gen del ultimo
        dia para hacer la predicion y se guardan los datos en un array */
        predictions[hour] = a + b * matrix_gen[hour][7];
    }

}



