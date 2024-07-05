// Proyecto Final -- Grafica -- B45707 -- B83986

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>
#include "pred_dem.h"

#define WIDTH 800
#define HEIGHT 600


void read_dem_csv(const char *filename_dem, DataDem **data, size_t *size) {
    // Se abre el archivo para leerlo
    FILE *file = fopen(filename_dem, "r");
    // Verifica que fopen es igual a NULL hubo un error abriendo el archivo
    if (file == NULL) {
        perror("Error abriendo el archivo\n");
        return;
    }
    // Se inicializa el tamaño del array
    size_t capacity = 10;
    // Se almacena memoria 
    *data = malloc(capacity * sizeof(DataDem));
    // Se inicializa la cantidad de datos leidos
    *size = 0;

    // Se lee cada linea del archivo
    while (fscanf(file, "%lf,%lf,%lf", &(*data)[*size].col1, &(*data)[*size].col2, &(*data)[*size].col3) == 3) {
        (*size)++;
        if (*size >= capacity) {
            capacity *= 2;
            // Cada dato se almacena en la estructura DataDem
            *data = realloc(*data, capacity * sizeof(DataDem));
        }
    }

    fclose(file); // Se cierra el archivo
}

// Funcion para graficar la demanda real y predicha
void plot_graph(DataDem *data, size_t size) {

    // se inicializa la superficie donde se va a dibujar
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
    cairo_t *cr = cairo_create(surface);

    // Se crea el fondo blanco para la imagen
    cairo_set_source_rgb(cr, 1, 1, 1); // White color
    cairo_paint(cr);

    // Se crea la cuadrilla en un color gris claro
    cairo_set_source_rgb(cr, 0.7, 0.7, 0.7); // Light grey color
    cairo_set_line_width(cr, 1.0);
    for (int i = 50; i <= WIDTH - 50; i += 50) {
        cairo_move_to(cr, i, HEIGHT - 50);
        cairo_line_to(cr, i, 50);
        cairo_stroke(cr);
    }
    for (int i = 50; i <= HEIGHT - 50; i += 50) {
        cairo_move_to(cr, 50, i);
        cairo_line_to(cr, WIDTH - 50, i);
        cairo_stroke(cr);
    }

    // Se crea el eje Y
    cairo_set_source_rgb(cr, 0, 0, 0); // Black color
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, 50, HEIGHT - 50);
    cairo_line_to(cr, 50, 50);
    cairo_stroke(cr);

    // Se crea el eje X
    cairo_move_to(cr, 50, HEIGHT - 50);
    cairo_line_to(cr, WIDTH - 50, HEIGHT - 50);
    cairo_stroke(cr);

    // Se agrega las etiquetas a los ejes, se define tipo de letra y tamaño
    cairo_set_source_rgb(cr, 0, 0, 0); // Black color
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);

    // Se agrega el nombre del eje X (horas)
    for (int i = 0; i <= 24; i += 2) {
        char label[4];
        snprintf(label, sizeof(label), "%d", i);
        cairo_move_to(cr, 50 + i * (WIDTH - 100) / 24.0 - 10, HEIGHT - 35);
        cairo_show_text(cr, label);
    }

    // Se agrega el nombre del eje Y (MW)
    for (int i = 1000; i <= 1800; i += 100) {
        char label[5];
        snprintf(label, sizeof(label), "%d", i);
        cairo_move_to(cr, 15, HEIGHT - 50 - (i - 1000) * 0.4);
        cairo_show_text(cr, label);
    }

    // Se grafican los datos
    cairo_set_line_width(cr, 2.0);

    // Se grafica los datos de demanda real (columna 2)
    cairo_set_source_rgb(cr, 0, 0, 1); // Blue color
    cairo_move_to(cr, 50, HEIGHT - 50 - (data[0].col2 - 1000) * 0.4);
    for (size_t i = 1; i < size; i++) {
        double x = 50 + data[i].col1 * (WIDTH - 100) / 24.0;
        double y = HEIGHT - 50 - (data[i].col2 - 1000) * 0.4;
        cairo_line_to(cr, x, y);
    }
    cairo_stroke(cr);

    // Se grafica los datos de demanda predicha (columna 3)
    cairo_set_source_rgb(cr, 1, 0, 0); // Red color
    cairo_move_to(cr, 50, HEIGHT - 50 - (data[0].col3 - 1000) * 0.4);
    for (size_t i = 1; i < size; i++) {
        double x = 50 + data[i].col1 * (WIDTH - 100) / 24.0;
        double y = HEIGHT - 50 - (data[i].col3 - 1000) * 0.4;
        cairo_line_to(cr, x, y);
    }
    cairo_stroke(cr);

    // Se añade el titulo 
    cairo_set_source_rgb(cr, 0, 0, 0); // Black color
    cairo_set_font_size(cr, 20);
    cairo_move_to(cr, WIDTH / 2 - 50, 30);
    cairo_show_text(cr, "Generación día 13/11/2023");

    // Se añade el nombre del eje X
    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, WIDTH / 2 - 50, HEIGHT - 10);
    cairo_show_text(cr, "Hora");

    // Se añade el nombre del eje Y
    cairo_save(cr);
    cairo_move_to(cr, 10, HEIGHT / 2);
    cairo_rotate(cr, -M_PI / 2);
    cairo_show_text(cr, "Generación (MWh)");
    cairo_restore(cr); // Restaurar el estado del contexto 

    // Se añade una leyenda
    cairo_set_source_rgb(cr, 0, 0, 1); // Blue color
    cairo_rectangle(cr, WIDTH - 150, 50, 10, 10);
    cairo_fill(cr);
    cairo_move_to(cr, WIDTH - 130, 60);
    cairo_show_text(cr, "13/11/2023 Real");

    cairo_set_source_rgb(cr, 1, 0, 0); // Red color
    cairo_rectangle(cr, WIDTH - 150, 70, 10, 10);
    cairo_fill(cr);
    cairo_move_to(cr, WIDTH - 130, 80);
    cairo_show_text(cr, "13/11/2023 Predicción");

    // Se guarda el resultado en un archivo .png
    cairo_surface_write_to_png(surface, "pred_demanda.png");

    // Se liberan los recursos de cairo
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}
