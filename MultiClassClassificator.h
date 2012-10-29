//
//  MultiClassClassificator.h
//  ASMNeuralNetwork
//
//  Created by Kamil Badyla on 10.10.2012.
//  Copyright (c) 2012 badeleux. All rights reserved.
//

#ifndef ASMNeuralNetwork_MultiClassClassificator_h
#define ASMNeuralNetwork_MultiClassClassificator_h

#include "Common.h"

using namespace std;
using namespace arma;

class MultiClassClassificator {
    
private:
    //Methods
    static fmat trainThetaVector(fmat initialTheta, fmat X, fmat y, int numOfIterations);
    static fmat sigmoidFunction(fmat z);
    double costFunction(fmat theta, fmat X, fmat y);
public:
    static void* trainThetaMatrix(void *args);
    static int predictUsingThetaMatrix(fmat allTheta, fmat X);
};

#endif
