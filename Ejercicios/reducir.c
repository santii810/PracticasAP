#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#define MULTI 4

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    IplImage* Img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);



    // Always check if the program can find the image file
    if (!Img) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage(argv[1], Img);
    cvWaitKey(0);

    // Crea la imagen para la componete azul
    IplImage* copia = cvCreateImage(cvSize(Img->width/MULTI, Img->height/MULTI), IPL_DEPTH_8U, 3);


    int fila, columna;
    for (fila = 0; fila < Img->height; fila+=MULTI) {
        unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
        unsigned char *pCopia = (unsigned char *) copia->imageData + (fila/MULTI) * copia->widthStep;

        for (columna = 0; columna < Img->width; columna++) {            
            if (columna%MULTI == 0) {
                *pCopia++ = *pImg++;
                *pCopia++ = *pImg++;
                *pCopia++ = *pImg++;
            }else{
                pImg+=3;
            }

        }
    }

    // crea y muestras las ventanas con las im genes
    cvNamedWindow("Copia", CV_WINDOW_AUTOSIZE);
    cvShowImage("Copia", copia);

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&Img);
    cvReleaseImage(&copia);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Copia");

    return EXIT_SUCCESS;
}
