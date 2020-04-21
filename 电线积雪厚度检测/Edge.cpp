#include"Wire.h"

void Edge_Multiple(Mat &Src, vector<vector<Point2i>> &MEdge, int SingleMinSum, int SingleMaxSum, Point2i Left_up, int width)
{
	if (Src.empty()) {
		//std::cout << "Edge_error��ͼƬΪ�գ��޷�����" << std::endl;
		return;
	}
	Mat dst;
	dst = Src.clone();
	if (dst.channels() == 3) {
		cvtColor(dst, dst, COLOR_RGB2GRAY);
	}
	//ȥë��
	threshold(dst, dst, 0, 255, THRESH_OTSU);
	Mat element = getStructuringElement(MORPH_RECT, Size(width, width));
	dilate(dst, dst, element);
	erode(dst, dst, element);

	int X = dst.cols;
	int Y = dst.rows;
	//�ı�Ⱦɫ
	for (int y = 0; y < Y; y++)
	{
		//�������
		dst.at<uchar>(y, 0) = 1;      //����
		dst.at<uchar>(y, X - 1) = 1;  //β��
	}
	for (int x = 0; x < X; x++)
	{
		dst.at<uchar>(0, x) = 1;       //����
		dst.at<uchar>(Y - 1, x) = 1;   //β��
	}

	vector<Point2i>  Seeds;
	for (int up = 1; up < X - 1; up++)
	{
		//�Ӷ�λ���µĵײ���ʼʶ��һֱ�������׿飬�򷵻ظ������ݽ�������
		if (dst.at<uchar>(1, up) == 0 && (dst.at<uchar>(1, up + 1) + dst.at<uchar>(1, up - 1) >= 255))
			Seeds.push_back(Point2i(up, 1));
	}
	for (int right = 1; right < Y - 1; right++)
	{
		//ͬ��ѭ�����ײ��������Ҷ��ƶ�����ʼ�ɼ����µ����ص�ı�Ե����ֵ���Ҵ���Seeds������
		if (dst.at<uchar>(right, X - 2) == 0 && (dst.at<uchar>(right - 1, X - 2) + dst.at<uchar>(right + 1, X - 2) >= 255))
			Seeds.push_back(Point2i(X - 2, right));
	}
	for (int down = X - 1; down >0; down--)
	{
		if (dst.at<uchar>(Y - 2, down) == 0 && (dst.at<uchar>(Y - 2, down + 1) + dst.at<uchar>(Y - 2, down - 1) >= 255))
			Seeds.push_back(Point2i(down, Y - 2));
	}
	for (int left = Y - 1; left >0; left--)
	{
		if (dst.at<uchar>(left, 1) == 0 && (dst.at<uchar>(left - 1, 1) + dst.at<uchar>(left + 1, 1) >= 255))
			Seeds.push_back(Point2i(1, left));
	}

	vector<Point2i> Edge;
	for (int i = 0; i < Seeds.size(); i++)
	{
	    //���Edge�Ĵ���״̬
		Edge.clear();
		dst.at<uchar>(Seeds[i].y, Seeds[i].x) = 1;
		Edge.push_back(Point(Seeds[i].x, Seeds[i].y));
		//���岼�����ͱ������Ҹ�ֵΪfalse
		bool Stop = false;
		vector<Point3i> Eight_Zone(10);
		//����ֵ����ʱ����ѭ���Ͽ�
		while (!Stop)
		{
			Eight_Zone[0].x = Edge[Edge.size() - 1].x - 1;
			Eight_Zone[0].y = Edge[Edge.size() - 1].y;
			Eight_Zone[0].z = dst.at<uchar>(Eight_Zone[0].y, Eight_Zone[0].x);
			Eight_Zone[1].x = Edge[Edge.size() - 1].x - 1;
			Eight_Zone[1].y = Edge[Edge.size() - 1].y - 1;
			Eight_Zone[1].z = dst.at<uchar>(Eight_Zone[1].y, Eight_Zone[1].x);
			Eight_Zone[2].x = Edge[Edge.size() - 1].x;
			Eight_Zone[2].y = Edge[Edge.size() - 1].y - 1;
			Eight_Zone[2].z = dst.at<uchar>(Eight_Zone[2].y, Eight_Zone[2].x);
			Eight_Zone[3].x = Edge[Edge.size() - 1].x + 1;
			Eight_Zone[3].y = Edge[Edge.size() - 1].y - 1;
			Eight_Zone[3].z = dst.at<uchar>(Eight_Zone[3].y, Eight_Zone[3].x);
			Eight_Zone[4].x = Edge[Edge.size() - 1].x + 1;
			Eight_Zone[4].y = Edge[Edge.size() - 1].y;
			Eight_Zone[4].z = dst.at<uchar>(Eight_Zone[4].y, Eight_Zone[4].x);
			Eight_Zone[5].x = Edge[Edge.size() - 1].x + 1;
			Eight_Zone[5].y = Edge[Edge.size() - 1].y + 1;
			Eight_Zone[5].z = dst.at<uchar>(Eight_Zone[5].y, Eight_Zone[5].x);
			Eight_Zone[6].x = Edge[Edge.size() - 1].x;
			Eight_Zone[6].y = Edge[Edge.size() - 1].y + 1;
			Eight_Zone[6].z = dst.at<uchar>(Eight_Zone[6].y, Eight_Zone[6].x);
			Eight_Zone[7].x = Edge[Edge.size() - 1].x - 1;
			Eight_Zone[7].y = Edge[Edge.size() - 1].y + 1;
			Eight_Zone[7].z = dst.at<uchar>(Eight_Zone[7].y, Eight_Zone[7].x);
			Eight_Zone[8].x = Edge[Edge.size() - 1].x - 1;
			Eight_Zone[8].y = Edge[Edge.size() - 1].y;
			Eight_Zone[8].z = dst.at<uchar>(Eight_Zone[8].y, Eight_Zone[8].x);
			Eight_Zone[9].x = Edge[Edge.size() - 1].x - 1;
			Eight_Zone[9].y = Edge[Edge.size() - 1].y - 1;
			Eight_Zone[9].z = dst.at<uchar>(Eight_Zone[9].y, Eight_Zone[9].x);

			for (int s = 1; s <= 8; s++)
			{
				if (Eight_Zone[s].z == 0 && ((Eight_Zone[s - 1].z + Eight_Zone[s + 1].z) == 255)) {
					Edge.push_back(Point2i(Eight_Zone[s].x, Eight_Zone[s].y));
					dst.at<uchar>(Eight_Zone[s].y, Eight_Zone[s].x) = 1;
					break;
				}
				else
				{
					if (s == 8)
						Stop = true;
				}
			}
		}
		if (Left_up.x != 0 || Left_up.y != 0)
		{
			for (int j = 0; j < Edge.size(); j++)
			{
				Edge[j].x += Left_up.x;
				Edge[j].y += Left_up.y;
			}
		}
		if (Edge.size() > SingleMinSum && Edge.size() > 1 && Edge.size()<SingleMaxSum)
			MEdge.push_back(Edge);
	}
}
