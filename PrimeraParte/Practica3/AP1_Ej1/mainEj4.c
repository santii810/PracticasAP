#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
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
    IplImage* copia = cvCreateImage(cvSize(Img->width + 40, Img->height + 40), IPL_DEPTH_8U, 3);


    int fila, columna;
    for (fila = 0; fila < copia->height; fila++) {


        unsigned char *pImg = (unsigned char *) Img->imageData + (fila-20) * Img->widthStep;
        unsigned char *pCopia = (unsigned char *) copia->imageData + fila * copia->widthStep;

        for (columna = 0; columna < copia->width; columna++) {
            if (columna < 20 || columna >= copia->width - 20 || fila < 20 || fila >= copia->height - 20) {
                *pCopia++ = 93;
                *pCopia++ = 213;
                *pCopia++ = 254;
            } else {
                *pCopia++ = *pImg++;
                *pCopia++ = *pImg++;
                *pCopia++ = *pImg++;
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
