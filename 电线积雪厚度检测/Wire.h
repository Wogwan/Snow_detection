#pragma once

#include<opencv2\opencv.hpp>
#include<opencv2\core.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

//【1】多边缘提取，Src为包含多个边缘的图像；Edge为提取出的边缘点容器；SingleMminSum为默认为边缘的最小边缘点数；Left_Up为传入的Src图的相对坐标；Width为可以提取边缘的最窄毛刺宽度。
void Edge_Multiple(Mat &Src, vector<vector<Point2i>> &Edge, int SingleMinSum = 10, int SingleMaxSum=1000000, Point2i Left_up = Point2i(0, 0), int Width = 3);

//【2】识别无雪层电线,Edge为检测出的电线边缘点，diameter为电线的粗细，interval为电线间的间距
void Wire(Mat &Src, vector<vector<Point2i>> &Edge,float& diameter, vector<float> &interval);

//【3】识别有雪层电线,Edge为检测出的电线边缘点，Snow为积雪的边缘点,snowwidth为积雪厚度，diameter为电线的粗细，interval为电线间的间距
void  SnowWire(Mat &Src, vector<vector<Point2i>> &Edge, vector<vector<Point2i>> &Snow, vector<float> &snowwidth, float& diameter, vector<float> &interval);