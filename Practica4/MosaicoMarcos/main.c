
#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>
#define ANCHO 16
#define ALTO 16

void copiarBloque(int i, int j, IplImage * imagenOrigen, int k, int l, IplImage * imagenDestino);
int compararBloque(int i, int j, IplImage* img1, int k, int l, IplImage* img2);
void crearMosaico(IplImage* img1, IplImage* img2);

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    // Always check if the program can find a file



    if (!Img1) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!Img2) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }


    //crearMosaico(Img1, Img2);
    copiarBloque(0,0,Img2,100,100,Img1);
    copiarBloque(56,25,Img2,10,10,Img1);
   
    struct timespec start, finish;
    float elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
   
     crearMosaico(Img1, Img2);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Tiempo transcurrido: %f", elapsed);
   
    
    cvNamedWindow("ImagenMosaico", CV_WINDOW_AUTOSIZE);
    cvShowImage("ImagenMosaico", Img1);
    cvWaitKey(0);

    cvDestroyWindow("ImagenMosaico");
    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);


    return EXIT_SUCCESS;

}

void copiarBloque(int i, int j, IplImage* imagenOrigen, int k, int l, IplImage* imagenDestino) {


    int fila;
    for (fila = 0; fila < ALTO; fila++) {

        unsigned char *pImagenDestino = (unsigned char *) imagenDestino->imageData + (fila + k) * imagenDestino->widthStep + 3 * l;
        unsigned char *pImagenOrigen = (unsigned char *) imagenOrigen->imageData + (fila + i) * imagenOrigen->widthStep + 3 * j;
        int columna;
        for (columna = 0; columna < ANCHO; columna++) {

            //Imagen PA TODO LO COLORE
            *pImagenDestino++ = *pImagenOrigen++;
            *pImagenDestino++ = *pImagenOrigen++;
            *pImagenDestino++ = *pImagenOrigen++;

        }


    }



}

int compararBloque(int i, int j, IplImage* img1, int k, int l, IplImage* img2) {

    int fila;
    int diferencia = 0;

    for (fila = 0; fila < ALTO; fila++) {
        unsigned char *pImg1 = (unsigned char *) img1->imageData + (fila + i) * img1->widthStep + 3* j;
        unsigned char *pImg2 = (unsigned char *) img2->imageData + (fila + k) * img2->widthStep + 3* l;
        int columna;

        for (columna = 0; columna < ANCHO; columna++) {
            diferencia += abs(*pImg1++ - *pImg2++);
            diferencia += abs(*pImg1++ - *pImg2++);
            diferencia += abs(*pImg1++ - *pImg2++);
        }

    }

    return diferencia;

}

void crearMosaico(IplImage* img1, IplImage* img2) {

    int filaBloque;
    int columnaBloque;
    int filaBloque2;
    int columnaBloque2;
    int indiceFila;
    int diferenciaMinima = ALTO * ANCHO * 255 * 3;
    int indiceColumna;
    int diferencia=0;
   
    for (filaBloque = 0; filaBloque < img1->height / ALTO; filaBloque++) {
        for (columnaBloque = 0; columnaBloque < img1->width / ANCHO; columnaBloque++) {          
         
            for (filaBloque2 = 0; filaBloque2 < img2->height / ALTO; filaBloque2++) {
                for (columnaBloque2 = 0; columnaBloque2 < img2->width / ANCHO; columnaBloque2++) {
                    diferencia = compararBloque(filaBloque * ALTO, columnaBloque* ANCHO, img1, filaBloque2 * ALTO, columnaBloque2 * ANCHO, img2);
                    if (diferencia < diferenciaMinima) {
                        diferenciaMinima = diferencia;
                        indiceFila = filaBloque2 * ALTO;
                        indiceColumna = columnaBloque2 * ANCHO ;
                    }

                }
            }

            copiarBloque(indiceFila , indiceColumna , img2, filaBloque * ALTO , columnaBloque * ANCHO , img1);

            diferenciaMinima = ALTO * ANCHO * 255 * 3;
        }

    }




}
