// Proyecto Final -- Datos Generacion -- B45707 - B83986

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicion de la estructura de los datos de los archivos .csv
typedef struct dataGen{
    char hour[10]; // array de caracteres para almacenar la hora
    double dataMW; // almacena los MW asociados a la hora
} dataGen;

// Definicion de la estructura para almacenar los datos de cada archivo
typedef struct fileData {
    char filename[256]; // array de caracteres para almacenar los nombres de los archivos
    dataGen *data;  // 
    int count;
} fileData;

// Funcion para leer un archivo CSV
void read_csv(const char *filename, dataGen **data, int *count) {
    // Se abre el archivo para leerlo
    FILE *file = fopen(filename, "r");
    // Verifica que fopen es igual a NULL hubo un error abriendo el archivo
    if (file == NULL) {
        perror("Error abriendo el archivo");
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

int main() {
    // Se define la lista de archivoss por leer
    const char *filenames[] = {"G-06112023.csv", "G-07112023.csv","G-08112023.csv", "G-09112023.csv", "G-10112023.csv", "G-11112023.csv","G-12112023.csv"};
    // La cantidad de archivos
    int num_files = sizeof(filenames) / sizeof(filenames[0]);

    // Arreglo de estructuras para almacenar los datos de los archivos
    fileData *files_data = malloc(num_files * sizeof(fileData));

    // Se llama la funcion para leer multiples archivos CSV
    read_multiple_csv(filenames, num_files, files_data);

    // Abrir archivo de salida para hacer prueba que se realiza el registro de los datos
    FILE *test_file = fopen("prueba_salida.txt", "w");
    if (test_file == NULL) {
        perror("Error abriendo el archivo de salida");
        free(files_data);
        return 1;
    }

    // Escribir los datos en el archivo de salida
    for (int i = 0; i < num_files; i++) {
        fprintf(test_file, "Datos del archivo: %s\n", files_data[i].filename);
        for (int j = 0; j < files_data[i].count; j++) {
            fprintf(test_file, "Hour: %s, Number: %f\n", files_data[i].data[j].hour, files_data[i].data[j].dataMW);
        }
        fprintf(test_file, "\n");
        free(files_data[i].data); // Liberar la memoria para cada archivo
    }

    fclose(test_file); // Cerrar el archivo de salida
    // Se libera la memoria del arreglo
    free(files_data);

    return 0;
}