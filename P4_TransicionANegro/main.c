#define IMAGE_NUMBER  2
#define DELAY  1
#define NOMBRE_V1 "Inicio"
#define NOMBRE_V2 "Resultado"

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


void degradar_bit(IplImage* image1);
void promocionar(IplImage* image1, IplImage* image2);
void degradar_pixel(IplImage* image1);
void main_degradarABit(IplImage* image1);
void main_degradarAPixel(IplImage* image1, IplImage* image2);
void main_convertirImagen(IplImage* image1, IplImage* image2);
void convertir(IplImage* image1, IplImage* image2);
void main_degradadoParalelismo(IplImage* image1);

int main(int argc, char **argv) {

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

    cvNamedWindow(NOMBRE_V1, CV_WINDOW_AUTOSIZE);
    cvShowImage(NOMBRE_V1, image1);
    cvWaitKey(0);


    /**ISERTAR LLAMADA A METODO*/

    //main_degradarABit(image1);
    //main_degradarAPixel( image1, image2);
    //main_convertirImagen(image1, image2);
    main_degradadoParalelismo(image1);



    /**FIN*/

    cvReleaseImage(&image1);
    cvReleaseImage(&image2);
    cvDestroyWindow(NOMBRE_V1);
    cvDestroyWindow(NOMBRE_V2);
    return EXIT_SUCCESS;

}

void degradar_bit(IplImage* image1) {
    int fila, columna;
    for (fila = 0; fila < image1->height; fila++) {
        unsigned char *pImg1 = (unsigned char *) image1->imageData + fila * image1->widthStep;
        for (columna = 0; columna < image1->width * 3; columna++) {
            if (*pImg1 > 0)
                *pImg1 = *pImg1 - 1;
            *pImg1++;

        }
    }
}

void degradar_pixel(IplImage* image1) {
    int i;
    unsigned char *pImg1 = (unsigned char *) image1->imageData;
    for (i = 0; i < image1->imageSize; i++) {
        if (*pImg1 > 0)
            (*pImg1)--;
        *pImg1++;
    }
}

void promocionar(IplImage* image1, IplImage* image2) {
    int fila, columna;
    for (fila = 0; fila < image1->height; fila++) {
        unsigned char *pImg1 = (unsigned char *) image1->imageData + fila * image1->widthStep;
        unsigned char *pImg2 = (unsigned char *) image2->imageData + fila * image2->widthStep;
        for (columna = 0; columna < image1->width * 3; columna++) {
            if (*pImg1 < *pImg2)
                (*pImg1)++;
            *pImg1++;
            *pImg2++;


        }
    }
}

void convertir(IplImage* image1, IplImage* image2) {
    int fila, columna;
    for (fila = 0; fila < image1->height; fila++) {
        unsigned char *pImg1 = (unsigned char *) image1->imageData + fila * image1->widthStep;
        unsigned char *pImg2 = (unsigned char *) image2->imageData + fila * image2->widthStep;
        for (columna = 0; columna < image1->width * 3; columna++) {
            if (*pImg1 < *pImg2)
                (*pImg1)++;
            if (*pImg1 > *pImg2)
                (*pImg1)--;
            *pImg1++;
            *pImg2++;


        }
    }
}

void main_degradarABit(IplImage* image1) {
    int i;
    for (i = 0; i < 255; i++) {

        degradar_bit(image1);
        cvNamedWindow(NOMBRE_V1, CV_WINDOW_AUTOSIZE);
        cvShowImage(NOMBRE_V1, image1);
        cvWaitKey(1);
    }
}

void main_degradarAPixel(IplImage* image1, IplImage* image2) {
    int i;
    for (i = 0; i < 255; i++) {
        degradar_pixel(image1);
        cvShowImage(NOMBRE_V1, image1);
        cvWaitKey(DELAY);
    }
    for (i = 0; i < 255; i++) {
        promocionar(image1, image2);
        cvShowImage(NOMBRE_V1, image1);
        cvWaitKey(DELAY);
    }
}

void main_convertirImagen(IplImage* image1, IplImage* image2) {
    int i;
    for (i = 0; i < 255; i++) {
        convertir(image1, image2);
        cvShowImage(NOMBRE_V1, image1);
        cvWaitKey(DELAY);
    }
}

void main_degradadoParalelismo(IplImage* image1) {


    __m128i unos = _mm_set1_epi8(1);
    IplImage* ImgAux = cvCloneImage(image1);
    int i, byte;

    for (i = 0; i <= 255; i++) {
        __m128i *pImgAux = (__m128i*) (ImgAux->imageData);
        for (byte = 0; byte < ImgAux->imageSize; byte += 16) {
            __m128i A = _mm_loadu_si128(pImgAux);
            __m128i B = _mm_subs_epu8(A, unos);
            _mm_storeu_si128(pImgAux, B);
            pImgAux++;
        }
        cvShowImage(NOMBRE_V1, ImgAux);
        cvWaitKey(1);
    }

}