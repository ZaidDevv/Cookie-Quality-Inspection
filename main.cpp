#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>

/* Project is done by Zaid Mohammed Hameed & Adithya Narayan Selvaraj  & Nehal Hussain */

using namespace cv;
using namespace std;

vector<Vec3f> circles;
//void avgColor(Vec3i c, Mat src, float&, float&, float&);

int main(int argc, char** argv)
{
	float valueGoodCaseAVg = 0, satGoodCaseAVg = 0, hueGoodCaseAVg = 0;
	Scalar myMean;
	const char* filename = argc >= 2 ? argv[1] : "C:\\Users\\Z\\Downloads\\imgproc\\pic.png";
	// Loads an image
	Mat src = imread(samples::findFile(filename), IMREAD_COLOR);
	// Check if image is loaded fine
	if (src.empty()) {
		printf(" Error opening image\n");
		printf(" Program Arguments: [image_name -- default %s] \n", filename);
		return EXIT_FAILURE;
	}

	Mat gray;
	Mat grayRef;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	medianBlur(gray, gray, 5);
	vector<Vec3f> circles;

	HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
		gray.rows / 16,  // change this value to detect circles with different distances to each other
		50, 23, 35, 70 // change the last two parameters
	);
	Mat myRef = imread("C:\\Users\\Z\\Downloads\\imgproc\\myref.png");

	vector<Vec3f> circlesRef;
	cvtColor(myRef, grayRef, COLOR_BGR2GRAY);

	HoughCircles(grayRef, circlesRef, HOUGH_GRADIENT, 1,
		gray.rows / 16,  // change this value to detect circles with different distances to each other
		60, 5, 50, 230 // change the last two parameters
	);

	Vec3i c1 = circlesRef[0];
	Point centerref = Point(c1[0], c1[1]);
	//avgColor(c1, myRef, valueGoodCaseAVg, hueGoodCaseAVg, satGoodCaseAVg);
	circle(myRef, centerref, 1, Scalar(0, 100, 100), 3, LINE_AA);
	circle(myRef, centerref, c1[2], Scalar(255, 0, 0), 3, LINE_AA);

	Mat roiRef = myRef(Range(c1[1] - c1[2], c1[1] + c1[2] + 1), Range(c1[0] - c1[2], c1[0] + c1[2] + 1));

	Mat1b masky(roiRef.rows, roiRef.cols);

	Scalar myMeanRef = cv::mean(roiRef);

	for (int i = 0; i < circles.size(); i++)
	{
		float valAvg = 0, satAvg = 0, hueAvg = 0;

		Vec3i c = circles[i];
		Vec3i fixedCircle = circles[0];
		Point center = Point(c[0], c[1]);
		// circle center
		circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
		// circle outline
		int radius = c[2];

		//avgColor(c, src, valAvg, hueAvg, satAvg);

		Mat roi = src(Range(abs(c[1] - c[2]), abs(c[1] + c[2] + 1)), Range(abs(c[0] - c[2]),abs(c[0] + c[2] + 1)));
		Mat1b mask(roi.rows, roi.cols);

		myMean = mean(roi);


		float avgClr = ((myMean[0] + myMean[1] + myMean[2])/3);


		cout << "avgColor " << avgClr << endl;

		cout << "Blue AVERAGE DIFFERENCE: " << myMean[0]<< endl;

		cout << "GREEN AVERAGE DIFFERENCE: " << myMean[1]<< endl;

		cout << "RED AVERAGE DIFFERENCE: " << myMean[2]<< endl;


		//cout << "RBlue AVERAGE DIFFERENCE: " << abs(myMeanRef[0]) << endl;

		//cout << "RGREEN AVERAGE DIFFERENCE: " << abs(myMeanRef[1]) << endl;

		//cout << "RRED AVERAGE DIFFERENCE: " << abs(myMeanRef[2]) << endl;

		if (avgClr < 103) {
			circle(src, center, radius, Scalar(255, 0, 0), 3, LINE_AA);
		}

		else {
			cout << "Defect" << endl;
		}
	}

	Mat outImg;

	resize(src, outImg, cv::Size(), 0.50, 0.50);

	imshow("Detected circles", outImg);
	imshow("Detected Reference", myRef);
	waitKey();

	return EXIT_SUCCESS;
}

/*void avgColor(Vec3i c, Mat src, float& value, float& sat, float& hue) {
	Mat hsv;
	cvtColor(src, hsv, COLOR_BGR2HSV);

	int radius = c[2];

	for (int j = c[0]; j < c[0] + radius; j++) {
		value += hsv.at<Vec3b>(j, c[1])[2];
		sat += hsv.at<Vec3b>(j, c[1])[0];
		hue += hsv.at<Vec3b>(j, c[1])[1];
	}

	for (int j = c[1]; j < c[1] + radius; j++) {
		value += hsv.at<Vec3b>(c[0], j)[2];
		sat += hsv.at<Vec3b>(c[0], j)[0];
		hue += hsv.at<Vec3b>(c[0], j)[1];
	}

	for (int j = c[1] + radius; j > abs(c[1] - radius); j--) {
		value += hsv.at<Vec3b>(c[0], j)[2];
		sat += hsv.at<Vec3b>(c[0], j)[0];
		hue += hsv.at<Vec3b>(c[0], j)[1];
	}

	for (int j = c[0] + radius; j > abs(c[0] - radius); j--) {
		value += hsv.at<Vec3b>(j, c[1])[2];
		sat += hsv.at<Vec3b>(j, c[1])[0];
		hue += hsv.at<Vec3b>(j, c[1])[1];
	}

	value /= radius * 4;
	sat /= radius * 4;
	hue /= radius * 4;
}*/
