#pragma once

#include "include.h"

class CPictureSynthesis
{
public:
	CPictureSynthesis();
	~CPictureSynthesis();
private:
	//kinect设备
	IKinectSensor* kinect;
	//身体索引
	IBodyIndexFrameSource *bodyIndexFrameSource;
	IBodyIndexFrameReader *bodyIndexFrameReader;
	IFrameDescription *fd; 
	//最终的合成图片
	Mat show;  
	//身体索引图的大小
	int width = 0;
	int height = 0;

	IBodyIndexFrame* bodyIndexFrame;

public:
	//启动身体索引
	bool Open(IKinectSensor* kinect);
	//图片合成
	Mat PictureSynthesis();

private:
	//背景
	Mat background;
	//前景
	Mat bodyImg;
	//背景图片数量
	int bgNum;

private:
	//背景计数器
	static int bgCount;
	//背景转化
	void ChangeBg();
	//图片移动
	Mat move(Mat moveImg);
	//背景图片数量计算
	void BgCount();

	bool get_filelist_from_dir(string path, vector<string>& files);
};

