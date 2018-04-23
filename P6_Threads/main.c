#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>
#include <pthread.h>

#define ANCHO 16
#define ALTO 16
#define NTHREADS 2

IplImage * Img1;
IplImage * Img2;

void copiarBloque(int i, int j, IplImage * imagenOrigen, int k, int l, IplImage * imagenDestino);
int compararBloque(int i, int j, IplImage* img1, int k, int l, IplImage* img2);
void crearMosaico(IplImage* img1, IplImage* img2);
IplImage* cambiar3a4(IplImage* imagen);
void mosaico_thread(void *ptr);


// Definicion de las variables globales aqui

int main(int argc, char** argv) {

    struct timespec start, finish;
    float elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (argc != 3) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    if (!Img1) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!Img2) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }

    Img1 = cambiar3a4(Img1);
    Img2 = cambiar3a4(Img2);

    int par = 0;
    int impar = 1;
    pthread_t idThreadPar;
    pthread_t idThreadImpar;
    pthread_create(&idThreadPar, NULL, &mosaico_thread, &par);
    pthread_create(&idThreadPar, NULL, &mosaico_thread, &impar);
    pthread_join(idThreadPar, NULL);
    pthread_join(idThreadImpar, NULL);

    cvNamedWindow("ImagenMosaico", CV_WINDOW_AUTOSIZE);

    cvShowImage("ImagenMosaico", Img1);
    cvWaitKey(0);

    cvDestroyWindow("ImagenMosaico");
    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);

    return EXIT_SUCCESS;

}

IplImage* cambiar3a4(IplImage* imagen) {

    IplImage* Img4 = cvCreateImage(cvSize(imagen->width, imagen->height), imagen->depth, 4);

    int fila;

    for (fila = 0; fila < imagen->height; fila++) {

        unsigned char *pImg4 = (unsigned char *) Img4->imageData + (fila) * Img4->widthStep;
        unsigned char *pImagen = (unsigned char *) imagen->imageData + (fila) * imagen->widthStep;

        int columna = 0;

        for (columna = 0; columna < imagen->width; columna++) {

            *pImg4++ = *pImagen++;
            *pImg4++ = *pImagen++;
            *pImg4++ = *pImagen++;
            *pImg4++ = 0;
        }
    }

    return Img4;
}

void copiarBloque(int i, int j, IplImage* imagenOrigen, int k, int l, IplImage* imagenDestino) {


    int fila;
    for (fila = 0; fila < ALTO; fila++) {

        __m128i *pImagenDestino = (__m128i*) (imagenDestino->imageData + (fila + k) * imagenDestino->widthStep + imagenDestino->nChannels * l);
        __m128i *pImagenOrigen = (__m128i*) (imagenOrigen->imageData + (fila + i) * imagenOrigen->widthStep + imagenDestino->nChannels * j);
        int columna;

        for (columna = 0; columna < imagenOrigen->nChannels; columna++) {

            *pImagenDestino = *pImagenOrigen;
            pImagenDestino++;
            pImagenOrigen++;

        }


    }


}

int compararBloque(int i, int j, IplImage* img1, int k, int l, IplImage* img2) {

    int fila;
    int diferencia = 0;

    for (fila = 0; fila < ALTO; fila++) {

        __m128i *pImg1 = (__m128i*) (img1->imageData + (fila + i) * img1->widthStep + img1->nChannels * j);
        __m128i *pImg2 = (__m128i*) (img2->imageData + (fila + k) * img2->widthStep + img2->nChannels * l);

        int columna;

        for (columna = 0; columna < img1->nChannels; columna++) {
            int sad = 0;

            __m128i A, B, C, D, E;
            A = _mm_load_si128((__m128i *) pImg1);
            B = _mm_load_si128((__m128i *) pImg2);
            C = _mm_sad_epu8(A, B);
            D = _mm_srli_si128(C, 8);
            E = _mm_add_epi32(C, D);
            sad = _mm_cvtsi128_si32(E);

            diferencia += sad;

            pImg1++;
            pImg2++;

        }
    }

    return diferencia;

}

void crearMosaico(IplImage* img1, IplImage* img2) {

    int filaBloque;
    int columnaBloque;
    int filaBloque2;
    int columnaBloque2;
    int diferenciaMinima = ALTO * ANCHO * 255 * img1->nChannels;
    int indiceFila;
    int indiceColumna;
    int diferencia = 0;

    for (filaBloque = 0; filaBloque < img1->height / ALTO; filaBloque++) {
        for (columnaBloque = 0; columnaBloque < img1->width / ANCHO; columnaBloque++) {
            for (filaBloque2 = 0; filaBloque2 < img2->height / ALTO; filaBloque2++) {
                for (columnaBloque2 = 0; columnaBloque2 < img2->width / ANCHO; columnaBloque2++) {
                    diferencia = compararBloque(filaBloque * ALTO, columnaBloque* ANCHO, img1, filaBloque2 * ALTO, columnaBloque2 * ANCHO, img2);
                    if (diferencia < diferenciaMinima) {
                        diferenciaMinima = diferencia;
                        indiceFila = filaBloque2 * ALTO;
                        indiceColumna = columnaBloque2 * ANCHO;
                    }
                }
            }

            copiarBloque(indiceFila, indiceColumna, img2, filaBloque * ALTO, columnaBloque * ANCHO, img1);

            diferenciaMinima = ALTO * ANCHO * 255 * img1->nChannels;
        }
    }
}

void mosaico_thread(void *ptr) {
    int *fila = (int *) ptr;
    int filaBloque;
    int columnaBloque;
    int filaBloque2;
    int columnaBloque2;
    int diferenciaMinima = ALTO * ANCHO * 255 * Img1->nChannels;
    int indiceFila;
    int indiceColumna;
    int diferencia = 0;

    for (filaBloque = *fila; filaBloque < Img1->height / ALTO; filaBloque += 2) {
        for (columnaBloque = 0; columnaBloque < Img1->width / ANCHO; columnaBloque++) {
            for (filaBloque2 = 0; filaBloque2 < Img2->height / ALTO; filaBloque2++) {
                for (columnaBloque2 = 0; columnaBloque2 < Img2->width / ANCHO; columnaBloque2++) {
                    diferencia = compararBloque(filaBloque * ALTO, columnaBloque* ANCHO, Img1, filaBloque2 * ALTO, columnaBloque2 * ANCHO, Img2);
                    if (diferencia < diferenciaMinima) {
                        diferenciaMinima = diferencia;
                        indiceFila = filaBloque2 * ALTO;
                        indiceColumna = columnaBloque2 * ANCHO;
                    }
                }
            }

            copiarBloque(indiceFila, indiceColumna, Img2, filaBloque * ALTO, columnaBloque * ANCHO, Img1);

            diferenciaMinima = ALTO * ANCHO * 255 * Img1->nChannels;
        }
    }
}