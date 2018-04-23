/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: santi
 *
 * Created on 3 de abril de 2018, 13:15
 */

#define IMAGE_NUMBER  2
#define DELAY  1
#define NOMBRE_V1 "Inicio"
#define NOMBRE_V2 "Resultado"
#define NOMBRE_V3 "4 canales"
#define BLOCK_SIZE 16

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>
/*
 * 
 */
void sustituirBloque(int filaBloque1, int colBloque1, IplImage* imagen1, int filaBloque2, int colBloque2, IplImage* imagen2);
int compararBloques(int filaBloque1, int colBloque1, IplImage* imagen1, int filaBloque2, int colBloque2, IplImage* imagen2);
void crearMosaico(IplImage* image1, IplImage* image2);
int compararBloques2(int i, int j, IplImage* img1, int k, int l, IplImage* img2);
void rellenarImagen(IplImage* image1, IplImage* image4c);
void copiarBloque_Paralelo(int img1f, int img1c, IplImage *img1, int img2f, int img2c, IplImage * img2) ;

int main(int argc, char** argv) {
    if (argc != (IMAGE_NUMBER + 1)) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* image1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* image2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);



    if (!image1 || !image2) {
        printf("Error: file %s or %s not found\n", argv[1], argv[2]);
        return EXIT_FAILURE;
    }

    //    cvNamedWindow(NOMBRE_V1, CV_WINDOW_AUTOSIZE);
    //    cvShowImage(NOMBRE_V1, image1);
    //    cvWaitKey(0);


    /**ISERTAR LLAMADA A METODO*/
    IplImage* image4c1 = cvCreateImage(cvSize(image1->width, image1->height), image1->depth, 4);
    IplImage* image4c2 = cvCreateImage(cvSize(image2->width, image2->height), image2->depth, 4);
    rellenarImagen(image1, image4c1);
    rellenarImagen(image2, image4c2);

    clock_t start = clock();
    crearMosaico(image4c1, image4c2);
    printf("Tiempo transcurrido: %f", ((double) clock() - start) / CLOCKS_PER_SEC);



    /**FIN*/
    cvNamedWindow(NOMBRE_V1, CV_WINDOW_AUTOSIZE);
    cvShowImage(NOMBRE_V1, image1);
    cvNamedWindow(NOMBRE_V3, CV_WINDOW_AUTOSIZE);
    cvShowImage(NOMBRE_V3, image4c1);
    cvWaitKey(0);

    cvReleaseImage(&image1);
    cvReleaseImage(&image2);
    cvDestroyWindow(NOMBRE_V1);
    cvDestroyWindow(NOMBRE_V2);
    return EXIT_SUCCESS;


}

void sustituirBloque(int filaBloque1, int colBloque1, IplImage* imagen1, int filaBloque2, int colBloque2, IplImage* imagen2) {
    int fila, columna;
    for (fila = 0; fila < BLOCK_SIZE; fila++) {
        __m128i *pImg1 = (__m128i *) (imagen1->imageData + ((filaBloque1 + fila) * imagen1->widthStep) + (colBloque1 * imagen1->nChannels));
        __m128i *pImg2 = (__m128i *) (imagen2->imageData + ((filaBloque2 + fila) * imagen2->widthStep) + (colBloque2 * imagen2->nChannels));

        for (columna = 0; columna < imagen1->nChannels; columna++) {
            *pImg1++ = *pImg2++;
        }
    }
}
    void copiarBloque_Paralelo(int img1f, int img1c, IplImage *img1, int img2f, int img2c, IplImage * img2) {
        int fila, columna;
        __m128i A;
        for (fila = 0; fila < BLOCK_SIZE; fila++) {
            __m128i *a = (__m128i *) (img1->imageData + (img1f * BLOCK_SIZE + fila) * img1->widthStep + img1c * BLOCK_SIZE * 3);
            __m128i *b = (__m128i *) (img2->imageData + (img2f * BLOCK_SIZE + fila) * img2->widthStep + img2c * BLOCK_SIZE * 3);
            for (columna = 0; columna < 3; columna++) {
                A = _mm_loadu_si128(a); //Puntero de la imagen original para crear
                _mm_store_si128(b, A); //A la parte de la imagen destino(b) le metes el bloque destino (A)
                b++;
                a++;
            }
        }
    }


int compararBloques(int filaBloque1, int colBloque1, IplImage* imagen1, int filaBloque2, int colBloque2, IplImage* imagen2) {
    int fila, columna, diferencia = 0;
    for (fila = 0; fila < BLOCK_SIZE; fila++) {
        __m128i *a = (__m128i *) (imagen1->imageData + ((filaBloque1 + fila) * imagen1->widthStep) + (colBloque1 * imagen1->nChannels));
        __m128i *b = (__m128i *) (imagen2->imageData + ((filaBloque2 + fila) * imagen2->widthStep) + (colBloque2 * imagen2->nChannels));
        __m128i A, B, C, D, E;

        for (columna = 0; columna < imagen1->nChannels; columna++) {
            A = _mm_load_si128(a++); //cargo A
            B = _mm_load_si128(b++); // cargo B
            C = _mm_sad_epu8(A, B);
            D = _mm_srli_si128(C, 8);
            E = _mm_add_epi32(C, D);
            diferencia += _mm_cvtsi128_si32(E);
        }
    }
    return diferencia;
}

void crearMosaico(IplImage* image1, IplImage* image2) {
    int i, j, k, l, diferencia, filaInicio, columnaInicio;
    for (i = 0; i < (image1->height / BLOCK_SIZE); i++) {
        for (j = 0; j < (image1->width / BLOCK_SIZE); j++) {
            int diferenciaMinima = BLOCK_SIZE * BLOCK_SIZE * 255 * image1->nChannels;
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
            //sustituirBloque(i * BLOCK_SIZE, j * BLOCK_SIZE, image1, filaInicio * BLOCK_SIZE, columnaInicio * BLOCK_SIZE, image2);
            copiarBloque_Paralelo(i * BLOCK_SIZE, j * BLOCK_SIZE, image1, filaInicio * BLOCK_SIZE, columnaInicio * BLOCK_SIZE, image2) ;

        }
    }
}

void rellenarImagen(IplImage* image1, IplImage* image4c) {
    int fila, columna;
    for (fila = 0; fila < image1->height; fila++) {
        unsigned char *pImg1 = (unsigned char *) (image1->imageData + fila * image1->widthStep);
        unsigned char *pImg2 = (unsigned char *) (image4c->imageData + fila * image4c->widthStep);
        for (columna = 0; columna < image1->width; columna++) {
            *pImg2++ = *pImg1++;
            *pImg2++ = *pImg1++;
            *pImg2++ = *pImg1++;
            *pImg2++ = 0;
        }
    }
}
