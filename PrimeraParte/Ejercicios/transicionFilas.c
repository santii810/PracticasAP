#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* imgFrutas = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* imgAves = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);



    // Always check if the program can find the image file
    if (!imgAves || !imgFrutas) {
        printf("Error: file %s or %s not found\n", argv[1], argv[2]);
        return EXIT_FAILURE;
    }



    // Crea la imagen para la componete azul
    IplImage* aves = cvCreateImage(cvSize(imgAves->width, imgAves->height), IPL_DEPTH_8U, 3);
    IplImage* frutas = cvCreateImage(cvSize(imgAves->width, imgAves->height), IPL_DEPTH_8U, 3);


    int fila, columna;
    for (fila = 0; fila < aves->height; fila++) {
        unsigned char *pAves = (unsigned char *) imgAves->imageData + fila * aves->widthStep;
        unsigned char *pFrutas = (unsigned char *) imgFrutas->imageData + fila * frutas->widthStep;

        for (columna = 0; columna < aves->width; columna++) {
            *pFrutas++ = *pAves++;
            *pFrutas++ = *pAves++;
            *pFrutas++ = *pAves++;
        }
        cvWaitKey(1);
        cvShowImage(argv[1], imgFrutas);

    }



    // crea y muestras las ventanas con las im genes
    //    cvNamedWindow("Copia", CV_WINDOW_AUTOSIZE);
    //    cvShowImage("Copia", copia);
    //
    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&imgFrutas);
    //    cvReleaseImage(&copia);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    //    cvDestroyWindow("Copia");

    return EXIT_SUCCESS;
}
