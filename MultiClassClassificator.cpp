
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
//        costFunction(initialTheta.t(), X, y, 0);
//
		fmat tranposeX = X.t();
	fmat firstMultiply = X*initialTheta.t();
		fmat sigmoid = sigmoidFunction(X*initialTheta.t());
	fmat next = sigmoid - y;
		fmat next2 = next.t() * X;
//	if (i==1)
//		return firstMultiply;	
        initialTheta -= ((sigmoidFunction(X*initialTheta.t()) - y).t()*X)/m;
		//std::cout << "Po pierwszej iteracji: "<< firstMultiply(17) << " "<< sigmoid(17) << " "<< next(17) << " "<< next2(1) << " " << tranposeX(0,17);
		//fflush(stdout);
		//return sigmoid;
}
    return initialTheta;
}

fmat MultiClassClassificator::trainThetaMatrix(fmat X, fmat y, int num_labels, fmat &allThetaTemp)
{
	
    numLabels = num_labels;
    allTheta = allThetaTemp; 

    
    for(int i = numLabels ; i > 0 ; i--)
    {
		
    	fmat initialThetaVector = zeros<fmat>(1,X.n_cols);
        fmat yTemp = zeros<fmat>(y.n_elem,1);
        for (unsigned int j = 0; j < y.n_elem; j++) {
            yTemp(j) = y(j) == i; 
        }
//		return trainThetaVector(initialThetaVector, X, yTemp, 50);
        allTheta.row(i-1) = trainThetaVector(initialThetaVector, X, yTemp, 50);
    }
    return allTheta;
}

int MultiClassClassificator::predictUsingThetaMatrix(fmat X)
{
    fmat predictVector = zeros<fmat>(numLabels, 1);
    predictVector = allTheta * X.t();
    cout << predictVector;
    return predictVector(0);
    
}


