//
//  Image.cpp
//  ASMNeuralNetwork
//
//  Created by Kamil Badyla on 09.10.2012.
//  Copyright (c) 2012 3Soft S.A. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include "Image.h"
#include <cmath>
#include <fstream>
using namespace std;

Image::Image()
{
    N = 0;
    M = 0;
    Q = 0;
    
    pixelVal = NULL;
}

Image::Image(int numRows, int numCols, int grayLevels)
{    
    
    N = numRows;
    M = numCols;
    Q = grayLevels;
    
    pixelVal = new double *[N];
    for(int i = 0; i < N; i++)
    {
        pixelVal[i] = new double [M];
        for(int j = 0; j < M; j++)
            pixelVal[i][j] = 0;
    }
}

Image::~Image()
{
    N = 0;
    M = 0;
    Q = 0;
    
    for(int i = 0; i < N; i++)
        delete pixelVal [N];
    
    delete pixelVal;
}

void Image::operator=(const Image& oldImage)
{
    N = oldImage.N;
    M = oldImage.M;
    Q = oldImage.Q;
    
    pixelVal = new double* [N];
    for(int i = 0; i < N; i++)
    {
        pixelVal[i] = new double [M];
        for(int j = 0; j < M; j++)
            pixelVal[i][j] = oldImage.pixelVal[i][j];
    }
}

void Image::setImageInfo(int numRows, int numCols, int maxVal)
{
    N = numRows;
    M = numCols;
    Q = maxVal;
}

void Image::getImageInfo(int &numRows, int &numCols, int &maxVal)
{
    numRows = N;
    numCols = M;
    maxVal = Q;
}

int Image::getPixelVal(int row, int col)
{
    return pixelVal[row][col];
}

fmat Image::getPixelMatrix()
{
    fmat pixelMatrix = zeros<fmat>(1, N*M);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j<M; j++) {
            pixelMatrix(j + i*M) = pixelVal[i][j];
        }
    }
    return pixelMatrix;
}


void Image::setPixelVal(int row, int col, int value)
{
    pixelVal[row][col] = value;
}


ostream& operator<<(ostream& os, const Image& image)
{
    cout << "Image has " << image.N << " rows and " << image.M << " cols.";
    for (int i = 0; i < image.N; i++) {
        for (int j = 0; j < image.M; j++) {
            os << image.pixelVal[i][j] << " ";
        }
        cout << endl;
    }
    return os;
}

int Image::readImage(char fname[], Image& image)
{
    int i, j;
    int N, M;
    unsigned char *charImage;
    char header [100], *ptr;
    ifstream ifp;
    
    ifp.open(fname, ios::in | ios::binary);
    
    if (!ifp) 
    {
        cout << "Can't read image: " << fname << endl;
        exit(1);
    }
    
    // read header
    
    ifp.getline(header,100,'\n');
    if ( (header[0]!=80) || (header[1]!=53) ) 
    {   
        cout << "Image " << fname << " is not PGM" << endl;
        exit(1);
    }
    
    ifp.getline(header,100,'\n');
    while(header[0]=='#')
        ifp.getline(header,100,'\n');
    
    M=strtol(header,&ptr,0);
    N=atoi(ptr);
    
    charImage = (unsigned char *) new unsigned char [M*N];
    ifp.seekg(13);
    ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    
    if (ifp.fail()) 
    {
        cout << "Image " << fname << " has wrong size" << endl;
        exit(1);
    }
    
    ifp.close();
    
    //
    // Convert the unsigned characters to integers
    //
    
    int val;
    
    for(i=0; i<N; i++)
        for(j=0; j<M; j++) 
        {
            val = (int)charImage[i*M+j];
            image.setPixelVal(i, j, val);     
        }
    
    delete [] charImage;
    image.scaleIntensity();
    
    
    
    return (1);
    
}

void Image::scaleIntensity()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
        	pixelVal[i][j] = pixelVal[i][j]/255.0f;
		//	if (pixelVal[i][j] == 0)
		//	pixelVal[i][j] += 0.1059;
		}
    }
}


int Image::readImageHeader(char fname[], int& N, int& M, int& Q, bool& type)
{
    char header [100], *ptr;
    ifstream ifp;
    
    ifp.open(fname, ios::in | ios::binary);
    
    if (!ifp) 
    {
        cout << "Can't read image: " << fname << endl;
        exit(1);
    }
    
    // read header
    
    type = false; // PGM
    
    ifp.getline(header,100,'\n');
    if ( (header[0] == 80) && (header[1]== 53) ) 
    {  
        type = false;
    }
    else if ( (header[0] == 80) && (header[1] == 54) ) 
    {       
        type = true;
    } 
    else 
    {
        cout << "Image " << fname << " is not PGM or PPM" << endl;
        exit(1);
    }
    
    ifp.getline(header,100,'\n');
    while(header[0]=='#')
        ifp.getline(header,100,'\n');
    
    M=strtol(header,&ptr,0);
    N=atoi(ptr);
    
    ifp.getline(header,100,'\n');
    
    Q=strtol(header,&ptr,0);
    
    ifp.close();
    
    return(1);
}

int Image::writeImage(char fname[], Image& image)
{
    int i, j;
    int N, M, Q;
    unsigned char *charImage;
    ofstream ofp;
    
    image.getImageInfo(N, M, Q);
    
    charImage = (unsigned char *) new unsigned char [M*N];
    
    // convert the integer values to unsigned char
    
    int val;
    
    for(i=0; i<N; i++)
    {
        for(j=0; j<M; j++) 
        {
            val = image.getPixelVal(i, j);
            charImage[i*M+j]=(unsigned char)val;
        }
    }
    
    ofp.open(fname, ios::out | ios::binary);
    
    if (!ofp) 
    {
        cout << "Can't open file: " << fname << endl;
        exit(1);
    }
    
    ofp << "P5" << endl;
    ofp << M << " " << N << endl;
    ofp << Q << endl;
    
    ofp.write( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    
    if (ofp.fail()) 
    {
        cout << "Can't write image " << fname << endl;
        exit(0);
    }
    
    ofp.close();
    
    delete [] charImage;
    
    return(1);
    
}

