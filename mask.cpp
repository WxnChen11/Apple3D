#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv/cv.h>
#include <stdio.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

void Sharpen(const Mat& myImage, Mat& Result);

int main( int argc, char** argv )                       //get # arguments and argument strings from command line
{
    if( argc != 2)                                      //check input for single string
    {
     cout << "Please enter image to display" << endl;
     return -1;                                         //exit with error
    }

    Mat image;                                          //initialize image matrix
    image = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);   // Read the file, can also use COLOR and GRAYSCALE options

    if(! image.data )                                   // Check for valid image data
    {
        cout <<  "Image not found" << std::endl ;
        return -1;                                      //exit with error
    }

    Mat result;

    Sharpen(image, result);

    namedWindow("Image", WINDOW_AUTOSIZE);
    imshow("Image", result);


    // Mat gray_image;
    // cvtColor( image, gray_image, CV_BGR2GRAY);

    // imwrite( "Gray_Ford.jpg", gray_image);


    // namedWindow(argv[1], WINDOW_AUTOSIZE );
    // namedWindow( "Gray Image", WINDOW_AUTOSIZE );    // Create display window
    // imshow( argv[1], image );                   // Display image
    // imshow( "Gray Image", gray_image);

    waitKey(0);                                          // Wait for user keystroke to exit
    return 0;
}

void Sharpen(const Mat& myImage, Mat& Result){

    Mat kern = (Mat_<char>(3,3) <<  0, -1, 0,
                                    -1, 5, -1,
                                    0, -1, 0);

    filter2D(myImage, Result, myImage.depth(), kern);


}


//compile using g++ -std=c++11 -I/usr/local/include/ -L/usr/local/lib -g -o EXEC_NAME main.cpp 
//-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d 
//-lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lopencv_imgcodecs