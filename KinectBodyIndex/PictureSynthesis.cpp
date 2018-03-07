#include "stdafx.h"
#include "PictureSynthesis.h"

#include <string>
#include <iostream>
#include <Kinect.h>
#include <iostream>

using namespace std;

CPictureSynthesis::CPictureSynthesis()
{//初始化
	this->kinect = nullptr;
	this->bodyIndexFrameSource = nullptr;
	this->bodyIndexFrameReader = nullptr;
	this->bodyIndexFrame = nullptr;
	this->fd = nullptr;
	this->width = 0;
	this->height = 0;
}

CPictureSynthesis::~CPictureSynthesis()
{
	//记得各种释放
	SafeRelease(bodyIndexFrameSource);
	SafeRelease(bodyIndexFrameReader);
	SafeRelease(fd);

	
}


bool CPictureSynthesis::Open(IKinectSensor* kinect)
{
	this->kinect = kinect;
	//获取身体索引帧的来源。
	kinect->get_BodyIndexFrameSource(&bodyIndexFrameSource);

	//为身体索引框架源创建一个框架阅读器
	bodyIndexFrameSource->OpenReader(&bodyIndexFrameReader);

	//获取身体索引框架的描述
	bodyIndexFrameSource->get_FrameDescription(&fd);
	//获取图像帧的宽度和高度
	fd->get_Width(&width);
	fd->get_Height(&height);

	//创建一种新的 图像帧大小 图像
	Mat temp(height, width, CV_8UC3);
	this->show = temp;

	Mat bg = imread("./Background/bg0.jpg");
	resize(bg, this->background, Size(this->width, this->height));   //调整至彩色图像的大小
	Mat body = imread("./Background/body.jpg");
	resize(body, this->bodyImg, Size(this->width, this->height));   //调整至彩色图像的大小

	return true;
}
Mat CPictureSynthesis::PictureSynthesis()
{
	this->ChangeBg();
	this->bodyImg = this->move(bodyImg);

	HRESULT hResult;
	
	//获取最近的框架。如果没有帧可用，它可能返回null
	hResult = bodyIndexFrameReader->AcquireLatestFrame(&bodyIndexFrame);
	if (SUCCEEDED(hResult))
	{
		unsigned int buffersize = 0;
		unsigned char* buffer = nullptr;
		//获取指向body索引帧数据的指针。将body索引帧数据输入到数组里，里面的数字代表距离    
		hResult = bodyIndexFrame->AccessUnderlyingBuffer(&buffersize, &buffer);
		if (SUCCEEDED(hResult))
		{
			for (int x = 0; x < height; x++)
			{
				for (int y = 0; y < width; y++)

				{
					unsigned int index = x * width + y;
					if (buffer[index] != 255)
					{//show.at返回指定数组元素的引用   人物部分
						show.at<Vec3b>(x, y) = this->bodyImg.at<Vec3b>(x, y);
					}
					else
					{//背景
						show.at<Vec3b>(x, y) = this->background.at<Vec3b>(x, y);
					}

				}
			}
		}
	}
	SafeRelease(bodyIndexFrame);

	if (waitKey(1) == VK_ESCAPE)
		;//空白行

	return show;
}

int CPictureSynthesis::bgCount = 0;
//动态背景的实现
void CPictureSynthesis::ChangeBg()
{
	this->bgCount++;
	this->bgCount = (this->bgCount) % 20;
	//路径
	string str1 = "./Background/bg";
	char str2[100];
	itoa(this->bgCount, str2, 10);
	string str3 = ".jpg";
	string backgroundPath = str1 + str2 + str3;

	Mat bg = imread(backgroundPath);

	resize(bg, this->background, Size(this->width, this->height));   //调整至彩色图像的大小
		
}

//图片移动
Mat CPictureSynthesis::move(Mat moveImg)
{
	Mat result = moveImg;
	Vec3b temp = moveImg.at<Vec3b>(0, 0);

	for (int x = 0; x < this->height; x++)
	{
		for (int y = 0; y < this->width; y++)

		{
			//show.at<Vec3b>(x, y) = this->bodyImg.at<Vec3b>(x, y);
			//Vec3b temp = moveImg.at<Vec3b>(x, y);
			if (0 == y)
			{
				temp = moveImg.at<Vec3b>(x, y);
				result.at<Vec3b>(x, y) = moveImg.at<Vec3b>(x, y + 1);
			}
			else if (y > 0 && y < this->width - 1)
			{
				result.at<Vec3b>(x, y) = moveImg.at<Vec3b>(x, y + 1);
			}
			else
			{
				result.at<Vec3b>(x, y - 1) = temp;
			}
		}
	}

	return result;
}