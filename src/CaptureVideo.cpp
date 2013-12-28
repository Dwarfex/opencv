#include <opencv2\opencv.hpp>
#include "CaptureVideo.h"

using namespace std;
using namespace cv;


void CaptureVideo::captureV(){
	videoCapture.open(0);
	namedWindow("PureVideo", CV_WINDOW_AUTOSIZE);
	while(true){
		Mat Purevideo;
		
		bool succes = videoCapture.read(Purevideo);
		if (succes == false){break;}


		imshow("PureVideo",Purevideo);
		waitKey(30);
	}
}
