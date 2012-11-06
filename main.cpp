
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


//params: pointer to X, X_rows, X_cols, ptr to Y, from, to, allthetaptr
typedef void* (*trainThetaMatrix)(void*);//float*, int, int, int*,int, int, float*);
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

	for (unsigned int i = 0 ; i < y.n_elem ; i++)
		if (y(i) == 10)
			y(i) = 0;

	X = X/255;

	//add bias unit
	fmat one_vector = ones<fmat>(X.n_rows/2,1);
	fmat X1 = X.submat(0,0,2499,399);
	X1 = join_rows(one_vector, X1);
	fmat X2 = X.submat(2500,0,4999,399);
	X2 = join_rows(one_vector, X2);

	fmat y1 = y.submat(0,0,2499,0);
	fmat y2 = y.submat(2500,0,4999,0);

	fmat one_vector2 = ones<fmat>(X.n_rows,1);
	X = join_rows(one_vector2, X);
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
	int testRow = 2300;

	while(1) 
	{
		char choice;
		clock_t begin;
		clock_t end;

		std::cout << "Witaj w programie uczacym macierz theta. " << std::endl << "By nauczyc macierz theta poprzez program napisny w jezyku wysokopoziomowym (C++) wcisnij klawisz \"c\". " << std::endl << "By nauczyc macierz w jezyku niskopioziomowym Netwide Assembler wcisnij \"a\"";
		std::cin >> choice;

		int prediction = 0;

		if (choice == 'c')
		{
			begin = clock();

			fmat thetaMatrix1 = zeros<fmat>(5, X1.n_cols);
			fmat thetaMatrix2 = zeros<fmat>(5, X2.n_cols);

			TrainArgs args1 = {&X1, &y1, 0, 5, &thetaMatrix1};
			TrainArgs args2 = {&X2, &y2, 5, 10, &thetaMatrix2};

			pthread_create(&thread1, NULL, &MultiClassClassificator::trainThetaMatrix, (void*)&args1);
			pthread_create(&thread2, NULL, &MultiClassClassificator::trainThetaMatrix, (void*)&args2);
			
			pthread_join(thread1, NULL);
			pthread_join(thread2, NULL);


			thetaMatrix1 = *(fmat*)args1.theta;
			thetaMatrix2 = *(fmat*)args2.theta;
			fmat allThetaMatrix =  join_cols(thetaMatrix1, thetaMatrix2);
			prediction = MultiClassClassificator::predictUsingThetaMatrix(allThetaMatrix, X1.row(testRow));

			end = clock();
			
		}
		else if (choice == 'a')
		{
			fmat allTheta1 = zeros<fmat>(5, X1.n_cols);
			fmat allTheta2 = zeros<fmat>(5,X2.n_cols);	
			//initialize arrays
			//float *array = convertArmadilloMatrixToNormalArray<float>(X);
			float *array1 = convertArmadilloMatrixToNormalArray<float>(X1);
			float *array2 = convertArmadilloMatrixToNormalArray<float>(X2);
			//int *arrayY = convertArmadilloMatrixToNormalArray<int>(y);
			int *arrayY1 = convertArmadilloMatrixToNormalArray<int>(y1);
			int *arrayY2 = convertArmadilloMatrixToNormalArray<int>(y2);
			float *allThetaArray1 = convertArmadilloMatrixToNormalArray<float>(allTheta1);
			float *allThetaArray2 = convertArmadilloMatrixToNormalArray<float>(allTheta2);
			float *X_row = convertArmadilloMatrixToNormalArray<float>(X1.row(testRow));
			float *predictVector = new float[10];

			TrainArgsASM *args1 = new TrainArgsASM();
			args1->X = array1;
			args1->X_row = (int)X1.n_rows;
			args1->X_col = (int)X1.n_cols;
			args1->Y = arrayY1;
			args1->from = 0;
			args1->to = 5;
			args1->allTheta = allThetaArray1;

			TrainArgsASM *args2 = new TrainArgsASM();
			args2->X = array2;
			args2->X_row = (int)X2.n_rows;
			args2->X_col = (int)X2.n_cols;
			args2->Y = arrayY2;
			args2->from = 5;
			args2->to = 10;
			args2->allTheta = allThetaArray2;
			
		
			begin = clock();
			pthread_attr_t attr;
			
			pthread_attr_init(&attr);

			pthread_attr_setstacksize(&attr, 80400000);
			
			pthread_create(&thread1,&attr,trainFunction, (void*)args1);
			pthread_create(&thread2,&attr,trainFunction, (void*)args2);
//			trainFunction(args1);	
//			trainFunction(args2);
			pthread_join(thread1, NULL);
			pthread_join(thread2, NULL);

			//merging allthetaarray
			
	//		porownaj(thetaMatrix1, allThetaArray1);

		//	return 0;


			float *allTheta = new float[10*401];
			float *copyArray = allThetaArray1;
			int counter;
			for (int i = 0 ; i < 10*401 ; i++)
			{
				counter = i;
				if (i>=2005)
				{	
					copyArray = allThetaArray2;
					counter = i - 2005;
				}
				allTheta[i] =  copyArray[counter];
			}
			predictFunction(X_row, allTheta, predictVector, 10, 401);
		
			float max = predictVector[0];
			
			std::cout << predictVector[0] << std::endl;
			for (int i = 1 ; i < 10 ; i++)
			{
				if (max < predictVector[i])
				{
					max = predictVector[i];
					prediction = i;
 				}
				std::cout << predictVector[i] << std::endl;
			}
			end = clock();			
			
			delete[] array1;
			delete[] array2;
			delete[] arrayY1;
			delete[] arrayY2;
			delete[] allTheta;
			delete[] predictVector;
			delete[] allThetaArray1;
			delete[] allThetaArray2;
			delete[] X_row;
		} 
		
		double elapsedSecs = (double)((end - begin)/1000) ;
		std::cout << "Na wytrenowanie macierzy potrzebne bylo " << std::setprecision(2) << elapsedSecs <<  "ms" << endl;
		std::cout << "Mysle ze pod tymi danymi kryje sie liczba: " << prediction << endl << endl;
	}


    fflush(stdout); 
    
	dlclose(handle); 
   
    return 0;
}

