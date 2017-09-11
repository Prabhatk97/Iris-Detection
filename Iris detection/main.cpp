#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include<vector>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("blueeye.jpg", CV_LOAD_IMAGE_GRAYSCALE), tmp;

	imshow("Source", src);

	double minVal = 0;
	minMaxLoc(src, &minVal, NULL, NULL, NULL);

	threshold(src, tmp, minVal + 18, 255, THRESH_BINARY_INV);

	//(Optional) remove noise (small areas of white pixels)
	/*
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	erode(tmp, tmp, element);
	dilate(tmp, tmp, element);
	*/
	vector<Vec4i> hierarchy;
	vector<vector<Point2i> > contours;
	findContours(tmp, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	//find contour with max area
	int maxArea = 0;
	Rect maxContourRect;
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		Rect rect = boundingRect(contours[i]);
		double squareKoef = ((double)rect.width) / rect.height;

		//check if contour is like square (shape)
#define SQUARE_KOEF 1.5
		if (area > maxArea && squareKoef < SQUARE_KOEF && squareKoef > 1.0 / SQUARE_KOEF)
		{
			maxArea = area;
			maxContourRect = rect;
		}
	}

	if (maxArea == 0)
	{
		std::cout << "Iris not found!" << std::endl;
	}
	else
	{
		Rect drawRect = Rect(maxContourRect.x - maxContourRect.width, maxContourRect.y - maxContourRect.height, maxContourRect.width * 3, maxContourRect.height * 3);

		rectangle(src, drawRect, Scalar(0), 1);

		imshow("Dest", src);

	}


	Mat ROI(src, Rect(maxContourRect.x - maxContourRect.width, maxContourRect.y - maxContourRect.height, maxContourRect.width * 3, maxContourRect.height * 3));

	Mat croppedImage;

	// Copy the data into new matrix
	ROI.copyTo(croppedImage);

	imwrite("newImage.jpg", croppedImage);
    imshow("crop", croppedImage);
	waitKey(0);

	

}
