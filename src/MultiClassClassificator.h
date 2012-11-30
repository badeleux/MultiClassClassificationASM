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

/**
	It is a class which handles all of calculations. This class uses Armadillo library to make linear algebra calculations.
 */
class MultiClassClassificator {
    
private:
    /**
    	This method is mainly used by trainThetaMatrix.
        Calculates row for trained Theta Matrix
     
    	@param initialTheta is the initial vector usually 0 vector
    	@param X training matrix, size: MxN
    	@param y training matrix, size: Mx1
    	@param numOfIterations - usually it is an 50
    	@returns vector 1xN it is directly one of the rows of allThetaMatrix
     */
    static fmat trainThetaVector(fmat initialTheta, fmat X, fmat y, int numOfIterations);
    
    /**
     This Method calcualate sigmoidFunction
     @param z fmat with vector z
     @returns vector with sigmoid function
     */
    static fmat sigmoidFunction(fmat z);
    /**
     This method just calculate cost function.
     @param theta
     @param X
     @param y
     @returns Cost of Function
     */
    double costFunction(fmat theta, fmat X, fmat y);
public:
    /**
     trainThetaMatrix is ready for running on two cores, this is the reason why to param you should pass one parameter
     - pointer to structure.
     
     @param args is an struct of type TrainArgs. Look at Common.h
     */
    static void* trainThetaMatrix(void *args);
    /**
     This method return prediction for given trained matrix
     @param allTheta Trained theta Matrix it should be of size: MxN
     @param X matrix it should be of size: 1xN
     @returns prediction - which class is the most predictable
     */
    static int predictUsingThetaMatrix(fmat allTheta, fmat X);
};

#endif
