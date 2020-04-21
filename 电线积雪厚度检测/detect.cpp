#include"Wire.h"

//��2��ʶ����ѩ�����,EdgeΪ�����ĵ��߱�Ե�㣬diameterΪ���ߵĴ�ϸ��intervalΪ���߼�ļ��
void Wire(Mat &Src, vector<vector<Point2i>> &Edge, float& diameter, vector<float> &interval)
{
	if (Src.empty())
	{
		printf("��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ��ͼƬ����~�� \n");
	}
	
	//ͼ��ת��Ϊ�Ҷ�ͼ
	if(Src.channels()==3)
		cvtColor(Src, Src, CV_BGR2GRAY);

	imwrite("���߻Ҷ�ͼ.bmp",Src);

	// ����ֱ��ͼ���⻯,���жԱȶȡ�������ǿ
	//equalizeHist(Src, Src);
	//imwrite("�Աȶ���ǿͼ.bmp", Src);
	
	//ͼ��ȥ��,��ѡ��һ
	//medianBlur(srcImage, srcImage, 3);
	//imwrite("ȥ��ͼ1.bmp", Src);
	//bilateralFilter(srcImage, srcImage, 4, 8, 2);
	//imwrite("ȥ��ͼ2.bmp", Src);
	//GaussianBlur(srcImage, srcImage, Size(3,3), 0, 0);
	//imwrite("ȥ��ͼ3.bmp", Src);
	//blur(srcImage, srcImage, Size(3,3), Point(-1, -1));
	//imwrite("ȥ��ͼ4.bmp", Src);

	//����Ӧ��ֵ����
	Mat Mid;
	adaptiveThreshold(Src, Mid, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 31, 10);
	//imwrite("��ֵͼ.bmp", Mid);
	//��ʴ����ʹ��������������
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); 
	erode(Mid, Mid, element);
	imwrite("��ʴͼ.bmp", Mid);

	//�ұ���ף�ʹ���������ҹ��ɷ�յ�����
	for (int i = 0; i < Mid.rows; i++)
	{
		Mid.at<uchar>(i, Mid.cols - 2) = 255;
		Mid.at<uchar>(i, Mid.cols - 1) = 255;
	}

	//���е��ߵ�������ȡ��ʹ��Moore�߽�׷���㷨�����������ͼ����������˹�����棬�߽�׷�ٲ��֡�
	Edge_Multiple(Mid, Edge, Mid.cols*2 - 20, Mid.cols*2+20,Point(0,0),3);
	//�˹��̺󣬵�����������ȡ����

	//����ֱ������
	vector<float> zhi;
	for (int i = 0; i < Edge.size(); i++)
	{
		int denominator = Edge[i].size()*0.5 - 10;
		int numerator = 0;
		for (int j = 0; j < denominator; j++)
		{
			numerator += abs(Edge[i][j].y - Edge[i][Edge[i].size() - j - 1].y);
		}
		//cout << "ֱ��=" << (float)numerator / denominator-1 << endl;
		zhi.push_back((float)numerator / denominator);
	}
	diameter = 0;
	for (int i = 0; i < zhi.size(); i++)
	{
		diameter+=(float)zhi[i]/zhi.size();
	}

	//�������
	vector<float> szhi;
	for (int i = 0; i < Edge.size()-1; i++)
	{
		int denominator = Src.cols*0.5-30;
		int numerator = 0;
		for (int j = 0; j < denominator; j++)
		{
			numerator += abs(Edge[i][Edge[i].size() - j - 1].y - Edge[i+1][j].y);
		}
		//cout << "���=" << (float)numerator / denominator+2 << endl;
		szhi.push_back((float)numerator / denominator);
	}
	for (int s =  szhi.size() - 1; s >= 0; s--)
		interval.push_back(szhi[s]);
}


//��3��ʶ����ѩ�����,EdgeΪ�����ĵ��߱�Ե�㣬SnowΪ��ѩ�ı�Ե��,snowwidthΪ��ѩ��ȣ�diameterΪ���ߵĴ�ϸ��intervalΪ���߼�ļ��
void  SnowWire(Mat &Src, vector<vector<Point2i>> &Edge, vector<vector<Point2i>> &Snow, vector<float> &snowwidth, float& diameter, vector<float> &interval)
{
	if (Src.empty())
	{
		printf("��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ��ͼƬ����~�� \n");
	}

	//ͼ��ת��Ϊ�Ҷ�ͼ
	if (Src.channels() == 3)
		cvtColor(Src, Src, CV_BGR2GRAY);

	// ����ֱ��ͼ���⻯,���жԱȶȡ�������ǿ
	//equalizeHist(Src, Src);

	//ͼ��ȥ��
	//medianBlur(srcImage, srcImage, 3);
	//bilateralFilter(srcImage, srcImage, 4, 8, 2);
	//GaussianBlur(srcImage, srcImage, Size(3,3), 0, 0);
	//blur(srcImage, srcImage, Size(3,3), Point(-1, -1));

	//����Ӧ��ֵ����
	Mat Mid;
	adaptiveThreshold(Src, Mid, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 31, 10);

	//��ʴ����ʹ��������������
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(Mid, Mid, element);

	//�ұ���ף�ʹ���������ҹ��ɷ�յ�����
	for (int i = 0; i < Mid.rows; i++)
	{
		Mid.at<uchar>(i, Mid.cols - 2) = 255;
		Mid.at<uchar>(i, Mid.cols - 1) = 255;
	}

	//���е��ߵ�������ȡ
	Edge_Multiple(Mid, Edge, Mid.cols * 2 - 20, Mid.cols * 2 + 20, Point(0, 0), 3);

	//����ֱ������
	vector<float> zhi;
	for (int i = 0; i < Edge.size(); i++)
	{
		int denominator = Edge[i].size() - 10;
		int numerator = 0;
		for (int j = 0; j < denominator; j++)
		{
			numerator += abs(Edge[i][j].y - Edge[i][Edge[i].size() - j - 1].y);
		}
		//cout << "ֱ��=" << (float)numerator / denominator-1 << endl;
		zhi.push_back((float)numerator / denominator - 1);
	}
	diameter = 0;
	for (int i = 0; i < zhi.size(); i++)
	{
		diameter += (float)zhi[i] / zhi.size();
	}

	//�������
	vector<float> szhi;
	for (int i = 0; i < Edge.size() - 1; i++)
	{
		int denominator = Src.cols - 30;
		int numerator = 0;
		for (int j = 0; j < denominator; j++)
		{
			numerator += abs(Edge[i][Edge[i].size() - j - 1].y - Edge[i + 1][j].y);
		}
		//cout << "���=" << (float)numerator / denominator+2 << endl;
		szhi.push_back((float)numerator / denominator + 2);
	}
	for (int s = szhi.size() - 1; s >= 0; s--)
		interval.push_back(szhi[s]);



	//�½�������ֵ
	int Step = 7;
	//ѩ��������ֵ
	int Width = 20;
	//����ۼ�����ֵ
	vector<int> s_width;
	//̽��ѩ����
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
						//���ؾ���ֵ
						s_width.push_back(w+5);
						//��������ֵ
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