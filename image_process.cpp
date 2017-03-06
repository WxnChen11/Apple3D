//Apple 3D Vision Intern Coding Assignment
//Winston Chen
//Mar 6, 2017

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv/cv.h>
#include <stdio.h>
#include <opencv/highgui.h>
#include <vector>
#include <string>
#include <stdlib.h>

namespace ImageProc{
    using namespace cv;
    using namespace std;

    class Image{

        private:
            vector <unsigned char>* Red; //store intensities in separate channels
            vector <unsigned char>* Green;
            vector <unsigned char>* Blue;
            vector <unsigned char>* Gray; //including grayscale channel
            int size;
            string type;
            string name;
            int height;
            int width;
            
        public:

            vector <unsigned char>* getRed() const { return Red;}
            vector <unsigned char>* getGreen() const { return Green;}
            vector <unsigned char>* getBlue() const { return Blue;}
            vector <unsigned char>* getGray() const { return Gray;}
            int getSize() const { return size;}
            string getType() const { return type;}
            string getName() const { return name;}
            int getHeight() const { return height;}
            int getWidth() const { return width;}
            void resize(int, int);
            void displayHist(int);
            void displayImage(string);
            Image();
            Image(string);
            Image(const Image&, string);
            ~Image();

    };

    Image::Image(){
        //default image, for testing
        this -> Red = new vector<unsigned char>(25);
        this -> Green = new vector<unsigned char>(25);
        this -> Blue = new vector<unsigned char>(25);
        this -> Gray = new vector<unsigned char>(25);
        this -> size = 25*25;
        this -> name = "Image";
        this -> type = "RBG"; //by default
        this -> height = 25;
        this -> width = 25;
    }

    Image::Image(string filename){ //load image from file

        Mat im;
        im = imread(filename, CV_LOAD_IMAGE_COLOR); //get rid of alpha channel, only want RGB

        this -> size = im.cols * im.rows;
        this -> width = im.cols;
        this -> height = im.rows;
        this -> name = filename.c_str();

        cout << "loading image: " << this->name << " | width: " << this -> width << " | height: " << this -> height << endl;

        this -> Red = new vector<unsigned char>(this -> size); //every channel requires size w x h
        this -> Green = new vector<unsigned char>(this -> size);
        this -> Blue = new vector<unsigned char>(this -> size);
        this -> Gray = new vector<unsigned char>(this -> size);

        bool grayscale = true; //check if picture is grayscale

        int i = 0;
        
        for(int x = 0; x < im.rows; x++){ //note x represents vertical direction

            for(int y = 0; y < im.cols; y++){ //y represents horizontal

                unsigned char b = im.at<Vec3b>(x,y)[0]; //data in BGR format so read from index 2 to 0 for RGB
                unsigned char g = im.at<Vec3b>(x,y)[1];
                unsigned char r = im.at<Vec3b>(x,y)[2];

                // cout << (int)r << endl;

                if (!(abs(b-g) < 10 && abs(b-r) < 10 && abs(r-g) < 10)){
                    grayscale = false; //basically if RBG values are too far apart, not a grayscale image. 
                }

                (this -> Blue) -> at(i) = b;
                (this -> Green) -> at(i) = g;
                (this -> Red) -> at(i) = r;
                (this -> Gray) -> at(i) = (unsigned char) (0.2126*r + 0.7152*g + 0.0722*b); //from https://en.wikipedia.org/wiki/Grayscale#Colorimetric_.28luminance-preserving.29_conversion_to_grayscale

                i++;
            }
        }
        
        if (grayscale == true){
            this -> type = "grayscale";
        }
        else{
            this -> type = "RGB"; //if not grayscale, then RGB
        }
        
        // cout << (int)(this -> Red) -> at(3) << " " << (int)(this -> Red) -> at(4) << " " << (int)(this -> Red) -> at(5) << " " << (int)(this -> Red) -> at(6) << endl;

    }

    void Image::displayHist(int bins){

        int hist_w = 512; int hist_h = 400; //choose 512 as convenient width for 256 bins

        int r_hist_a[256] = {0}; //allocate histogram arrays
        int g_hist_a[256] = {0};
        int b_hist_a[256] = {0};
        int gr_hist_a[256] = {0};

        for (int i = 0; i < this->size; i++){ //get the number of occurences of each value
            r_hist_a[this->Red->at(i)] ++;
            g_hist_a[this->Green->at(i)] ++;
            b_hist_a[this->Blue->at(i)] ++;
            gr_hist_a[this->Gray->at(i)] ++;
        }      

        int range = (int)256/bins; //get the range from bin numbers

        int r_sum = 0; //temporary values to keep track of sum
        int g_sum = 0;
        int b_sum = 0;
        int gr_sum = 0;

        for(int i = 0; i < 256; i++){
            
            r_sum += r_hist_a[i]; //add to moving sum
            g_sum += g_hist_a[i];
            b_sum += b_hist_a[i];
            gr_sum += gr_hist_a[i];

            if ((i+1)%range == 0){ //time to assign the sum to the values within the bin range
                for(int j = i+1-range; j < i+1; j++){
                    r_hist_a[j] = r_sum;
                    g_hist_a[j] = g_sum;
                    b_hist_a[j] = b_sum;
                    gr_hist_a[j] = gr_sum;
                }
                r_sum = 0; //reset sums
                g_sum = 0;
                b_sum = 0;
                gr_sum = 0;
            }
        }

        Mat b_hist = Mat(1, 256, CV_32SC1, b_hist_a); //change to CV_32SC1 to match signed int!!
        Mat r_hist = Mat(1, 256, CV_32SC1, r_hist_a);
        Mat g_hist = Mat(1, 256, CV_32SC1, g_hist_a);
        Mat gr_hist = Mat(1, 256, CV_32SC1, gr_hist_a);

        int bin_w = cvRound( (double) hist_w/bins ); //get the width of each bin

        Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) ); //initialize histogram img

        normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() ); //normalize, can also manually find max but this is faster
        normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        normalize(gr_hist, gr_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

        for( int i = 1; i < bins; i++ ) //draw the histogram. bottom of the screen is height hist_h!
        {
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<int>(i-1)) ) ,
                            Point( bin_w*(i), hist_h - cvRound(b_hist.at<int>(i)) ),
                            Scalar( 255, 0, 0), 2, 8, 0  );
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<int>(i-1)) ) ,
                            Point( bin_w*(i), hist_h - cvRound(g_hist.at<int>(i)) ),
                            Scalar( 0, 255, 0), 2, 8, 0  );
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<int>(i-1)) ) ,
                            Point( bin_w*(i), hist_h - cvRound(r_hist.at<int>(i)) ),
                            Scalar( 0, 0, 255), 2, 8, 0  );
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(gr_hist.at<int>(i-1)) ) ,
                            Point( bin_w*(i), hist_h - cvRound(gr_hist.at<int>(i)) ),
                            Scalar( 125, 125, 125), 2, 8, 0  );
        }

        namedWindow(this->name + " HISTOGRAM", CV_WINDOW_AUTOSIZE );
        imshow(this->name + " HISTOGRAM", histImage );

        waitKey(0);

    }

    Image::Image(const Image& copyfrom, string type){

        try{

        if (type == "deep"){ //deep copy: instantiate new vector, copy contents (vector automatically performs deep copy)
            vector <unsigned char>* r = new vector <unsigned char>(copyfrom.getSize());
            *r = *(copyfrom.getRed());
            this -> Red = r;
            vector <unsigned char>* g = new vector <unsigned char>(copyfrom.getSize());
            *g = *(copyfrom.getGreen());
            this -> Green = g;
            vector <unsigned char>* b = new vector <unsigned char>(copyfrom.getSize());
            *b = *(copyfrom.getBlue());
            this -> Blue = b;
            vector <unsigned char>* gr = new vector <unsigned char>(copyfrom.getSize());
            *gr = *(copyfrom.getGray());
            this -> Gray = gr;
            this -> size = copyfrom.getSize();
            this -> type = copyfrom.getType().c_str(); //c_str to deep copy string
            this -> name = copyfrom.getName().c_str();
            this -> height = copyfrom.getHeight();
            this -> width = copyfrom.getWidth();
        }
        else if (type == "shallow"){
            this -> Red = copyfrom.getRed();
            this -> Green = copyfrom.getGreen();
            this -> Blue = copyfrom.getBlue();
            this -> Gray = copyfrom.getGray();
            this -> size = copyfrom.getSize();                  //attributes below and including size will be deep-copied.
            this -> type = copyfrom.getType().c_str();          //for a complete shallow copy,
            this -> name = copyfrom.getName().c_str();          //simply use Image* new_Img = old_Img;
            this -> height = copyfrom.getHeight();              //assuming old_Img is declared using
            this -> width = copyfrom.getWidth();                //Image* old_Img = new Image();
        }

        else{
            throw 10;
        }
        }
        catch (int e){
            cout << "Exception: Did not specify -string: type- in Image::Image(const Image& copyfrom, string type). Options: deep, shallow. Exception No. " << e << '\n';
        }
    }

    Image::~Image(){
        cout << "Image " << this->name << " has been deleted" << endl; //show which image has been deleted
    }

    void Image::displayImage(string colour){

        try{

        Mat im(this->getHeight(), this->getWidth(), CV_8UC3, Scalar(0,0,0)); //initialize Mat

        if (colour == "RGB"){

            int i = 0;

            for(int x = 0; x < this->getHeight(); x++){

                for(int y = 0; y < this->getWidth(); y++){

                    im.at<Vec3b>(x,y)[0] = this->getBlue()->at(i); //http://docs.opencv.org/2.4.13.2/doc/user_guide/ug_mat.html
                    im.at<Vec3b>(x,y)[1] = this->getGreen()->at(i);
                    im.at<Vec3b>(x,y)[2] = this->getRed()->at(i);

                    i++;
                }
            }

        }
        else if (colour == "GRAYSCALE"){ //grayscale, rgb values are all the same value

            int i = 0;

            for(int x = 0; x < this->getHeight(); x++){

                for(int y = 0; y < this->getWidth(); y++){

                    unsigned char gray = this->getGray()->at(i);

                    im.at<Vec3b>(x,y)[0] = gray; //http://docs.opencv.org/2.4.13.2/doc/user_guide/ug_mat.html
                    im.at<Vec3b>(x,y)[1] = gray;
                    im.at<Vec3b>(x,y)[2] = gray;

                    i++;
                }
            }

        }
        else{
            throw 12;
        }

        namedWindow(this->getName() + " (" + colour + ")", WINDOW_AUTOSIZE );
        imshow( this->getName() + " (" + colour + ")", im );

        

        waitKey(0);

        }
        catch (int e){
            cout << "Exception: Did not specify -string: colour- in void Image::displayImage(string colour). Options: RBG, GRAYSCALE. Exception No. " << e << endl;
        }
    
    }

    void Image::resize(int width, int height){ //Actually a crop function

        int i = 0;

        try{

        if (width > this-> width || height > this->height){
            throw 14;
        }

        this -> size = width*height;

        vector <unsigned char>* red_v = new vector <unsigned char> (this->size);
        vector <unsigned char>* green_v = new vector <unsigned char> (this->size);
        vector <unsigned char>* blue_v = new vector <unsigned char> (this->size);
        vector <unsigned char>* gray_v = new vector <unsigned char> (this->size);

        for(int x = 0; x < height; x++){

            for(int y = 0; y < width; y++){

                red_v -> at(i) = this->Red->at(x * this->width + y); //x*width + y gets the index of the intensity value we want
                green_v -> at(i) = this->Green->at(x * this->width + y);
                blue_v -> at(i) = this->Blue->at(x * this->width + y);
                gray_v -> at(i) = this->Blue->at(x * this->width + y);

                i++;
            }
        }

        delete this->Red; //delete old vectors
        delete this->Blue;
        delete this->Green;
        delete this->Gray;

        this->Red = red_v;
        this->Blue = blue_v;
        this->Gray = gray_v;
        this->Green = green_v;

        this -> width = width; //set new width and height
        this -> height = height;
        }
        catch(int e){
            cout << "Exception: Invalid -int: height- or -int: width- in void Image::resize(int width, int height). Exception No. " << e << endl;
        }
    }
}

using namespace cv;
using namespace std;
using namespace ImageProc;

int main( int argc, char** argv){

    if( argc != 2)                                      //check input for single string
    {
     cout << "Please enter image to display" << endl;
     return -1;                                         //exit with error
    }

    Image* img = new Image(argv[1]);
    Image* img_sc = img; //shallow copy
    Image* img_dc = new Image(*img, "deep"); //deep copy

    //CODE DEMO: PRESS ANY KEY TO PROGRESS IN THE DEMO. 6 IMAGES WILL BE DISPLAYED IN TOTAL.

    img -> displayImage("RGB"); //Display RGB Image

    img -> displayImage("GRAYSCALE"); //Display Grayscale Image

    img -> displayHist(256); //Display Image Histogram
    
    img -> resize(100, 100); //Resize Image (Actually just a crop, may have interpereted the task wrong)

    img -> displayImage("RGB"); //display the now resized (cropped) image

    img_sc -> displayImage("RGB"); //shallow copied image is also resized, as expected (just a pointer)

    img_dc -> displayImage("RGB"); //deep copied image is NOT resized! Can see the full image again.
    
    return 0;
}

//compile using g++ -std=c++11 -I/usr/local/include/ -L/usr/local/lib -g -o image_process image_process.cpp 
//-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d 
//-lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lopencv_imgcodecs