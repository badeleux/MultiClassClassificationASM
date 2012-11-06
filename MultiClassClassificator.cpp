
//
//  MultiClassClasificator.cpp
//  ASMMultiClassClassificator
//
//  Created by Kamil Badyla on 10.10.2012.
//  Copyright (c) 2012 badeleux. All rights reserved.
//

#include <iostream>
#include "MultiClassClassificator.h"

fmat MultiClassClassificator::sigmoidFunction(fmat z)
{
    return 1/(1+exp(-z));
}

double MultiClassClassificator::costFunction(fmat theta, fmat X, fmat y)
{
    int m = y.n_elem;
    fmat J = (-y.t() * log(sigmoidFunction(X*theta)) - (1-y).t()*log(1-sigmoidFunction(X*theta)))/m;
    cout << J;
    fflush(stdout);
    return J(0,0);
    
}

fmat MultiClassClassificator::trainThetaVector(fmat initialTheta, fmat X, fmat y, int numOfIterations)
{
    int m = y.n_elem;
    for (int i = 0; i < numOfIterations; i++) {
	  initialTheta -= ((sigmoidFunction(X*initialTheta.t()) - y).t()*X)/m;
	}
    return initialTheta;
}

void* MultiClassClassificator::trainThetaMatrix(void *args)
{
	TrainArgs *arguments = (TrainArgs*)args;

	fmat X = *((fmat*)arguments->X);
	fmat y = *((fmat*)arguments->y);
	int from = arguments->from;
	int to = arguments->to;
	fmat *allTheta = (fmat*)(arguments->theta);
    for(int i = from ; i < to ; i++)
    {
    	fmat initialThetaVector = zeros<fmat>(1,X.n_cols);
        fmat yTemp = zeros<fmat>(y.n_elem,1);
        for (unsigned int j = 0; j < y.n_elem; j++) {
            yTemp(j) = y(j) == i; 
        }
		
        allTheta->row(i-from) = trainThetaVector(initialThetaVector, X, yTemp, 50);
	}
    return 0;
}

int MultiClassClassificator::predictUsingThetaMatrix(fmat allTheta, fmat X)
{
    fmat predictVector = zeros<fmat>(allTheta.n_rows, 1);
    predictVector = allTheta * X.t();
	
	std::cout << predictVector;
	float maxElement = predictVector.max();
	for ( unsigned int i = 0 ; i < predictVector.n_elem ; i++)
		if (predictVector(i) == maxElement)
			return i;
	return 0;
}


