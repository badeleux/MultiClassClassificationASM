
//
//  MultiClassClasificator.cpp
//  ASMMultiClassClassificator
//
//  Created by Kamil Badyla on 10.10.2012.
//  Copyright (c) 2012 badeleux. All rights reserved.
//

#include <iostream>
#include "MultiClassClassificator.h"

mat MultiClassClassificator::sigmoidFunction(mat z)
{
    return 1/(1+exp(-z));
}

double MultiClassClassificator::costFunction(mat theta, mat X, mat y, double lambda)
{
    int m = y.n_elem;
    mat J = (-y.t() * log(sigmoidFunction(X*theta)) - (1-y).t()*log(1-sigmoidFunction(X*theta)))/m;
    cout << J;
    fflush(stdout);
    return J(0,0);
    
}

mat MultiClassClassificator::trainThetaVector(mat initialTheta, mat X, mat y, int numOfIterations, double lambda)
{
    int m = y.n_elem;
    for (int i = 0; i < numOfIterations; i++) {
//        costFunction(initialTheta.t(), X, y, 0);
        mat regularizationTerm = initialTheta;
        regularizationTerm(0) = 0;
        initialTheta -= ((sigmoidFunction(X*initialTheta.t()) - y).t()*X)/m + lambda*regularizationTerm/m;
    }
    return initialTheta;
}

mat MultiClassClassificator::trainThetaMatrix(mat X, mat y, int num_labels, double lambda)
{
    numLabels = num_labels;
    allTheta = zeros<mat>(num_labels, X.n_cols+1);
    mat initialThetaVector = zeros<mat>(1,X.n_cols+1);
    
    mat one_vector = ones<mat>(X.n_rows,1);
    X = join_rows(one_vector, X);
    
    for(int i = 0 ; i < num_labels ; i++)
    {
        mat yTemp = zeros<mat>(y.n_elem,1);
        for (int j = 0; j < y.n_elem; j++) {
            yTemp(j) = y(j) == i+1; 
        }
        allTheta.row(i) = trainThetaVector(initialThetaVector, X, yTemp, 50,1);
    }
    return allTheta;
}

int MultiClassClassificator::predictUsingThetaMatrix(mat X)
{
    mat predictVector = zeros(numLabels, 1);
    predictVector = allTheta * X.t();
    cout << predictVector;
    return predictVector(0);
    
}


