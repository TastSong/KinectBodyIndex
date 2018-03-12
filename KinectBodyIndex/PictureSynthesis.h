#pragma once

#include "include.h"

class CPictureSynthesis
{
public:
	CPictureSynthesis();
	~CPictureSynthesis();
private:
	//kinect�豸
	IKinectSensor* kinect;
	//��������
	IBodyIndexFrameSource *bodyIndexFrameSource;
	IBodyIndexFrameReader *bodyIndexFrameReader;
	IFrameDescription *fd; 
	//���յĺϳ�ͼƬ
	Mat show;  
	//��������ͼ�Ĵ�С
	int width = 0;
	int height = 0;

	IBodyIndexFrame* bodyIndexFrame;

public:
	//������������
	bool Open(IKinectSensor* kinect);
	//ͼƬ�ϳ�
	Mat PictureSynthesis();

private:
	//����
	Mat background;
	//ǰ��
	Mat bodyImg;
	//����ͼƬ����
	int bgNum;

private:
	//����������
	static int bgCount;
	//����ת��
	void ChangeBg();
	//ͼƬ�ƶ�
	Mat move(Mat moveImg);
	//����ͼƬ��������
	void BgCount();

	bool get_filelist_from_dir(string path, vector<string>& files);
};

