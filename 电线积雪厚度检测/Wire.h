#pragma once

#include<opencv2\opencv.hpp>
#include<opencv2\core.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

//��1�����Ե��ȡ��SrcΪ���������Ե��ͼ��EdgeΪ��ȡ���ı�Ե��������SingleMminSumΪĬ��Ϊ��Ե����С��Ե������Left_UpΪ�����Srcͼ��������ꣻWidthΪ������ȡ��Ե����խë�̿�ȡ�
void Edge_Multiple(Mat &Src, vector<vector<Point2i>> &Edge, int SingleMinSum = 10, int SingleMaxSum=1000000, Point2i Left_up = Point2i(0, 0), int Width = 3);

//��2��ʶ����ѩ�����,EdgeΪ�����ĵ��߱�Ե�㣬diameterΪ���ߵĴ�ϸ��intervalΪ���߼�ļ��
void Wire(Mat &Src, vector<vector<Point2i>> &Edge,float& diameter, vector<float> &interval);

//��3��ʶ����ѩ�����,EdgeΪ�����ĵ��߱�Ե�㣬SnowΪ��ѩ�ı�Ե��,snowwidthΪ��ѩ��ȣ�diameterΪ���ߵĴ�ϸ��intervalΪ���߼�ļ��
void  SnowWire(Mat &Src, vector<vector<Point2i>> &Edge, vector<vector<Point2i>> &Snow, vector<float> &snowwidth, float& diameter, vector<float> &interval);