#include "backgroundsubtraction.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include<algorithm>

using namespace cv;
using namespace std;

 
const int nmixtures =3;
const bool bShadowDetection = true;
const int history = 5;

void BackgroundSubtraction::backgroundSubtraction(){
	Mat frame;
	Mat front;
	Mat back;

    VideoCapture videoCapture;
	videoCapture.open(0);

	//BackgroundSubtractorMOG2 bgSubtraction;
	BackgroundSubtractorMOG2 bgSubtraction (history,nmixtures,bShadowDetection);
	//bgSubtraction.set("nmixtures",3);
	//bgSubtraction.set("detectShafows",false);

	int backgroundFrame = 500;


	while(true){

		vector<vector<Point> > contours;

		bool succes = videoCapture.read(frame);
		if (succes == false){break;}

		if(backgroundFrame>0){
			bgSubtraction.operator()(frame,front);
			backgroundFrame--;
		} else {
			bgSubtraction.operator()(frame,front,0);
		}


		bgSubtraction.getBackgroundImage(back);
		erode(front,front,Mat());
		dilate(front,front,Mat());

		findContours(front,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		 if(backgroundFrame>0)
                        putText(frame, "Recording Background", cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
		imshow("Hand",front);
		imshow("PureVideo",frame);
		waitKey(30);
	}


}
