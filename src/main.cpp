#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "tracking.h"
using namespace std;
using namespace cv;

int main() {
	Tracking Tracking;
	//Mat roughImage = imread("C:\\Users\\qcyu\\Desktop\\YQC_IMGS\\0323\\calib_test\\1.1mm_h.bmp", 0);
	//calib_test里150，200，1.3,1.4：200，150,1.2h:150,150
	
	//Tracking.ini_center(roughImage,200);
	
	
	//文件夹defect3
	//Mat roughImage = imread("C:\\Users\\qcyu\\Desktop\\YQC_IMGS\\0323\\battery_edge\\defect3\\edge10.bmp", 0);
	
	//light10断的地方不能追踪,默认值：150，5
	//cout << "111" << endl;
	//Tracking.ini_center(roughImage);



	//文件夹defect11
	Mat roughImage = imread("C:\\Users\\qcyu\\Desktop\\YQC_IMGS\\0323\\battery_edge\\defect11\\edge7.bmp", 0);
	//edge0:150,60,edge1,2,4:150,30，edge7，默认值：150，30   //edge3,7有断点
	Tracking.ini_center(roughImage,5);
	//cout << "111" << endl;

	//文件夹defect14
	//Mat roughImage = imread("C:\\Users\\qcyu\\Desktop\\YQC_IMGS\\0323\\battery_edge\\defect14\\edgel3.bmp", 0);
	//edge5:30,30,edge6:30,210，默认值：30，30
	//Tracking.ini_center(roughImage);


	//Mat roughImage = imread("C:\\Users\\qcyu\\Desktop\\YQC_IMGS\\0323\\battery_edge\\defect3\\1.jpg", 0);
	// 200
	//transpose(roughImage, roughImage);
	//Tracking.ini_center(roughImage);
	
	
	//第三个参数表示边界的阈值，对亮一点的图片设置大一点，对暗一点的图片设置小一点，图像越暗，边界越模糊，则要设置小（5-30），图像越亮，边界越明显，则可以设置大（200-230）
	//defect3 文件夹里设置150， defect11文件夹2、4里设置30，3、6、8、10设置5，light取150
	//在暗的区域，取小一点，可以识别暗的连接线，如edge3，在亮图中如果取小了，则容易收到干扰
	


	

	Tracking.draw_points();
}