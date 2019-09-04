#include <iostream>
#include <string>
#include <math.h> 
#include <set>
#include <fstream>

//opencv libs
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


//Gloabal Variable - hole missing (intead of -1)
int HoleVal = 255;
//point - x,x and pixels value

struct PointUniq
{
	int x; // col
	int y; // row
	int value;
	int uniqLoc;
	PointUniq() {}
	PointUniq(int a, int b, int c )
	{
		x = a;
		y = b;
		value = c;
		uniqLoc = x*100 + y;
	};

	bool operator < (const PointUniq& rhs) const
	{
		return this->uniqLoc < rhs.uniqLoc;
	}


};

//functions declaration
bool fexists(const std::string& filename);
void boundaryPoints(String file, set<PointUniq>& Points);
void boundaryPoints8Connectivity(String file, set<PointUniq>& Points);
float Weight(int x1, int y1, int x2, int y2, float z, float eps);
float TotalWieght(int x, int y, set<PointUniq>& Points, float z, float eps);
float TotalWieghtQ2(int x, int y, set<PointUniq>& Points, float z, float eps);
void floodFillUtil(Mat& im, int x, int y, set<PointUniq>& PointsBound, float z, float eps);
void floodFillUtil8Way(Mat& im, int x, int y, set<PointUniq>& PointsBound, float z, float eps);
void fillHole(String file, float  z, float eps, int connectivity);


//if file exists func
bool fexists(const std::string& filename) {
	std::ifstream ifile(filename.c_str());
	return (bool)ifile;
}

//find boundary points
void boundaryPoints(String file, set<PointUniq>& Points) {

	Mat grey = imread(file, IMREAD_GRAYSCALE);
	PointUniq p;
	int tmp1;
	int count = 0;
	for (int r = 0; r < grey.rows; r++) {
		for (int c = 0; c < grey.cols; c++) {
			int tmp = grey.at<uint8_t>(r, c);
			if (tmp == HoleVal) {
				tmp1 = grey.at<uint8_t>(r + 1, c);
				if (tmp1 != HoleVal) {
					p = { r + 1, c, grey.at<uint8_t>(r + 1, c) };
					Points.insert(p);
				}
				tmp1 = grey.at<uint8_t>(r - 1, c);
				if (tmp1 != HoleVal) {
					p = { r - 1, c, grey.at<uint8_t>(r - 1, c) };
					
					Points.insert(p);
				}
				tmp1 = grey.at<uint8_t>(r , c +1);
				if (tmp1 != HoleVal) {
					p = { r, c + 1, grey.at<uint8_t>(r, c + 1)};
					Points.insert(p);
				}
				tmp1 = grey.at<uint8_t>(r , c - 1 );
				if (tmp1 != HoleVal) {
					p = { r - 1, c, grey.at<uint8_t>(r - 1, c) };
					Points.insert(p);
				}
			}

		}
	}
	//end of loop

}
//find boundary points 8 connectivity
void boundaryPoints8Connectivity(String file, set<PointUniq>& Points) {
	Mat grey = imread(file, IMREAD_GRAYSCALE);
	for (int r = 0; r < grey.rows; r++) {
		for (int c = 0; c < grey.cols; c++) {
			int tmp = grey.at<uint8_t>(r, c);
			if (tmp == HoleVal) {
				if (grey.at<uint8_t>(r + 1, c) != HoleVal) {
					PointUniq p(r + 1, c, grey.at<uint8_t>(r + 1, c));
					Points.insert(p);
				}
				if (grey.at<uint8_t>(r - 1, c) != HoleVal) {
					PointUniq p(r - 1, c, grey.at<uint8_t>(r - 1, c));
					Points.insert(p);
				}
				if (grey.at<uint8_t>(r, c + 1) != HoleVal) {
					PointUniq p(r, c + 1, grey.at<uint8_t>(r, c + 1));
					Points.insert(p);
				}
				if (grey.at<uint8_t>(r, c - 1) != HoleVal) {
					PointUniq p(r - 1, c, grey.at<uint8_t>(r - 1, c));
					Points.insert(p);
				}
				if (grey.at<uint8_t>(r + 1, c - 1) != HoleVal) {
					PointUniq p(r + 1, c - 1, grey.at<uint8_t>(r + 1, c - 1));
					Points.insert(p);
				}
				if (grey.at<uint8_t>(r - 1, c + 1) != HoleVal) {
					PointUniq p(r - 1, c + 1, grey.at<uint8_t>(r - 1, c + 1));
					Points.insert(p);
				}
				if (grey.at<uint8_t>(r + 1, c + 1) != HoleVal) {
					PointUniq p(r + 1, c + 1, grey.at<uint8_t>(r + 1, c + 1));
					Points.insert(p);
				}
				if (grey.at<uint8_t>(r - 1, c - 1) != HoleVal) {
					PointUniq p(r - 1, c - 1, grey.at<uint8_t>(r - 1, c - 1));
					Points.insert(p);
				}
			}

		}
	}
	//end of loop

}
//wieght function
float Weight(int x1, int y1, int x2, int y2, float z, float eps) {
	float tmp = sqrt((pow((x1 - x2), 2)) + (pow((y1 - y2), 2)));
	return (1 / ((pow(tmp, z)) + eps));
}
//Value set - I(u) 
float TotalWieght(int x, int y, set<PointUniq>& Points, float z, float eps) {
	int NumOfElements = Points.size();
	int tmpx, tmpy;
	PointUniq tmpP;
	float sum = 0, tmpVal;
	set<PointUniq>::iterator it;
	for (it = Points.begin(); it != Points.end(); ++it)
	{
		tmpP = *it;
		tmpx = tmpP.x;
		tmpy = tmpP.y;
		tmpVal = tmpP.value;
		sum += Weight(x, y, tmpx, tmpy, z, eps) * tmpP.value;
	}

	sum = sum / NumOfElements;
	return sum;
}
//Value set - I(u)  Q2
float TotalWieghtQ2(int x, int y, set<PointUniq>& Points, float z, float eps) {
	int NumOfElements = Points.size();
	int tmpx, tmpy;
	PointUniq tmpP;
	float sum = 0, tmpVal;
	set<PointUniq>::iterator it;

	static PointUniq quaterPoints[4];
	static bool FirstCalc = false; 
	//ROI
	if (FirstCalc ==  false) {
		int minX = 10000000, minY = 1000000, maxX = 0, maxY = 0;
		for (it = Points.begin(); it != Points.end(); ++it)
		{
			tmpP = *it;
			if (tmpP.x < minX) { minX = tmpP.x; }
			if (tmpP.y < minY) { minY = tmpP.y; }
			if (tmpP.x > maxX) { maxX = tmpP.x; }
			if (tmpP.y > maxY) { maxY = tmpP.y; }
		}

		//calc midlle point in the ROI
		int middleX = (minX + maxX) / 2;
		int middleY = (minY + maxY) / 2;

		// set 4 middle coordinates
		quaterPoints[0].x = (middleX + minX) / 2;
		quaterPoints[0].y = (middleY + minY) / 2;

		quaterPoints[1].x = (middleX + maxX) / 2;
		quaterPoints[1].y = (middleY + minY) / 2;

		quaterPoints[2].x = (middleX + maxX) / 2;
		quaterPoints[2].y = (middleY + maxY) / 2;

		quaterPoints[3].x = (middleX + minX) / 2;
		quaterPoints[3].y = (middleY + maxY) / 2;

		//calculate middle point pixels value
		for (int i = 0; i < 4; i++) {
			for (it = Points.begin(); it != Points.end(); ++it)
			{
				tmpP = *it;
				tmpx = tmpP.x;
				tmpy = tmpP.y;
				tmpVal = tmpP.value;
				sum += Weight(quaterPoints[i].x, quaterPoints[i].y, tmpx, tmpy, z, eps) * tmpP.value;
			}
			quaterPoints[i].value = sum / NumOfElements;
		}
		FirstCalc = true;
	}
	//end of 4 quater calc 

	//calc missing mixel at O(1)
	for (int i = 0; i < 4; i++) {
		sum += Weight(quaterPoints[i].x, quaterPoints[i].y, x, y, z, eps) * quaterPoints[i].value;
	}
	sum = sum /4;
	return sum;
}

//set all mising points 4 way
void floodFillUtil(Mat& im, int x, int y, set<PointUniq>& PointsBound, float z, float eps)
{
	// Base cases
	int tmp = im.at<uint8_t>(x, y);
	if (tmp != HoleVal)
		return;
	float res = TotalWieghtQ2(x, y, PointsBound, z, eps);
	int   resInt = static_cast<int>(res);
	// Replace the color at (x, y) 
	im.at<uint8_t>(x, y) = resInt;
	// Recur for north, east, south and west 
	floodFillUtil(im, x + 1, y, PointsBound, z, eps);
	floodFillUtil(im, x - 1, y, PointsBound, z, eps);
	floodFillUtil(im, x, y + 1, PointsBound, z, eps);
	floodFillUtil(im, x, y - 1, PointsBound, z, eps);
}
//set all mising points 8 way
void floodFillUtil8Way(Mat& im, int x, int y, set<PointUniq>& PointsBound, float z, float eps)
{
	// Base cases
	int tmp = im.at<uint8_t>(x, y);
	if (tmp != HoleVal)
		return;
	float res = TotalWieght(x, y, PointsBound, z, eps);
	int   resInt = static_cast<int>(res);
	// Replace the color at (x, y) 
	im.at<uint8_t>(x, y) = resInt;
	// Recur for north, east, south and west 
	floodFillUtil(im, x + 1, y, PointsBound, z, eps);
	floodFillUtil(im, x - 1, y, PointsBound, z, eps);
	floodFillUtil(im, x, y + 1, PointsBound, z, eps);
	floodFillUtil(im, x, y - 1, PointsBound, z, eps);
	floodFillUtil(im, x + 1, y + 1, PointsBound, z, eps);
	floodFillUtil(im, x - 1, y + 1, PointsBound, z, eps);
	floodFillUtil(im, x - 1, y - 1, PointsBound, z, eps);
	floodFillUtil(im, x + 1, y - 1, PointsBound, z, eps);
}

//********************************************
//******************MAIN FUNC ****************
//********************************************
void fillHole(String file, float z = 2, float eps = 0.01, int connectivity = 8) {
	//check image exists load image
	if (!fexists(file)) {
		std::cout << file << " is not exists! , exit program";
		return;
	}
	// boundary points set
	set<PointUniq> PointsBound;
	Mat grey = imread(file, IMREAD_GRAYSCALE);
	//find firstpoint
	int startX = -1;
	int startY = -1;
	for (int r = 0; r < grey.rows; r++) {
		for (int c = 0; c < grey.cols; c++) {
			int tmp = grey.at<uint8_t>(r, c);
			if (tmp == HoleVal)
			{
				startX = r;
				startY = c;
				break;
			}
		}
		if (startX != -1) { break; }
	}
	//fill mising pixels 
	if (connectivity == 4) {
		boundaryPoints(file, PointsBound);
		floodFillUtil(grey, startX, startY, PointsBound, z, eps);
	}
	else if (connectivity == 8) {
		boundaryPoints8Connectivity(file, PointsBound);
		floodFillUtil8Way(grey, startX, startY, PointsBound, z, eps);
	}
	else {
		std::cout << "connectivity most be int 8 or 4 ur input : " << connectivity << " is invalid! , exit program";
		return;
	}
	//output file with full hole
	cv::imwrite("GreyFilled.jpg", grey);
}

int main(void) {

	//example to call a file
	fillHole("resGrey2.jpg"  , 2 , 0.01 ,4);
	return(0);
}




