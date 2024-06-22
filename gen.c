// Proyecto Final -- Datos Generacion -- B45707 - B83986

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicion de la estructura de los datos de los archivos .csv
typedef struct dataGen{
    char hour[10]; // array de caracteres para almacenar la hora
    double dataMW; // almacena los MW asociados a la hora
} dataGen;

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

int main() {
    // Se define el nombre del archivo
    const char *filename = "G-06112023.csv";

    dataGen *data;
    int count;
    // Se llama la funcion read_csv
    read_csv(filename, &data, &count);
    // Se imprime los datos leidos, como prueba de que se esta leyendo el archivo
    for (int i = 0; i < count; i++) {
        printf("Hour: %s, Number: %f\n", data[i].hour, data[i].dataMW);
    }
    // Se libera la memoria
    free(data);

    return 0;
}

