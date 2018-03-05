#define IMAGE_NUMBER  2
#define BLOCK_SIZE 8

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

void cambiarColores(IplImage* image1, IplImage* image2);

int main(int argc, char **argv) {
    if (argc != (IMAGE_NUMBER + 1)) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* image1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);


    // Always check if the program can find the image file
    if (!image1) {
        printf("Error: file %s or not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow("Inicio", CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage("Inicio", image1);
    cvWaitKey(0);
    IplImage* image2 = cvCreateImage(cvSize(image1->width, image1->height), IPL_DEPTH_8U, 3);


    cambiarColores(image1, image2);


    // crea y muestras las ventanas con las imagenes
    cvNamedWindow("Resultado", CV_WINDOW_AUTOSIZE);
    cvShowImage("Resultado", image2);
    cvWaitKey(0);


    cvDestroyWindow("Inicio");
    cvDestroyWindow("Resultado");

    // memory release for images before exiting the application
    cvReleaseImage(&image1);
    cvReleaseImage(&image2);

    return EXIT_SUCCESS;
}

void cambiarColores(IplImage* image1, IplImage* image2) {
    int fila, columna;
    for (fila = 0; fila < image1->height; fila++) {
        unsigned char *pImg = (unsigned char *) image1->imageData + fila * image1->widthStep;
        unsigned char *pCopy = (unsigned char *) image2->imageData + fila * image2->widthStep;

        for (columna = 0; columna < image1->width; columna++) {
            if (fila < image1->height / 2) {
                if (columna < image1->width / 2) {
                    //Imagen Normal
                    *pCopy++ = *pImg++;
                    *pCopy++ = *pImg++;
                    *pCopy++ = *pImg++;
                } else {
                    //Imagen Roja
                    *pCopy++ = 0;
                    pImg++;
                    *pCopy++ = *pImg++;
                    *pCopy++ = 0;
                    pImg++;
                }
            } else {
                if (columna < image1->width / 2) {
                    //Imagen Green
                    *pCopy++ = 0;
                    pImg++;
                    *pCopy++ = 0;
                    pImg++;
                    *pCopy++ = *pImg++;
                } else {
                    //Imagen BLUE
                    *pCopy++ = *pImg++;
                    *pCopy++ = 0;
                    pImg++;
                    *pCopy++ = 0;
                    pImg++;
                }
            }
        }
    }
}