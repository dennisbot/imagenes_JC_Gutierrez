# Trabajo de Lectura y escritura de archivos BMP
En este trabajo se muestra el proceso de lectura y escritura de archivos DIB (Device Independent Bitmap), el archivo bmp en particular, haciendo uso de sus respectivos formatos es decir 54 bytes para la sección del header y si la profundidad es distinto de 24 bits se tendrá una sección adicional para la paleta de colores sino se continua con el resto que corresponde a los datos de la matriz de la imagen bmp.
# Trabajo de Ecualización usando Histogramas de frecuencias
En esta parte del trabajo se utiliza una matriz de histogramas y otra de histograma acumulado realizando luego la formula por cada pixel para obtener nuestra imagen ecualizada (balanceada) para poder visualizar por ejem un mejor contraste.

#Instrucciones
el punto de entrada de la aplicación se encuentra en el archivo **main.cpp**, se uso la librería CImg.h para mostrar las imagenes luego de la operación de la ecualización para las distintas profundidades e.g 1, 4, 8 y 24.