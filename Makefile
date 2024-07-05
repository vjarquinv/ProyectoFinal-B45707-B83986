# Proyecto Final -- Makefile Datos Generacion y Demanda -- B45707 - B83986

# Variables de compilador y banderas
CC = gcc
CFLAGS = -Wall -I/usr/include/cairo
LIBS = -lcairo

# Nombre del ejecutable final
EXEC = main

# Archivos de origen
SRCS = gen.c main.c graph.c

# Archivos objeto generados a partir de los archivos de origen
OBJS = $(SRCS:.c=.o)

# Regla que compila y enlaza el ejecutable, luego ejecuta el programa
all: $(EXEC)
	./$(EXEC)

# Regla para compilar el ejecutable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Regla para compilar archivos .c en archivos .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJS) $(EXEC) datos_demanda.csv pred_demanda.png

# Marcado de dependencias
gen.o: gen.c pred_dem.h
main.o: main.c pred_dem.h
graph.o: graph.c pred_dem.h

.PHONY: all clean