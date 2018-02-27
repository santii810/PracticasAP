#define IMAGE_NUMBER  2
#define BLOCK_SIZE 16

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

void sustituirBloque(int filaBloque1, int colBloque1, IplImage* imagen1, int filaBloque2, int colBloque2, IplImage* imagen2);
int compararBloques(int filaBloque1, int colBloque1, IplImage* imagen1, int filaBloque2, int colBloque2, IplImage* imagen2);
void crearMosaico(IplImage* image1, IplImage* image2);

int main(int argc, char **argv) {

    if (argc != (IMAGE_NUMBER + 1)) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* image1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* image2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find the image file
    if (!image1 || !image2) {
        printf("Error: file %s or %s not found\n", argv[1], argv[2]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow("Inicio", CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage("Inicio", image1);
    cvWaitKey(0);

    crearMosaico(image1, image2);


    // crea y muestras las ventanas con las imagenes
    cvNamedWindow("Resultado", CV_WINDOW_AUTOSIZE);
    cvShowImage("Resultado", image1);
    cvWaitKey(0);


    cvDestroyWindow("Inicio");
    cvDestroyWindow("Inicio");
    // crea y muestras las ventanas con las imagenes
    cvNamedWindow("Resultado", CV_WINDOW_AUTOSIZE);
    cvShowImage("Resultado", image1);

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&image1);
    cvReleaseImage(&image2);

    // Self-explanatory
    cvDestroyWindow("Resultado");

    return EXIT_SUCCESS;
}

void sustituirBloque(int filaBloque1, int colBloque1, IplImage* imagen1, int filaBloque2, int colBloque2, IplImage* imagen2) {
    int fila, columna;
    for (fila = 0; fila < BLOCK_SIZE; fila++) {
        unsigned char *pImg1 = (unsigned char *) imagen1->imageData + ((filaBloque1 + fila) * imagen1->widthStep) + (colBloque1 * 3);
        unsigned char *pImg2 = (unsigned char *) imagen2->imageData + ((filaBloque2 + fila) * imagen2->widthStep) + (colBloque2 * 3);

        for (columna = 0; columna < BLOCK_SIZE; columna++) {
            *pImg1++ = *pImg2++;
            *pImg1++ = *pImg2++;
            *pImg1++ = *pImg2++;
        }
    }
}

int compararBloques(int filaBloque1, int colBloque1, IplImage* imagen1, int filaBloque2, int colBloque2, IplImage* imagen2) {
    int fila, columna, diferencia = 0;
    for (fila = 0; fila < BLOCK_SIZE; fila++) {
        unsigned char *pImg1 = (unsigned char *) imagen1->imageData + ((filaBloque1 + fila) * imagen1->widthStep) + (colBloque1 * 3);
        unsigned char *pImg2 = (unsigned char *) imagen2->imageData + ((filaBloque2 + fila) * imagen2->widthStep) + (colBloque2 * 3);

        for (columna = 0; columna < BLOCK_SIZE * 3; columna++) {
            diferencia += abs(*pImg1++ - *pImg2++);
        }
    }
    return diferencia;
}



void crearMosaico(IplImage* image1, IplImage* image2) {
    int i, j, k, l, diferencia, filaInicio, columnaInicio;
    for (i = 0; i < (image1->height / BLOCK_SIZE); i++) {
        for (j = 0; j < (image1->width / BLOCK_SIZE); j++) {
            int diferenciaMinima = BLOCK_SIZE * BLOCK_SIZE * 255 * 3;
            for (k = 0; k < (image2->height / BLOCK_SIZE); k++) {
                for (l = 0; l < (image2->width / BLOCK_SIZE); l++) {
                    diferencia = compararBloques(i * BLOCK_SIZE, j * BLOCK_SIZE, image1, k * BLOCK_SIZE, l * BLOCK_SIZE, image2);
                    if (diferenciaMinima > diferencia) {
                        diferenciaMinima = diferencia;
                        filaInicio = k;
                        columnaInicio = l;
                    }
                }
            }
            sustituirBloque(i * BLOCK_SIZE, j * BLOCK_SIZE, image1, filaInicio * BLOCK_SIZE, columnaInicio * BLOCK_SIZE, image2);
        }
    }
}
