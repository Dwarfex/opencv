#include "HandSchwerpunkt.h"
using namespace cv;

void handSchwerpunkt(cv::Mat& frame){
	int middleX = 0;
	int middleY = 0;
	int crosssize = 10;
	int redPixelCount = 0;
	int redPixelx = 0;
	int redPixely = 0;
					
					
					
					for(int x = 0; x < frame.cols; x++){
						
						for(int y = 0; y < frame.rows; y++){
							
							
							Vec3b colour = frame.at<Vec3b>(Point(x, y));
							if(colour.val[0]==0 && colour.val[1]==255 && colour.val[2]==0){
								redPixelCount++;
								redPixely = redPixely + y;
								redPixelx = redPixelx + x;
							}
						}
					}
					if(redPixelCount > 0){
					middleX = redPixelx / redPixelCount;
					middleY = redPixely/ redPixelCount;
					}
					 //ende schwerpunkt

					 // Kreuz zeichnen
						Point center(middleX, middleY);
						line(frame,Point(center.x - crosssize, center.y),Point(center.x + crosssize, center.y),Scalar(0,255,0),2,8);
						line(frame,Point(center.x, center.y - crosssize),Point(center.x, center.y + crosssize),Scalar(0,255,0),2,8);
					//ende Kreuz zeichnen

}
