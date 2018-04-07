/*
lab2.cpp
Skyler Penna
CS 315: Lab Sec 2 (T 2:00-5:00)

The objective of this lab assignment is to gain practice using the EasyBMP library to alter and create .bmp's
*/
#include <iostream>
#include "EasyBMP.h"

using namespace std;

//part1 functions:
void makeChecker(BMP & Output);

//part2 functions:
void makeSierpinski(BMP & Output, int X[], int Y[], int Z[], int P[], int posn);

//part3 functions:
void mainDiag(BMP & mainImg, BMP & smallImg, int inW, int inH);
void resize(BMP & smallImg);
void popLeftAdj(BMP & mainImg, BMP & smallImg, int inW, int inH, int toL);
void popLowerAdj(BMP & mainImg, BMP & smallImg, int inW, int inH, int toL);

int main( int argc, char* argv[]) {
    //Part 1:
    BMP board;
    board.SetSize(256, 256);
    board.SetBitDepth(8);

    makeChecker(board);
    board.WriteToFile("board.bmp");

    //Part 2: 
    BMP sierpinski;
    int posn = 10000;

    sierpinski.SetSize(256, 256);
    sierpinski.SetBitDepth(8);

    int X[] = {128, 5};
    int Y[] = {5, 251};
    int Z[] = {251, 251};
    int P[] = {171, 34};

    makeSierpinski(sierpinski, X, Y, Z, P, posn);

    sierpinski.WriteToFile("sierpinski.bmp");

    //Part 3:

    string input;
    cout << "Please input the file name (Ex: 'einstein.bmp'): " << endl;
    cin >> input;
    BMP mainImg; 
    BMP in;
    in.ReadFromFile(input.c_str());

    mainImg.SetSize(512, 512);
    mainImg.SetBitDepth(16);

    int inW = 0, inH = 0;

    mainDiag(mainImg, in, inW, inH);

    mainImg.WriteToFile("mainImg.bmp");
    
    return 0;
}
//part 1: loops through the output image.  
//If the particular pixel divided by 32 is divisible by 2 then the pixel is set to black.
void makeChecker(BMP & Output) {
    int picWidth = Output.TellWidth();
    int picHeight = Output.TellHeight();

    for (int i = 0; i < picWidth; i++) {
        for (int j = 0; j < picHeight; j++) {
            
            if (((i/32)+(j/32))%2) {
                Output(i, j)->Red = 0;
                Output(i, j)->Green = 0;  
                Output(i, j)->Blue = 0;      
            } else {
                Output(i, j)->Red = 255;
                Output(i, j)->Green = 0;
                Output(i, j)->Blue = 0;
            
            }
        }
    }
}

//Part 2: Rolls a random number between 0 and 3.  Based on the number chose the corresponding point:
//X, Y, or Z is chosen.  P is move to the position between the chosen point and the function is called
//again with 1 less "posn"
void makeSierpinski(BMP & Output, int X[], int Y[], int Z[], int P[], int posn) {

    int point = (rand()%3);
    
    if (posn == 0 ) {
        return;
    }
    else if (point == 0) {

        P[0] = ((X[0]+P[0])/2);
        P[1] = ((X[1]+P[1])/2);
        
        Output(P[0], P[1])->Red = 255;
        Output(P[0], P[1])->Green = 0;
        Output(P[0], P[1])->Blue = 0;
        
        makeSierpinski(Output, X, Y, Z, P, posn-1);
    }
    else if (point == 1) {
        P[0] = ((Y[0]+P[0])/2);
        P[1] = ((Y[1]+P[1])/2);
        
        Output(P[0], P[1])->Red = 255;
        Output(P[0], P[1])->Green = 0;
        Output(P[0], P[1])->Blue = 0;
        
        makeSierpinski(Output, X, Y, Z, P, posn-1);
    } else {
        P[0] = ((Z[0]+P[0])/2);
        P[1] = ((Z[1]+P[1])/2);
        
        Output(P[0], P[1])->Red = 255;
        Output(P[0], P[1])->Green = 0;
        Output(P[0], P[1])->Blue = 0;
        
        makeSierpinski(Output, X, Y, Z, P, posn-1);
    }
}

/*
Takes the input image and called the resize() function on it.  When the new, smaller, image is generated
copies the image onto the first unpopulated bottom left corner.  then calls the two adjacent functions,
When the adjacent functions are done running mainDiag() is recursively called.
*/
void mainDiag(BMP & mainImg, BMP & smallImg, int inW, int inH) {
    
    if (smallImg.TellHeight() < 2) {
        
        for (int i = 0; i < mainImg.TellHeight(); i++) {
            PixelToPixelCopy(smallImg, 0, 0, mainImg, i, 0);
            int temp = mainImg.TellHeight()-1;
            PixelToPixelCopy(smallImg, 0, 0, mainImg, temp, i);
        }
        return;
    } else {

        resize(smallImg);

        inH += smallImg.TellHeight();                

        int fromL = 0, fromR = smallImg.TellWidth(), fromB = smallImg.TellHeight(), fromT = 0;
        int toL = (mainImg.TellHeight()-inH);

        RangedPixelToPixelCopy(smallImg, fromL, fromR, fromB, fromT, mainImg, inW, toL);

        inW += smallImg.TellWidth();
        popLeftAdj(mainImg, smallImg, inW, inH, toL);
        popLowerAdj(mainImg, smallImg, inW, inH, toL);
        mainDiag(mainImg, smallImg, inW, inH);
    }
}
/*
Resizes the input image in half.  Combines 4 pixel RGB values into 1 pixel.
*/
void resize(BMP & smallImg) {
    BMP temp;

    int w = (smallImg.TellWidth()/2);
    int h = (smallImg.TellHeight()/2);

    temp.SetSize(w, h);

    for (int i = 0; i < temp.TellWidth(); i++) {
        for (int j = 0; j < temp.TellHeight(); j++) {
            temp(i,j)->Red = ((smallImg(2*i,2*j)->Red + smallImg(2*i+1,2*j)->Red + smallImg(2*i, 2*j+1)->Red + smallImg(2*i+1, 2*j+1)->Red)/4);
            temp(i,j)->Green = ((smallImg(2*i,2*j)->Green + smallImg(2*i+1,2*j)->Green + smallImg(2*i, 2*j+1)->Green + smallImg(2*i+1, 2*j+1)->Green)/4);
            temp(i,j)->Blue = ((smallImg(2*i,2*j)->Blue + smallImg(2*i+1,2*j)->Blue + smallImg(2*i, 2*j+1)->Blue + smallImg(2*i+1, 2*j+1)->Blue)/4);
        }
    }

    smallImg.SetSize(w, h);
    RangedPixelToPixelCopy(temp, 0, w, h, 0, smallImg, 0, 0);
}

/*
populates the squares to the left of the corresponding diagonal image.  Squares are of the same size
as the diagonal image.
*/
void popLeftAdj(BMP & mainImg, BMP & smallImg, int inW, int inH, int toL) {
    int tempW = inW-smallImg.TellWidth();

    if (tempW < 0) {
        return;
    } else {
        int tfL = 0, tfR = smallImg.TellWidth(), tfB = smallImg.TellHeight(), tfT = 0;
        RangedPixelToPixelCopy(smallImg, tfL, tfR, tfB, tfT, mainImg, tempW, toL);
        popLeftAdj(mainImg, smallImg, tempW, inH, toL);
    }
}

/*
Does the same as popLeftAdj but for the squares below the diagonal.
*/
void popLowerAdj(BMP & mainImg, BMP & smallImg, int inW, int inH, int toL) {
    int tempH = toL+smallImg.TellWidth();

    if (tempH > mainImg.TellHeight()) {
        return;
    } else {
        int tfL = 0, tfR = smallImg.TellWidth(), tfB = smallImg.TellHeight(), tfT = 0;
        RangedPixelToPixelCopy(smallImg, tfL, tfR, tfB, tfT, mainImg, inW-smallImg.TellHeight(), tempH);
        popLowerAdj(mainImg, smallImg, inW, inH, tempH);
    }
}
