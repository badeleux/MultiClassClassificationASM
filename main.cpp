
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
#include <ctime>
#include <iomanip>
using namespace std;


//params: pointer to X, X_rows, X_cols, ptr to Y, Y_rows, number_of_class
typedef unsigned int (*trainThetaMatrix)(float*, int, int, int*,int, float*);


template <class T> 
T* convertArmadilloMatrixToNormalArray(fmat matrix)
{
	//creating matrix
	T *array = new T[matrix.n_rows * matrix.n_cols];
	//filling array
	for (unsigned int i = 0 ; i < matrix.n_rows ; i++)
	{
		for (unsigned int j = 0 ; j < matrix.n_cols ; j++)
		{

			array[j + i*matrix.n_cols] = (T)matrix(i,j);
		}
	}
	
	return array;
}


int main(int argc, const char * argv[])
{
    void *handle;
	char *error;
	
	//get handle for dynamic library 
    handle = dlopen("./libclass.so", RTLD_LAZY);
	if(handle == NULL) {
		fprintf(stderr, "Error: open/load error of libclass.so failed: %s\n", dlerror());
		exit(1);
	}
	
	//get handle for train Function
	trainThetaMatrix trainFunction = (trainThetaMatrix) dlsym(handle, "trainThetaMatrix");
	if((error = dlerror()) != NULL) {
		fprintf(stderr, "Error: symbol lookup in libclass.so failed: %s\n", dlerror());
		exit(2);
	}
	
    //load training set
    fmat X, y;
    X.load("./X.dat", raw_ascii);
    y.load("./Y.dat", raw_ascii);
	
	//add bias unit
	fmat one_vector = ones<fmat>(X.n_rows,1);
	X = join_rows(one_vector,X);

    //int M, N, Q; // rows, cols, grayscale
    //bool type;

	// read image header
    //Image::readImageHeader("./seven.PGM", N, M, Q, type);

	// allocate memory for the image array
	//Image image(N, M, Q);
	// read image
    //Image::readImage("./seven.PGM", image);


    //fmat imagePixelMatrix = image.getPixelMatrix();
	//imagePixelMatrix = join_rows(ones<fmat>(1,1), X.row(2256));


	fmat allTheta = zeros<fmat>(10, X.n_cols);

	while(1) 
	{
		char choice;
		clock_t begin;
		clock_t end;

		std::cout << "Witaj w programie uczacym macierz theta. " << std::endl << "By nauczyc macierz theta poprzez program napisny w jezyku wysokopoziomowym (C++) wcisnij klawisz \"c\". " << std::endl << "By nauczyc macierz w jezyku niskopioziomowym Netwide Assembler wcisnij \"a\"";
		std::cin >> choice;
		if (choice == 'c')
		{
    		MultiClassClassificator classificator = MultiClassClassificator();
			begin = clock();
			classificator.trainThetaMatrix(X, y, 10, allTheta);
			end = clock();
		}
		else if (choice == 'a')
		{
			float *array = convertArmadilloMatrixToNormalArray<float>(X);
			int *arrayY = convertArmadilloMatrixToNormalArray<int>(y);
			float *allThetaArray = convertArmadilloMatrixToNormalArray<float>(allTheta);
			begin = clock();
			trainFunction(array,5000,401,arrayY,10, allThetaArray);
			end = clock();
			delete[] array;
			delete[] allThetaArray;
			delete[] arrayY;
		} 

		double elapsedSecs = (double)((end - begin)/1000) ;
		std::cout << "Na wytrenowanie macierzy potrzebne bylo " << std::setprecision(2) << elapsedSecs <<  "ms";
	}


    fflush(stdout); 
    
	dlclose(handle); 
   
    return 0;
}

