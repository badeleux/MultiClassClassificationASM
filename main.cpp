
//
//  main.cpp
//  ASMNeuralNetwork
//
//  Created by Kamil Badyla on 08.10.2012.
//  Copyright (c) 2012 3Soft S.A. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include "Image.h"
#include "MultiClassClassificator.h"

//params: pointer to X, X_rows, X_cols, ptr to Y, Y_rows, number_of_class, lambda
typedef unsigned int (*trainThetaMatrix)(int*, int, int, int*, int, int, int);

int* convertArmadilloMatrixToNormalArray(Mat<int> matrix);

int* convertArmadilloMatrixToNormalArray(Mat<int> matrix)
{
	//creating matrix
	int *array = new int[matrix.n_rows * matrix.n_cols];
	//filling array
	for (int i = 0 ; i < matrix.n_rows ; i++)
	{
		for (int j = 0 ; j < matrix.n_cols ; j++)
		{
			array[j + i*matrix.n_cols] = matrix(i,j);
		}
	}
	
	return array;

}

int main(int argc, const char * argv[])
{
    void *handle;
	char *error;
	 
	
    // insert code here...
    handle = dlopen("./libclass.so", RTLD_LAZY);
	if(handle == NULL) {
		fprintf(stderr, "Error: open/load error of libclass.so failed: %s\n", dlerror());
		exit(1);
	}
	
	trainThetaMatrix trainFunction = (trainThetaMatrix) dlsym(handle, "trainThetaMatrix");
	if((error = dlerror()) != NULL) {
		fprintf(stderr, "Error: symbol lookup in libclass.so failed: %s\n", dlerror());
		exit(2);
	}
	
    
    mat X, y;
    X.load("./Y.dat", raw_ascii);
    y.load("./Y.dat", raw_ascii);
    int M, N, Q; // rows, cols, grayscale
    int val;
    bool type;

	// read image header
    Image::readImageHeader("./seven.PGM", N, M, Q, type);

	// allocate memory for the image array
	Image image(N, M, Q);
	// read image
    Image::readImage("./seven.PGM", image);


    mat imagePixelMatrix = image.getPixelMatrix();
imagePixelMatrix = join_rows(ones(1,1), X.row(2256));


    MultiClassClassificator classificator = MultiClassClassificator();
    classificator.trainThetaMatrix(X, y, 10, 23);
//    classificator.predictUsingThetaMatrix(imagePixelMatrix);
	Mat<int> testX;
	testX << 2 << 3 << endr << 4 << 5 << endr;

	int *array = convertArmadilloMatrixToNormalArray(testX);

	std::cout << "Przed";
   for (int i = 0 ; i < 4 ; i++)
		std::cout << array[i] << " "; 
 trainFunction(array, 3,3,array,10,0x69,0x123);

std::cout << "Po: ";
   for (int i = 0 ; i < 4 ; i++)
		std::cout << array[i] << " "; 
    
   fflush(stdout); 
    
   dlclose(handle); 
    
    return 0;
}

