#include"Wire.h"

//【2】识别无雪层电线,Edge为检测出的电线边缘点，diameter为电线的粗细，interval为电线间的间距
void Wire(Mat &Src, vector<vector<Point2i>> &Edge, float& diameter, vector<float> &interval)
{
	if (Src.empty())
	{
		printf("读取图片错误，请确定目录下是否有imread函数指定图片存在~！ \n");
	}
	
	//图像转换为灰度图
	if(Src.channels()==3)
		cvtColor(Src, Src, CV_BGR2GRAY);

	imwrite("电线灰度图.bmp",Src);

	// 进行直方图均衡化,进行对比度、亮度增强
	//equalizeHist(Src, Src);
	//imwrite("对比度增强图.bmp", Src);
	
	//图像去噪,任选其一
	//medianBlur(srcImage, srcImage, 3);
	//imwrite("去噪图1.bmp", Src);
	//bilateralFilter(srcImage, srcImage, 4, 8, 2);
	//imwrite("去噪图2.bmp", Src);
	//GaussianBlur(srcImage, srcImage, Size(3,3), 0, 0);
	//imwrite("去噪图3.bmp", Src);
	//blur(srcImage, srcImage, Size(3,3), Point(-1, -1));
	//imwrite("去噪图4.bmp", Src);

	//自适应阈值处理
	Mat Mid;
	adaptiveThreshold(Src, Mid, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 31, 10);
	//imwrite("阈值图.bmp", Mid);
	//腐蚀处理，使电线轮廓更明显
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); 
	erode(Mid, Mid, element);
	imwrite("腐蚀图.bmp", Mid);

	//右边填白，使电线上下右构成封闭的轮廓
	for (int i = 0; i < Mid.rows; i++)
	{
		Mid.at<uchar>(i, Mid.cols - 2) = 255;
		Mid.at<uchar>(i, Mid.cols - 1) = 255;
	}

	//进行电线的轮廓提取，使用Moore边界追踪算法，详见《数字图像处理》刚萨雷斯第三版，边界追踪部分。
	Edge_Multiple(Mid, Edge, Mid.cols*2 - 20, Mid.cols*2+20,Point(0,0),3);
	//此过程后，电线轮廓被提取出来

	//电线直径计算
	vector<float> zhi;
	for (int i = 0; i < Edge.size(); i++)
	{
		int denominator = Edge[i].size()*0.5 - 10;
		int numerator = 0;
		for (int j = 0; j < denominator; j++)
		{
			numerator += abs(Edge[i][j].y - Edge[i][Edge[i].size() - j - 1].y);
		}
		//cout << "直径=" << (float)numerator / denominator-1 << endl;
		zhi.push_back((float)numerator / denominator);
	}
	diameter = 0;
	for (int i = 0; i < zhi.size(); i++)
	{
		diameter+=(float)zhi[i]/zhi.size();
	}

	//间隔计算
	vector<float> szhi;
	for (int i = 0; i < Edge.size()-1; i++)
	{
		int denominator = Src.cols*0.5-30;
		int numerator = 0;
		for (int j = 0; j < denominator; j++)
		{
			numerator += abs(Edge[i][Edge[i].size() - j - 1].y - Edge[i+1][j].y);
		}
		//cout << "间隔=" << (float)numerator / denominator+2 << endl;
		szhi.push_back((float)numerator / denominator);
	}
	for (int s =  szhi.size() - 1; s >= 0; s--)
		interval.push_back(szhi[s]);
}


//【3】识别有雪层电线,Edge为检测出的电线边缘点，Snow为积雪的边缘点,snowwidth为积雪厚度，diameter为电线的粗细，interval为电线间的间距
void  SnowWire(Mat &Src, vector<vector<Point2i>> &Edge, vector<vector<Point2i>> &Snow, vector<float> &snowwidth, float& diameter, vector<float> &interval)
{
	if (Src.empty())
	{
		printf("读取图片错误，请确定目录下是否有imread函数指定图片存在~！ \n");
	}

	//图像转换为灰度图
	if (Src.channels() == 3)
		cvtColor(Src, Src, CV_BGR2GRAY);

	// 进行直方图均衡化,进行对比度、亮度增强
	//equalizeHist(Src, Src);

	//图像去噪
	//medianBlur(srcImage, srcImage, 3);
	//bilateralFilter(srcImage, srcImage, 4, 8, 2);
	//GaussianBlur(srcImage, srcImage, Size(3,3), 0, 0);
	//blur(srcImage, srcImage, Size(3,3), Point(-1, -1));

	//自适应阈值处理
	Mat Mid;
	adaptiveThreshold(Src, Mid, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 31, 10);

	//腐蚀处理，使电线轮廓更明显
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(Mid, Mid, element);

	//右边填白，使电线上下右构成封闭的轮廓
	for (int i = 0; i < Mid.rows; i++)
	{
		Mid.at<uchar>(i, Mid.cols - 2) = 255;
		Mid.at<uchar>(i, Mid.cols - 1) = 255;
	}

	//进行电线的轮廓提取
	Edge_Multiple(Mid, Edge, Mid.cols * 2 - 20, Mid.cols * 2 + 20, Point(0, 0), 3);

	//电线直径计算
	vector<float> zhi;
	for (int i = 0; i < Edge.size(); i++)
	{
		int denominator = Edge[i].size() - 10;
		int numerator = 0;
		for (int j = 0; j < denominator; j++)
		{
			numerator += abs(Edge[i][j].y - Edge[i][Edge[i].size() - j - 1].y);
		}
		//cout << "直径=" << (float)numerator / denominator-1 << endl;
		zhi.push_back((float)numerator / denominator - 1);
	}
	diameter = 0;
	for (int i = 0; i < zhi.size(); i++)
	{
		diameter += (float)zhi[i] / zhi.size();
	}

	//间隔计算
	vector<float> szhi;
	for (int i = 0; i < Edge.size() - 1; i++)
	{
		int denominator = Src.cols - 30;
		int numerator = 0;
		for (int j = 0; j < denominator; j++)
		{
			numerator += abs(Edge[i][Edge[i].size() - j - 1].y - Edge[i + 1][j].y);
		}
		//cout << "间隔=" << (float)numerator / denominator+2 << endl;
		szhi.push_back((float)numerator / denominator + 2);
	}
	for (int s = szhi.size() - 1; s >= 0; s--)
		interval.push_back(szhi[s]);



	//下降度像素值
	int Step = 7;
	//雪层跨度坐标值
	int Width = 20;
	//厚度累计坐标值
	vector<int> s_width;
	//探测雪层厚度
	for (int c = 0; c < Edge.size(); c++)
	{
		vector<Point> single;
		for (int s = Edge[c].size()-1; s > Edge[c].size()*0.5+5; s--)
		{
			if (Edge[c][s].x != Edge[c][s - 1].x)
			{
				int index = 0;
				for (int w = Width; w > 0; w--)
				{
					//width-w
					int ddd = abs(Src.at<uchar>(Edge[c][s].y - Width, Edge[c][s].x) - Src.at<uchar>(Edge[c][s].y-w, Edge[c][s].x));
					if (ddd >= Step)
						index+=1;
					if (index > 4)
					{
						//返回距离值
						s_width.push_back(w+5);
						//返回坐标值
						single.push_back(Point(Edge[c][s].x, Edge[c][s].y - w-5));
						break;
					}
				}
			}
		}
		Snow.push_back(single);
		sort(s_width.begin(),s_width.end());
		int begin = s_width.size()*0.3, end = s_width.size()*0.6;
		float snow_width = 0;
		for (int i = begin; i < end; i++)
		{
			snow_width += (float)s_width[i] / (end - begin);
		}
		snowwidth.push_back(snow_width);
	}
	

}