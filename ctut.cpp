#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

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

    namedWindow( "Display window", WINDOW_AUTOSIZE );    // Create display window
    imshow( "Display window", image );                   // Display image

    waitKey(0);                                          // Wait for user keystroke to exit
    return 0;
}

//compile using g++ -std=c++11 -I/usr/local/include/ -L/usr/local/lib -g -o EXEC_NAME main.cpp 
//-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d 
//-lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lopencv_imgcodecs