// Proyecto Final -- Interfaz de predicion de demanda -- B45707 - B83986

#ifndef PRED_DEM
#define PRED_DEM

#define ROWS 24
#define COLS 7
#define WIDTH 800
#define HEIGHT 600

// Definición de la estructura de los datos de los archivos .csv
typedef struct dataGen {
    char hour[10]; // Array de caracteres para almacenar la hora
    double dataMW; // Almacena los MW asociados a la hora
} dataGen;

// Definición de la estructura para almacenar los datos de cada archivo
typedef struct fileData {
    char filename[256]; // Array de caracteres para almacenar los nombres de los archivos
    dataGen *data;  // Puntero a un array de estructuras dataGen
    int count; // Número de entradas de datos en el archivo
} fileData;

// Definición de la estructura de los datos del archivo .csv para la demanda
typedef struct {
    double col1; // Almacena los datos de la columna 1
    double col2; // Almacena los datos de la columna 2
    double col3; // Almacena los datos de la columna 3
} DataDem;

// Declaraciones de funciones

// Funcion para leer un archivo CSV
void read_csv(const char *filename, dataGen **data, int *count);

// Funcion para leer multiples archivos CSV y almacenarlos
void read_multiple_csv(const char **filenames, int num_files, fileData *files_data);

// Funcion para llenar matriz con la segunda columna de los datos de generacion
void data_matrix(const fileData *files_data, int num_files, double matrix[ROWS][COLS]);

// Funcion para la regresion lineal
void linear_reg(double *x, double *y, int n, double *a, double *b);

// Funcion para predecir la demanda
void predict_dem(double matrix_gen[ROWS][COLS], double matrix_dem[ROWS][COLS], double predictions[ROWS]);

// Funcion para leer un archivo CSV de demanda
void read_dem_csv(const char *filename_dem, DataDem **data, size_t *size);

// Funcion para graficar la demanda real y predicha
void plot_graph(DataDem *data, size_t size);

#endif