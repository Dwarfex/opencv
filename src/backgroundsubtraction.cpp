#include "backgroundsubtraction.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include<algorithm>

using namespace cv;
using namespace std;

 
const int nmixtures =5;
const bool bShadowDetection = false;
const int history = 3;


void BackgroundSubtraction::backgroundSubtraction(){
	Mat frame; //Kamera Input, unangetstest
	Mat front; //Fordegrund (Hand)
	Mat back; //Hintegrunde
	Mat cannyfilter;

	//CammyFilter
	int edgeThresh = 3;
  int lowThreshold = 45;
  //int const max_lowThreshold = 100;
  int ratio = 3;
  int kernel_size = 3;
  //

    VideoCapture videoCapture;
	videoCapture.open(0);

	
	BackgroundSubtractorMOG2 bgSubtraction (history,nmixtures,bShadowDetection);
	//BackgroundSubtractorMOG2 bgSubtraction (nmixtures,bShadowDetection);

	int backgroundFrame = 500;


	while(true){

		vector<vector<Point> > contours;

		bool succes = videoCapture.read(frame);
		if (succes == false){break;}
		 flip(frame,frame,1); // drehe das Video

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
		
		blur( front, cannyfilter, Size(3,3) );
		Canny( cannyfilter, cannyfilter, lowThreshold, lowThreshold*ratio, kernel_size );
		
		//findContours(cannyfilter,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

		for(int i=0; i < contours.size();i++){
			//Kleine unwichtige gebiete ignorieren
			if(contourArea(contours[i])>=5000) {
				vector<vector<Point> > tcontours;
				tcontours.push_back(contours[i]);
				drawContours(frame,tcontours,-1,Scalar(0,0,255),2);

				//Detect Hull in current contour
                                vector<vector<Point> > hulls(1);
                                vector<vector<int> > hullsI(1);
                                convexHull(Mat(tcontours[0]),hulls[0],false);
                                convexHull(Mat(tcontours[0]),hullsI[0],false);
                                drawContours(frame,hulls,-1,cv::Scalar(0,255,0),2);

                                //Find minimum area rectangle to enclose hand
                                RotatedRect rect=minAreaRect(Mat(tcontours[0]));

                                //Find Convex Defects
                                vector<Vec4i> defects;
                                if(hullsI[0].size()>0)
                                {
                                        Point2f rect_points[4]; rect.points( rect_points );
                                        for( int j = 0; j < 4; j++ )
                                                line( frame, rect_points[j], rect_points[(j+1)%4], Scalar(255,0,0), 1, 8 );
                                        Point rough_palm_center;
                                        convexityDefects(tcontours[0], hullsI[0], defects);
                                        if(defects.size()>=3)
                                        {
                                                vector<Point> palm_points;
                                                for(int j=0;j<defects.size();j++)
                                                {
                                                        int startidx=defects[j][0]; Point ptStart( tcontours[0][startidx] );
                                                        int endidx=defects[j][1]; Point ptEnd( tcontours[0][endidx] );
                                                        int faridx=defects[j][2]; Point ptFar( tcontours[0][faridx] );
                                                        //Sum up all the hull and defect points to compute average
                                                        rough_palm_center+=ptFar+ptStart+ptEnd;
                                                        palm_points.push_back(ptFar);
                                                        palm_points.push_back(ptStart);
                                                        palm_points.push_back(ptEnd);
                                                }
										}
								}
			}

		}
		
		 if(backgroundFrame>0)
                        putText(frame, "Recording Background", cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);


		imshow("Cammy-Filter",cannyfilter);
		imshow("Hand",front);
		imshow("PureVideo",frame);
		waitKey(30);
	}


}
