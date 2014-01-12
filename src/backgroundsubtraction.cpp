#include "backgroundsubtraction.h"
#include "HandSchwerpunkt.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include<algorithm>

using namespace cv;
using namespace std;

//Variablen für BckgoundSubstration
const int nmixtures =5;
const bool bShadowDetection = false;
const int history = 15;


void BackgroundSubtraction::backgroundSubtraction(){
	Mat frame; //Kamera Input, unangetstest
	Mat front; //Fordegrund (Hand)
	Mat back; //Hintegrunde (der Rest)
	int middleX = 0;
	int middleY = 0;
	int crosssize = 10;

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
				drawContours(frame,tcontours,-1,Scalar(0,0,255),2); //zeichen Kontur

				
				//Erkenne huelle
                vector<vector<Point> > hulls(1);
                vector<vector<int> > hullsI(1);
                convexHull(Mat(tcontours[0]),hulls[0],false);
                convexHull(Mat(tcontours[0]),hullsI[0],false);
                drawContours(frame,hulls,-1,cv::Scalar(0,255,0),2);



			}

		}

		//Handschwerpunkt anzeichnen
		handSchwerpunkt(frame);


		 if(backgroundFrame>0)
                        putText(frame, "Hintergrund aufnehmen", cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);


		imshow("Hand",front);
		imshow("Hand",back);
		imshow("PureVideo",frame);
		waitKey(30);
	}


}
