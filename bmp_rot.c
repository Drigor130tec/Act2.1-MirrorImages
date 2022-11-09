#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 20 

void createImage(){
    
    char inputImageName [16];
    snprintf(inputImageName, 16, "image%i.bmp", omp_get_thread_num()+1);
    char outputImageName[16];
    snprintf(outputImageName, 16, "output%i.bmp", omp_get_thread_num()+1);
    
    
    FILE *inputImage; 
    inputImage = fopen(inputImageName,"rb");    //Imagen original a transformar
    FILE *outputImage;
    outputImage = fopen(outputImageName,"wb");    //Imagen transformada
    
    
    long ancho, alto;
    unsigned char r, g, b;  
    unsigned char* arr_in;
    unsigned char* arr_out;         

    unsigned char xx[54];
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(inputImage);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto  = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];


    arr_in = (unsigned char*)malloc(ancho*alto*3*sizeof(unsigned char));
    arr_out = (unsigned char*)malloc(ancho*alto*3*sizeof(unsigned char));

    int padding= 0;
    int j= 0;
    while(!feof(inputImage)){
        arr_in[j] = fgetc(inputImage);
        arr_out [j] = arr_in [j];
        j++;
    }
    padding = ancho%4;

    
    for(int i = 0; i < alto; i++){
        for(int j = 0; j < (ancho*3); j+= 3){
            arr_out [(i*((ancho*3)+padding))+j-3] = arr_in [(i*((ancho*3)+padding))+ (ancho*3) - j];
            arr_out [(i*((ancho*3)+padding))+j-4] = arr_in [(i*((ancho*3)+padding))+ (ancho*3 )- j - 1];
            arr_out [(i*((ancho*3)+padding))+j-5] = arr_in [(i*((ancho*3)+padding))+ (ancho*3) - j - 2];
        }
    }

    //Seccion en la que genera la nueva imagen invertida
    for(int i = 0; i < ancho*alto*3; i++){
        fputc(arr_out[i], outputImage);
    }
    fclose(inputImage);
    fclose(outputImage);
}

int main(){
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    createImage();

    }
