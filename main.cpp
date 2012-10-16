
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
typedef unsigned int (*trainThetaMatrix)(int*, int, int, int*, int, int, double);


int main(int argc, const char * argv[])
{
    void *handle;
	char *error;
	trainThetaMatrix *trainFunc;
	 
	
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
    X.load("./X.dat", raw_ascii);
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
    imagePixelMatrix = join_rows(ones(1,1), imagePixelMatrix);


    MultiClassClassificator classificator = MultiClassClassificator();
    classificator.trainThetaMatrix(X, y, 10, 23);

    classificator.predictUsingThetaMatrix(imagePixelMatrix);
    
    
    
    
    
   dlclose(handle); 
    
    return 0;
}

