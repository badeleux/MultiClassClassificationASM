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
    fmat allTheta;
    int numLabels;
    //Methods
    fmat trainThetaVector(fmat initialTheta, fmat X, fmat y, int numOfIterations, double lambda);
    fmat sigmoidFunction(fmat z);
    double costFunction(fmat theta, fmat X, fmat y, double lambda);
public:
    
    fmat trainThetaMatrix(fmat X, fmat y, int num_labels, double lambda, fmat&);
    int predictUsingThetaMatrix(fmat X);
};

#endif
