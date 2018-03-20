#define IMAGE_NUMBER  2
#define BLOCK_SIZE 128

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

void degradar(IplImage* image1);
void promocionar(IplImage* image1,IplImage* image1);

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

    cvNamedWindow("Inicio", CV_WINDOW_AUTOSIZE);
    cvShowImage("Inicio", image1);
    cvWaitKey(0);


    int i;
    for (i = 0; i < 255; i++) {
        degradar(image1);
        cvShowImage("Inicio", image1);
        cvWaitKey(1);
    }
    for (i = 0; i < 255; i++) {
        promocionar(image1,image2);
        cvShowImage("Inicio", image1);
        cvWaitKey(1);
    }
    



    cvReleaseImage(&image1);
    cvReleaseImage(&image2);
    cvDestroyWindow("Inicio");
    cvDestroyWindow("Resultado");
    return EXIT_SUCCESS;
}

void degradar(IplImage* image1) {
    int i;
    unsigned char *pImg1 = (unsigned char *) image1->imageData;
    for (i = 0; i < image1->imageSize; i++) {
            if (*pImg1 > 0)
                (*pImg1)--;
            *pImg1++;
    }
}
