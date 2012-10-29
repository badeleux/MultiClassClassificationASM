
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
typedef float* (*trainThetaMatrix)(float*, int, int, int*,int, float*);
//params: X.row, allTheta, predictVector, allTheta_rows, allTheta_cols
typedef unsigned int (*predictUsingThetaMatrix)(float*, float*, float*, int, int);

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

	//get handle for predict Function
	predictUsingThetaMatrix predictFunction = (predictUsingThetaMatrix) dlsym(handle, "predictUsingThetaMatrix");
	if ((error = dlerror()) != NULL) {
		fprintf(stderr, "Error: symbol lookup in libclass.so failed: %s\n", dlerror());
		exit(2);
	}
	
									//load training set
    fmat X, y;
    X.load("./X.dat", raw_ascii);
    y.load("./Y.dat", raw_ascii);
	
	X = X/255;

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

	pthread_t thread1, thread2;


	while(1) 
	{
		char choice;
		clock_t begin;
		clock_t end;

		std::cout << "Witaj w programie uczacym macierz theta. " << std::endl << "By nauczyc macierz theta poprzez program napisny w jezyku wysokopoziomowym (C++) wcisnij klawisz \"c\". " << std::endl << "By nauczyc macierz w jezyku niskopioziomowym Netwide Assembler wcisnij \"a\"";
		std::cin >> choice;

		

		if (choice == 'c')
		{
			begin = clock();

			fmat thetaMatrix1 = zeros<fmat>(5, X.n_cols);
			fmat thetaMatrix2 = zeros<fmat>(5, X.n_cols);

			TrainArgs args1 = {&X, &y, 0, 5, &thetaMatrix1};
			TrainArgs args2 = {&X, &y, 5, 10, &thetaMatrix2};

			printf("pierwszy; %x\n", args1.theta);
			pthread_create(&thread1, NULL, &MultiClassClassificator::trainThetaMatrix, (void*)&args1);
			pthread_create(&thread2, NULL, &MultiClassClassificator::trainThetaMatrix, (void*)&args2);
			
			pthread_join(thread1, NULL);
			pthread_join(thread2, NULL);


			
			printf("%x\n", args1.theta);
			thetaMatrix1 = *(fmat*)args1.theta;
			thetaMatrix2 = *(fmat*)args2.theta;
			fmat allThetaMatrix =  join_cols(thetaMatrix1, thetaMatrix2);
			MultiClassClassificator::predictUsingThetaMatrix(allThetaMatrix, X.row(4425));
			

			end = clock();
			
		}
		else if (choice == 'a')
		{
//			MultiClassClassificator classificator = MultiClassClassificator();
//			fmat test = classificator.trainThetaMatrix(X,y,10,allTheta);
			
	fmat allTheta = zeros<fmat>(10, X.n_cols);
			float *array = convertArmadilloMatrixToNormalArray<float>(X);
			int *arrayY = convertArmadilloMatrixToNormalArray<int>(y);
			float *allThetaArray = convertArmadilloMatrixToNormalArray<float>(allTheta);
			float *X_row = convertArmadilloMatrixToNormalArray<float>(X.row(325));
			float *predictVector = new float[10];
			begin = clock();
			float *transposeASM = trainFunction(array,5000,401,arrayY,10, allThetaArray);
	/*		float *transposeC = convertArmadilloMatrixToNormalArray<float>(test);
			for (int i =0 ; i < 5000 ; i++)
{
				if (transposeASM[i] == transposeC[i])
					std::cout << "Ok ";
				else
					std::cout << "nie zgadza sie na " <<  transposeASM[i] << " " << transposeC[i];
				std::cout << std::endl;
				fflush(stdout);
}*/
			end = clock();
			predictFunction(X_row, allThetaArray, predictVector, 10, 401);
			for (int i = 0 ; i < 10 ; i++)
				std::cout << predictVector[i] << std::endl;
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

