#include"Wire.h"
#include<string.h>
#include <strstream>


//float转为string
string TranStr(float a) {
	stringstream ss;
	ss << a;
	return ss.str();
}




int main()
{
	// 【1】加载源图像
	Mat src,src1;
	string filename = "1.jpg";  //检测电线可以用”1.jpg”，“2.jpg”，“3.jpg”，“4.jpg”，“5.jpg”，检测雪层的请用“雪层.jpg”

	src = imread(filename, 1);
	src1 = imread(filename);

	if (src1.empty() || src.empty())
	{
		cout << "图片为空，加载错误！" << endl;
		return 0;
	}

	//【2】电线轮廓
	vector<vector<Point>> edge;
	//电线直径
	float zhi;
	//电线间隔
	vector<float> spa;

	//检测电线函数
	Wire(src,edge,zhi,spa);
	//打印电线间隔
	cout << "直径=" << zhi << endl;
	for (int i = 0; i < spa.size(); i++)
		cout << "间隔=" << spa[i] << endl;

	//画出电线边缘
	for (int s = 0; s < edge.size(); s++)
	{
		string name = "WireWidth:" + TranStr(zhi);
		putText(src1, name, Point(edge[s][0].x, edge[s][0].y + 35), 1, 1, Scalar(255, 0, 0));

		for (int d = 0; d < edge[s].size(); d++)
			src1.at<Vec3b>(edge[s][d].y, edge[s][d].x) = { 0,0,255 };
	}


	////【3】电线轮廓
	vector<vector<Point>> S_edge;
	//电线直径
	float S_zhi;
	//电线间隔
	vector<float> S_spa;
	//雪层轮廓
	vector<vector<Point>> S_snow;
	//雪层厚度
	vector<float> S_width;
	////检测雪层函数
	SnowWire(src, S_edge, S_snow, S_width, S_zhi, S_spa);


	//画出雪层边缘
	for (int s = 0; s < S_snow.size(); s++)
	{
		//雪层边缘轮廓点小于等于电线单侧轮廓0.5长度，返回。表示没有雪层存在
		if (S_snow[s].size() >= S_edge[s].size()*0.5*0.5)
		{
			putText(src1, "Have Snow", Point(0, 10), 1, 1, Scalar(255, 0, 0));

			//雪层厚度不为零，进行文字显示
			if (S_width[s] != 0)
			{
				string name = "SnowWidth:" + TranStr(S_width[s]);
				putText(src1, name, Point(S_snow[s][0].x, S_snow[s][0].y - 15), 1, 1, Scalar(255, 0, 0));
			}
			cout << "雪层厚度：" << S_width[s] << endl;
			for (int d = 0; d < S_snow[s].size(); d++)
				src1.at<Vec3b>(S_snow[s][d].y, S_snow[s][d].x) = { 0,255,0 };
		}
		else
		{
			putText(src1, "NO Snow", Point(0, 10), 1, 1, Scalar(255, 0, 0));
			break;
		}

	}

	

	//【4】显示最后的轮廓图
	imshow("结果", src1);

	waitKey();
	return 0;
}