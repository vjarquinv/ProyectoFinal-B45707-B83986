# Proyecto Final-Estimación de la Demanda Eléctrica Nacional
## Integrantes: Nathalia Redondo Brenes B45707, Valeria Jarquín Vargas B83986
## Tabla de contenidos
-[Descripción](#descripción)
-[Instalación](#instalación)
-[Uso](#uso)

## Descripción
El proyecto consiste en la predicción de la demanda del día 13 de noviembre de 2023, a partir de la generación y demanda de la semana del 06 al 12 de noviembre 2023, utilizando el modelo de regresión lineal- método de mínimos cuadrados.

## Instalación
Para instalar este proyecto, seguir los siguientes pasos:
1. Clona el repositorio
   ```bash
   git clone https://github.com/vjarquinv/ProyectoFinal-B45707-B83986.git
2. En el caso de no tener gcc para compilar archivos
   ```bash
   sudo apt install gcc
3. Instalar la libreria de Cairo
   ```bash
   sudo apt-get install libcairo2-dev

## Uso
Para utilizar el proyecto una vez lista la instalación, en la terminal:
1. Compila el proyecto con make y se generan los archivos de salida.
  ```bash
  make
2. Con make clean se limpian los archivos generados.
  ```bash
  make clean
