
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

double MultiClassClassificator::costFunction(fmat theta, fmat X, fmat y, double lambda)
{
    int m = y.n_elem;
    fmat J = (-y.t() * log(sigmoidFunction(X*theta)) - (1-y).t()*log(1-sigmoidFunction(X*theta)))/m;
    cout << J;
    fflush(stdout);
    return J(0,0);
    
}

fmat MultiClassClassificator::trainThetaVector(fmat initialTheta, fmat X, fmat y, int numOfIterations, double lambda)
{
    int m = y.n_elem;
    for (int i = 0; i < numOfIterations; i++) {
//        costFunction(initialTheta.t(), X, y, 0);
        initialTheta -= ((sigmoidFunction(X*initialTheta.t()) - y).t()*X)/m;
    }
    return initialTheta;
}

fmat MultiClassClassificator::trainThetaMatrix(fmat X, fmat y, int num_labels, double lambda, fmat &allThetaTemp)
{
	
    numLabels = num_labels;
    allTheta = allThetaTemp; 
    fmat initialThetaVector = zeros<fmat>(1,X.n_cols);
    
    for(int i = 0 ; i < num_labels ; i++)
    {
        fmat yTemp = zeros<fmat>(y.n_elem,1);
        for (int j = 0; j < y.n_elem; j++) {
            yTemp(j) = y(j) == i+1; 
        }
        allTheta.row(i) = trainThetaVector(initialThetaVector, X, yTemp, 50,(double)1);
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


