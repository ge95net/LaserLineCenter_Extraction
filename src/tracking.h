// tracking.h : Include file for standard system include files,
// or project specific include files.

#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;
// TODO: Reference additional headers your program requires here.

class Tracking {
public:
	Tracking();
	void ini_center(Mat image,int T);
	void track(Mat image ,int T);
	void draw_points();
	float gaussion_dis(float x);
	void fitline();


private:
	vector<Point2f> laserCenter_ini;
	vector<Point2f> initial_center;
	vector<float> trend_point;
	vector<Point2f> beginn_end_points;
	vector<int>start_point;
	vector<float> points;

	Mat ROI_coor;
	Mat ROI_value;
	int ini_x;
	Mat RGBimage;

	vector<float> expection;
	vector<float> accu_exp;
	vector<float> prob;
	float left1_e;
	float right1_e;

	float left2_e;
	float middle_e;
	float right2_e;

	float accu1_left;
	float accu1_right;

	float accu2_left;
	float accu_middle;
	float accu2_right;

	float past2_left;
	float past1_left;
	float past_middle;
	float past1_right;
	float past2_right;


	float left2_prob;
	float middle_prob;
	float right2_prob;

	float left1_prob;
	float right1_prob;

	float dis;

	float left2_gray;
	float left1_gray;
	float middle_gray;
	float right1_gray;
	float right2_gray;


	float co_left2;
	float co_left1;
	float co_middle;
	float co_right1;
	float co_right2;

	float gamma;
};