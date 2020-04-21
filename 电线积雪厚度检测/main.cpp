#include"Wire.h"
#include<string.h>
#include <strstream>


//floatתΪstring
string TranStr(float a) {
	stringstream ss;
	ss << a;
	return ss.str();
}




int main()
{
	// ��1������Դͼ��
	Mat src,src1;
	string filename = "1.jpg";  //�����߿����á�1.jpg������2.jpg������3.jpg������4.jpg������5.jpg�������ѩ������á�ѩ��.jpg��

	src = imread(filename, 1);
	src1 = imread(filename);

	if (src1.empty() || src.empty())
	{
		cout << "ͼƬΪ�գ����ش���" << endl;
		return 0;
	}

	//��2����������
	vector<vector<Point>> edge;
	//����ֱ��
	float zhi;
	//���߼��
	vector<float> spa;

	//�����ߺ���
	Wire(src,edge,zhi,spa);
	//��ӡ���߼��
	cout << "ֱ��=" << zhi << endl;
	for (int i = 0; i < spa.size(); i++)
		cout << "���=" << spa[i] << endl;

	//�������߱�Ե
	for (int s = 0; s < edge.size(); s++)
	{
		string name = "WireWidth:" + TranStr(zhi);
		putText(src1, name, Point(edge[s][0].x, edge[s][0].y + 35), 1, 1, Scalar(255, 0, 0));

		for (int d = 0; d < edge[s].size(); d++)
			src1.at<Vec3b>(edge[s][d].y, edge[s][d].x) = { 0,0,255 };
	}


	////��3����������
	vector<vector<Point>> S_edge;
	//����ֱ��
	float S_zhi;
	//���߼��
	vector<float> S_spa;
	//ѩ������
	vector<vector<Point>> S_snow;
	//ѩ����
	vector<float> S_width;
	////���ѩ�㺯��
	SnowWire(src, S_edge, S_snow, S_width, S_zhi, S_spa);


	//����ѩ���Ե
	for (int s = 0; s < S_snow.size(); s++)
	{
		//ѩ���Ե������С�ڵ��ڵ��ߵ�������0.5���ȣ����ء���ʾû��ѩ�����
		if (S_snow[s].size() >= S_edge[s].size()*0.5*0.5)
		{
			putText(src1, "Have Snow", Point(0, 10), 1, 1, Scalar(255, 0, 0));

			//ѩ���Ȳ�Ϊ�㣬����������ʾ
			if (S_width[s] != 0)
			{
				string name = "SnowWidth:" + TranStr(S_width[s]);
				putText(src1, name, Point(S_snow[s][0].x, S_snow[s][0].y - 15), 1, 1, Scalar(255, 0, 0));
			}
			cout << "ѩ���ȣ�" << S_width[s] << endl;
			for (int d = 0; d < S_snow[s].size(); d++)
				src1.at<Vec3b>(S_snow[s][d].y, S_snow[s][d].x) = { 0,255,0 };
		}
		else
		{
			putText(src1, "NO Snow", Point(0, 10), 1, 1, Scalar(255, 0, 0));
			break;
		}

	}

	

	//��4����ʾ��������ͼ
	imshow("���", src1);

	waitKey();
	return 0;
}