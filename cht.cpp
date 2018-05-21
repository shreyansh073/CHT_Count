/*
	Count the number of circles/cylinders using circular Hough Transform and generate a csv report of the analysis.
*/

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;
using namespace cv;


namespace
{
    // windows and trackbars name
    const std::string windowName = "Hough Circle Detection Demo";
    const std::string cannyThresholdTrackbarName = "Canny threshold";
    const std::string accumulatorThresholdTrackbarName = "Accumulator Threshold";
    const std::string usage = "Usage : tutorial_HoughCircle_Demo <path_to_input_image>\n";

    // initial and max values of the parameters of interests.
    const int cannyThresholdInitialValue = 50;
    const int accumulatorThresholdInitialValue = 20;
    const int maxAccumulatorThreshold = 200;
    const int maxCannyThreshold = 255;

    int HoughDetection(const Mat& src_gray, const Mat& src_display, int cannyThreshold, int accumulatorThreshold,int min,int max,vector<Vec3f> &circles)
    {
        // will hold the results of the detection
        
        // runs the actual detection
        HoughCircles( src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows/10, cannyThreshold, accumulatorThreshold,min, max );

        // clone the colour, input image for displaying purposes
        Mat display = src_display.clone();
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            //circle( display, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            std::string s = std::to_string(i);
            putText(display, s, center, FONT_HERSHEY_SIMPLEX,0.5, Scalar(100,100,0), 1, LINE_8, false );
            circle( display, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }

        // shows the results
        
        imshow( windowName, display);
        return circles.size();
    }
}



int main(int argc, char** argv)
{
    //create file output stream
    ofstream outputFile;
        
    Mat src, src_gray;

    // Read the image
    String imageName("../data/stuff.jpg"); // by default
    if (argc > 1)
    {
       imageName = argv[1];
    }
    src = imread( imageName, IMREAD_COLOR );

    if( src.empty() )
    {
        std::cerr<<"Invalid input image\n";
        std::cout<<usage;
        return -1;
    }
    
    cout<<"Enter minimum and maximum radius"<<std::endl;
    int min=0,max=0;
    cin>>min>>max;
    
    outputFile.open("report.csv");
    
    // write the file headers
    outputFile << "dp" << "," << "min_dist" << "," <<"param_1" <<"," <<"param_2" <<"," <<"count" << "," <<"min_radius"<<","<<"max_radius"<<std::endl;

    // Convert it to gray
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    // Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    //declare and initialize both parameters that are subjects to change
    int cannyThreshold = cannyThresholdInitialValue;
    int accumulatorThreshold = accumulatorThresholdInitialValue;

    // create the main window, and attach the trackbars
    namedWindow( windowName, WINDOW_AUTOSIZE );
    createTrackbar(cannyThresholdTrackbarName, windowName, &cannyThreshold,maxCannyThreshold);
    createTrackbar(accumulatorThresholdTrackbarName, windowName, &accumulatorThreshold, maxAccumulatorThreshold);

    // infinite loop to display
    // and refresh the content of the output image
    // until the user presses q or Q
    std::vector<Vec3f> circles;
    char key = 0;int count=0;
    while(key != 'q' && key != 'Q')
    {
        // those parameters cannot be =0
        // so we must check here
        cannyThreshold = std::max(cannyThreshold, 1);
        accumulatorThreshold = std::max(accumulatorThreshold, 1);

        //runs the detection, and update the display
        count=HoughDetection(src_gray, src, cannyThreshold, accumulatorThreshold,min,max,circles);

        // get user key
        key = (char)waitKey(10);
    }
    
    outputFile << "1" << "," << src_gray.rows/10 << "," <<cannyThreshold<<"," <<accumulatorThreshold <<"," <<count<< "," <<min<<"," <<max<< std::endl<<std::endl;
    outputFile<<"circle_no"<<","<<"x_coordinate"<<","<<"y-coordinate"<<","<<"predicted_radius"<<std::endl;
    for( size_t i = 0; i < circles.size(); i++ )
    {
        //Individual circle coordinates and radius  
        outputFile<<i<<","<<circles[i][0]<<","<<circles[i][1]<<","<<circles[i][2]<<std::endl;
    }
    
    //Output date and time of report
    time_t now = time(0);
    tm *ltm = localtime(&now);
    outputFile<<std::endl<<"Year"<<","<<"Month"<<","<<"Day"<<","<<"Hour"<<","<<"Min"<<","<<"Sec"<<std::endl;
    outputFile<<1970 + ltm->tm_year<<","<< 1 + ltm->tm_mon <<","<< ltm->tm_mday <<","<<1 + ltm->tm_hour<<","<<1 + ltm->tm_min<<","<<1 + ltm->tm_sec<<std::endl;  
    
    outputFile.close();
    return 0;
}
