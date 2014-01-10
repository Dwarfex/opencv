#include "backgroundsubtraction.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include<algorithm>

using namespace cv;
using namespace std;

//Variablen für BckgoundSubstration
const int nmixtures =8;
const bool bShadowDetection = true;
const int history = 3;


void BackgroundSubtraction::backgroundSubtraction(){
	Mat frame; //Kamera Input, unangetstest
	Mat front; //Fordegrund (Hand)
	Mat back; //Hintegrunde (der Rest)


    VideoCapture videoCapture;
	videoCapture.open(0);

	
	BackgroundSubtractorMOG2 bgSubtraction (history,nmixtures,bShadowDetection);  
	
	int backgroundFrame = 200; //Frame anzahl bis der Hintergrund festgelegt wird 


	while(true){

		vector<vector<Point> > contours;

		bool succes = videoCapture.read(frame);
		if (succes == false){break;}
		flip(frame,frame,1); // drehe das Video 

		if(backgroundFrame>0){ //wende BGSubstructionMOG2 an bis "backgroundFrame" auf Bull ist
			bgSubtraction.operator()(frame,front); 
			backgroundFrame--;
		} else {
			bgSubtraction.operator()(frame,front,0); //sobald "backgroundFrame" null ist, ist der Hintergrund festgelegt
		}

		bgSubtraction.getBackgroundImage(back);

		erode(front,front,Mat()); //Wende Eroding an
		dilate(front,front,Mat()); //Wende Delating an

		//In OpenCV intigrietes Kantenerkennungs system das den CannyFillter benutzt
		findContours(front,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE); 
		
		

		for(int i=0; i < contours.size();i++){
			//Kleine unwichtige gebiete ignorieren
			if(contourArea(contours[i])>=5000) {
				vector<vector<Point> > tcontours;
				tcontours.push_back(contours[i]);
				drawContours(frame,tcontours,-1,Scalar(0,0,255),2);

				/*
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
								}*/
			}

		}
		
		 if(backgroundFrame>0)
                        putText(frame, "Hintergrund aufnehmen", cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);


		imshow("Hand",front);
		imshow("Hand",back);
		imshow("PureVideo",frame);
		waitKey(30);
	}


}
