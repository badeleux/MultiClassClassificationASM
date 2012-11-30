//
//  Image.h
//  ASMNeuralNetwork
//
//  Created by Kamil Badyla on 09.10.2012.
//  Copyright (c) 2012 3Soft S.A. All rights reserved.
//

#ifndef IMAGE_H
#define IMAGE_H
#include <armadillo>

using namespace arma;
using namespace std;
class Image
{
public:
    /**
      Creates an Image 0x0
    	@returns Image
     */
    Image();

    /**
     Creates an Image of numRows x numCols and creates the arrays for it
    	@param numRows
    	@param numCols
    	@param grayLevels
    	@returns 
     */
    Image(int numRows, int numCols, int grayLevels);
    
    /**
     destroy image
     */
    ~Image();
    
    /**
     copies oldImage into whatever you = it to
    	@param Image& 
     */
    void operator=(const Image&);
    /**
    	sets the number of rows, columns and graylevels
    	@param numRows
    	@param numCols
    	@param maxVal 
     */
    void setImageInfo(int numRows, int numCols, int maxVal);
    /**
    	returns the number of rows, columns and gray levels
    	@param numRows
    	@param numCols
    	@param maxVal 
     */
    void getImageInfo(int &numRows, int &numCols, int &maxVal);
    /**
    	returns the gray value of a specific pixel
    	@param row
    	@param col
    	@returns 
     */
    int getPixelVal(int row, int col);
    /**
    	Get pixel VECTOR!
    	@returns
     */
    fmat getPixelMatrix();
    /**
    	sets the gray value of a specific pixel
    	@param row
    	@param col
    	@param value 
     */
    void setPixelVal(int row, int col, int value);
    /**
    	read image from file
    	@param os
    	@param image
    	@returns 
     */
    friend ostream& operator<<(ostream& os, const Image& image);
    
    /**
    	Read image header
    	@param char name of file
    	@param int& reference to number of rows
    	@param int& reference to number of cols
    	@param int& reference to gray scale
    	@param bool& reference to type bool
    	@returns if returned 1 all went good
     */
    static int readImageHeader(char[], int&, int&, int&, bool&);
    /**
    	Read Image
    	@param char name of file
    	@param Image& Image reference
    	@returns if returned 1 all went good
     */
    static int readImage(char[], Image&);
    /**
    	Write to Image
    	@param char name of image
    	@param Image& Image to write.
    	@returns if returned 1 all went good
     */
    static int writeImage(char[], Image&);
private:
    int N; // number of rows
    int M; // number of columns
    int Q; // number of gray levels
    double **pixelVal;
    /**
    	scaleIntensity, scale gray values by 255.0f
     */
    void scaleIntensity();
};

#endif
