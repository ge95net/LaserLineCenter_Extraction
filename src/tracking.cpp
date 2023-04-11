// CharacterPoints.cpp: 定义应用程序的入口点。
//

#include "tracking.h"
#include <filesystem>
#include <algorithm>
#include <cmath>
#include <random>
#include <stdlib.h>
using namespace std;
using namespace cv;

Tracking::Tracking() {

}

void Tracking::ini_center(Mat image,int T) {
	//find ini_center
	int max = 0;
	int sum=0;
	float sumValue = 0;
	float x = 0;
	float y = 0;
	float xValue = 0;
	int num = 0;
	int start_line = 0;
	RGBimage = image;


	uchar* ptr = image.ptr<uchar>(0);

	
	for (int w = 0; w < image.cols; w++)
	{
		if (ptr[w] >= max) {
			max = ptr[w];
		}
	}
	//找有几个极值点，并求均值坐标
	for (int w = 0; w < image.cols; w++)
	{
		if (ptr[w] >= max * 0.6) {
			sumValue += ptr[w];
			xValue += w * ptr[w];
		}
	}
	x = xValue / sumValue;
	
	initial_center.push_back(Point2f(x, 0));
	track(image,T);

	
}

void Tracking::track(Mat image, int T) {
	//探索
	int max = 0;
	int sumValue = 0;
	int sumv = 0;
	int T_boundary ;
	int max_current = 0;
	int lb = 0;
	int rb = 0;
	float u;
	float diff;
	float next_x;
	int sign_l = 0;
	int sign_r = 0;
	float t_point;
	float gamma = 0.9;

	for (int i = 1; i < image.rows; i++)
	{
		uchar* ptr = image.ptr<uchar>(i);
		uchar* ptr_current = image.ptr<uchar>(i - 1);

		float x = initial_center[i - 1].x;
		int x_i = round(x);
		int sumValue = 0;
		int sumv = 0;
		int max = 0;
		int max_current = 0;

		

		int left1 = x_i - 1;
		int right1 = x_i + 1;
		int left2 = x_i - 2;
		int right2 = x_i + 2;





		left1_e = gaussion_dis(abs(x - left1));
		middle_e = gaussion_dis(abs(x - x_i));
		right1_e = gaussion_dis(abs(x - right1));
		right2_e = gaussion_dis(abs(x - right2));
		left2_e = gaussion_dis(abs(x - left2));




		if (i == 1) {
			accu1_left = left1_e;
			accu_middle = middle_e;
			accu1_right = right1_e;
			accu2_left = left2_e;
			accu2_right = right2_e;
		}
		else {
			accu1_left = gamma * past1_left + left1_e;
			accu_middle = gamma * past_middle + middle_e;
			accu1_right = gamma * past1_right + right1_e;
			accu2_left = gamma * past2_left + left2_e;
			accu2_right = gamma * past2_right + right2_e;

		}


		int sum1 = 0;
		points.swap(vector<float>());
		for (int t = -2; t < 3; t++) {
			points.push_back(ptr[x_i + t]);
			if (ptr[x_i + t] > max) {
				max = ptr[x_i + t];
				sum1 += ptr[x_i + t];
			}
		}
		float eva = sum1 / 5;

			//计算接下来几行的粗略中心的偏移情况,计算了边界值之后灰度重心算中心
			
		trend_point.swap(vector<float>());
		if (image.rows - i > 10) {
			for (int q = 0; q < 10; q++) {
				uchar* data = image.ptr<uchar>(i + q);
				lb = 0;
				rb = 0;
				int max1 = 0;
				
				T_boundary = eva / 2;
				if (T_boundary <= 10) {
					T_boundary = 5;
				}

				for (int m = 1;; m++)
				{
					if (lb == 0) {

						if (data[x_i - m] < T_boundary) {
							sign_l = x_i - m;
							if (data[x_i - m - 1] < T_boundary && data[x_i - m - 2] < T_boundary && data[x_i - m - 3] < T_boundary) {
								lb = sign_l;
							}
						}


					}



					if (rb == 0) {



						if (data[x_i + m] < T_boundary) {
							sign_r = x_i + m;
							if (data[x_i + m + 1] < T_boundary && data[x_i + m + 2] < T_boundary && data[x_i + m + 3] < T_boundary) {
								rb = sign_r;
							}
						}
					}
					if (lb != 0 && rb != 0) {
						break;
					}
				}
				sumv = 0;
				sumValue = 0;
				for (int m = lb; m < rb; m++) {

					sumValue += (ptr[m] * m);
					sumv += ptr[m];


				}
				
				u = (rb + lb) / 2;
				diff = abs(u - x);
			}


			if (diff > 2) {
				diff = 2;
			}

			trend_point.push_back(u);

		}
				
						
		if (trend_point.size() != 0) {
			int sum1 = 0;

			for (int l = 0; l < trend_point.size(); l++) {
				sum1 += trend_point[l];
			}
			t_point = sum1 / trend_point.size();

			if (t_point - x_i > 2) {
				t_point = x_i + 2;
			}
			if (x_i - t_point > 2) {
				t_point = x_i - 2;
			}
		}


		


				
				




			
		else {
			t_point = x;
			diff = 0;
		}


		float left2_imp = gaussion_dis(abs(x_i - 2 - t_point));
		float left1_imp = gaussion_dis(abs(x_i - 1 - t_point));
		float middle_imp = gaussion_dis(abs(x_i - t_point));
		float right1_imp = gaussion_dis(abs(x_i + 1 - t_point));
		float right2_imp = gaussion_dis(abs(x_i + 2 - t_point));




		




		co_left2 = accu2_left * (1 - diff / 2) + left2_imp * diff / 2;
		co_left1 = accu1_left * (1 - diff / 2) + left1_imp * diff / 2;
		co_middle = accu_middle * (1 - diff / 2) + middle_imp * diff / 2;
		co_right1 = accu1_right * (1 - diff / 2) + right1_imp * diff / 2;
		co_right2 = accu2_right * (1 - diff / 2) + accu2_right * diff / 2;
			//计算由历史轨迹影响的概率
		left1_prob = co_left1 / (co_left1 + co_middle + co_right1 + co_left2 + co_right2);
		middle_prob = co_middle / (co_left1 + co_middle + co_right1 + co_left2 + co_right2);
		right1_prob = co_right1 / (co_left1 + co_middle + co_right1 + co_left2 + co_right2);
		left2_prob = co_left2 / (co_left1 + co_middle + co_right1 + co_left2 + co_right2);
		right2_prob = co_right2 / (co_left1 + co_middle + co_right1 + co_left2 + co_right2);

			//用概率来过滤灰度值，再灰度重心
		left2_gray = left2_prob * ptr[x_i - 2];
		left1_gray = left1_prob * ptr[x_i - 1];
		middle_gray = middle_prob * ptr[x_i];
		right1_gray = right1_prob * ptr[x_i + 1];
		right2_gray = right2_prob * ptr[x_i + 2];


		next_x = (left2_gray * (x_i - 2) + left1_gray * (x_i - 1) + middle_gray * x_i + right1_gray * (x_i + 1) + right2_gray * (x_i + 2)) / (left2_gray + left1_gray + middle_gray + right1_gray + right2_gray);

		for (int t = -2; t < 3; t++) {
			if (ptr[x_i + t] > max) {
				max = ptr[x_i + t];
			}
			if (ptr_current[x_i + t] > max_current) {
				max_current = ptr_current[x_i + t];
			}
		}
		if (max < 10 && max_current < 10 && abs(next_x - x) < 1) {
			for (int y = -50; y < 51; y++) {
				sumValue += (ptr[x_i + y] * (x_i + y));
				sumv += ptr[x_i + y];

			}
			next_x = sumValue / sumv;
		}

		initial_center.push_back(Point2f(next_x, i));

		//根据下一行调整动作期望
		past2_left = accu2_left - left2_e;
		past1_left = accu1_left - left1_e;
		past_middle = accu_middle - middle_e;
		past1_right = accu1_right - right1_e;
		past2_right = accu2_right - right2_e;



		float new_left2 = gaussion_dis(abs(next_x - (x_i - 2)));
		float new_left1 = gaussion_dis(abs(next_x - (x_i - 1)));
		float new_middle = gaussion_dis(abs(next_x - x_i));
		float new_right1 = gaussion_dis(abs(next_x - (x_i + 1)));
		float new_right2 = gaussion_dis(abs(next_x - (x_i + 2)));
		if (abs(next_x - x) < 2) {
			past2_left = past2_left + new_left2;
			past1_left = past1_left + new_left1;
			past_middle = past_middle + new_middle;
			past1_right = past1_right + new_right1;
			past2_right = past2_right + new_right2;
		}
		else {
			past2_left = 0;
			past1_left = 0;
			past_middle = 0;
			past1_right = 0;
			past2_right = 0;
		}

		}


	}


	

	


float Tracking::gaussion_dis(float x) {
	return exp(-pow(x, 2));
}





void Tracking::draw_points() {
	Mat imgRGB_laser;
	Mat imgRGB_board;
	Vec4f line_para;
	cvtColor(RGBimage, imgRGB_laser, COLOR_GRAY2RGB);
	for (int i = 0; i < initial_center.size(); i++) {
		cv::circle(imgRGB_laser, initial_center[i], 0.4, Scalar(0, 0, 255), 1);
	}
	cout << "111" << endl;
}