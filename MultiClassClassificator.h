//
//  MultiClassClassificator.h
//  ASMNeuralNetwork
//
//  Created by Kamil Badyla on 10.10.2012.
//  Copyright (c) 2012 badeleux. All rights reserved.
//

#ifndef ASMNeuralNetwork_MultiClassClassificator_h
#define ASMNeuralNetwork_MultiClassClassificator_h

#include </usr/include/armadillo>

using namespace std;
using namespace arma;

class MultiClassClassificator {
    
private:
    //Variables
    mat allTheta;
    int numLabels;
    //Methods
    mat trainThetaVector(mat initialTheta, mat X, mat y, int numOfIterations, double lambda);
    mat sigmoidFunction(mat z);
    double costFunction(mat theta, mat X, mat y, double lambda);
public:
    
    mat trainThetaMatrix(mat X, mat y, int num_labels, double lambda);
    int predictUsingThetaMatrix(mat X);
};

#endif
