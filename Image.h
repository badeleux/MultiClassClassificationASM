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
    Image();
    Image(int numRows, int numCols, int grayLevels);
    ~Image();
    Image(const Image& oldImage);
    void operator=(const Image&);
    void setImageInfo(int numRows, int numCols, int maxVal);
    void getImageInfo(int &numRows, int &numCols, int &maxVal);
    int getPixelVal(int row, int col);
    mat getPixelMatrix();
    void setPixelVal(int row, int col, int value);
    bool inBounds(int row, int col);
    void getSubImage(int upperLeftRow, int upperLeftCol, 
                     int lowerRightRow, int lowerRightCol, Image& oldImage);
    friend ostream& operator<<(ostream& os, const Image& image);
    int meanGray();
    void enlargeImage(int value, Image& oldImage);
    void shrinkImage(int value, Image& oldImage);
    void reflectImage(bool flag, Image& oldImage);
    void translateImage(int value, Image& oldImage);
    /*
     r' = r + t
     c' = c + t
     */
    void rotateImage(int theta, Image& oldImage);
    Image operator+(const Image &oldImage);
    Image operator-(const Image& oldImage);
    void negateImage(Image& oldImage);
    
    static int readImageHeader(char[], int&, int&, int&, bool&);
    static int readImage(char[], Image&);
    static int writeImage(char[], Image&);
private:
    int N; // number of rows
    int M; // number of columns
    int Q; // number of gray levels
    double **pixelVal;
    void scaleIntensity();
};

#endif
