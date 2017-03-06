#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv/cv.h>
#include <stdio.h>
#include <opencv/highgui.h>
#include <vector>

using namespace cv;
using namespace std;

int main (){


    vector <unsigned char>* Red;

    Red = new vector <unsigned char> (10);

    Red -> at(2) = 3;

    vector <unsigned char>* Red_Copy;
    vector <unsigned char> Red_Copy_Actual;

    Red_Copy = Red;
    // Red_Copy_Actual = *Red;
    // Red_Copy = &Red_Copy_Actual;

    cout << (int)Red_Copy -> at(2) << endl;
    Red_Copy -> at(2) = 6;
    cout << (int)Red -> at(2) << endl;
    cout << (int)Red_Copy -> at(2) << endl;
    return 0;
}