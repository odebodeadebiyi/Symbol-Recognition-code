#include "opencv_aee.hpp"
#include "main.hpp"     // You can use this file for declaring defined values and functions
#include "pi2c.h"

int area;

Mat transformed;
Mat HSVtransformed;
Mat image_HSV;
Mat image_PINK;
Mat circle;
Mat star;
Mat triangle;
Mat umbrella;
Mat HSVcircle;
Mat HSVtriangle;
Mat HSVstar;
Mat HSVumbrella;
Mat BWcircle;
Mat BWtriangle;
Mat BWstar;
Mat BWumbrella;
Mat WBcircle;
Mat WBtriangle;
Mat WBstar;
Mat WBumbrella;
Mat comparison;

void stored();


void setup()
{
    setupCamera(320, 240);  // Enable the camera for OpenCV
    stored();
}

int main( int argc, char** argv )
{
    setup();    // Call a setup function to prepare IO and devices
    cv::namedWindow("Photo");   // Create a GUI window called photo

    while(1)    // Main loop to perform image processing
    {
        Mat frame;
        Mat image; // Create a matrix to store the image
        while(frame.empty())
            frame = captureFrame(); // Capture a frame from the camera and store in a new matrix variable

        cv::imshow("Photo", frame); //Display the image in the window

        cvtColor(frame, image_HSV, COLOR_BGR2HSV); // Convert the image to HSV

        inRange(image_HSV, Scalar(35, 68, 57), Scalar(213, 255, 128), image_PINK); //pink
        cv::namedWindow("Symbol");
        cv::imshow("Symbol", image_PINK); //Display the image in the window

        std::vector< std::vector<cv::Point> > contours; // Variable for list of contours
        std::vector<Vec4i> hierarchy; // Variable for image topology data
        cv::findContours(image_PINK, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0)); // Calculate the contours and store them

        for(int i = 0; i < contours.size(); i++)
        {
        drawContours(frame, contours, i, Scalar(0,255,0), 2, LINE_8, noArray(), 0, Point() ); // Draw each in red
        area = cv::contourArea(contours[i]);
        }

        cv::namedWindow("Contours");
        cv::imshow("Contours", frame); //Display the image in the window

        std::vector< std::vector<cv::Point> > approxedcontours(contours.size());
        for(int i = 0; i < contours.size(); i++)
        {
        cv::approxPolyDP(contours[i],approxedcontours[i], 10, true);
        }

        std::vector<Point>boundingContour = approxedcontours[0];

        transformed = transformPerspective(boundingContour, frame, 320, 240);
        cvtColor(transformed, HSVtransformed, COLOR_BGR2HSV); // Convert the image to HSV
        inRange(HSVtransformed, Scalar(35, 68, 57), Scalar(213, 255, 128), comparison);
	cv::namedWindow("Comparison");
        cv::imshow("Comparison",comparison); //Display the image in the window


        float circlematch = compareImages(comparison, WBcircle);
        std::cout << "Circle matching percentage: " << circlematch<< std::endl;
        if (circlematch >= 70)
        {
        printf("Circle is a match\n");
        }
        else
        {
        float starmatch = compareImages(comparison, WBstar);
        std::cout << "Star matching percentage: " << starmatch<< std::endl;
        if (starmatch >= 70)
            {
            printf("Star is a match\n");
            }
            else{
            float trianglematch = compareImages(comparison, WBtriangle);
           std::cout << "Triangle matching percentage: " << trianglematch<< std::endl;
            if (trianglematch >= 70)
                {
                printf("Triangle is a match\n");
                }
                else{
                float umbrellamatch = compareImages(comparison, WBumbrella);
                std::cout << "Umbrella matching percentage: " << umbrellamatch<< std::endl;
                if (umbrellamatch >= 70)
                    {
                    printf("Umbrella is a match\n");
                    }
                    else{
                    printf("No matching symbol\n");
                    }
                }
            }
        }

        int key = cv::waitKey(1);   // Wait 1ms for a keypress (required to update windows)


        key = (key==255) ? -1 : key;    // Check if the ESC key has been pressed
        if (key == 27)
            break;
	}

	closeCV();  // Disable the camera and close any windows

	return 0;
}


void stored()
{
        Mat circle = readImage("/home/pi/Desktop/OpenCV-Template/Circle (Red Line).png"); // Read the image
        cvtColor(circle, HSVcircle, COLOR_BGR2HSV); // Convert the image to HSV
        inRange(HSVcircle, Scalar(0, 0, 255), Scalar(139, 255, 255), BWcircle);
        bitwise_not(BWcircle, WBcircle);
        cv::namedWindow("Circle");
        cv::imshow("Circle",WBcircle);
        Mat triangle = readImage("/home/pi/Desktop/OpenCV-Template/Triangle (Blue Line).png"); // Read the image
        cvtColor(triangle, HSVtriangle, COLOR_BGR2HSV); // Convert the image to HSV
        inRange(HSVtriangle, Scalar(0, 0, 255), Scalar(139, 255, 255), BWtriangle);
        bitwise_not(BWtriangle, WBtriangle);
        Mat star = readImage("/home/pi/Desktop/OpenCV-Template/Star (Green Line).png"); // Read the image
        cvtColor(star, HSVstar, COLOR_BGR2HSV); // Convert the image to HSV
        inRange(HSVstar, Scalar(0, 0, 255), Scalar(139, 255, 255), BWstar);
        bitwise_not(BWstar, WBstar);
        Mat umbrella = readImage("/home/pi/Desktop/OpenCV-Template/Umbrella (Yellow Line).png"); // Read the image
        cvtColor(umbrella, HSVumbrella, COLOR_BGR2HSV); // Convert the image to HSV
        inRange(HSVumbrella, Scalar(0, 0, 255), Scalar(139, 255, 255), BWumbrella);
        bitwise_not(BWumbrella, WBumbrella);
}
