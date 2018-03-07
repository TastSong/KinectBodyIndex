
// KinectBodyIndexDlg.h : ͷ�ļ�
//
#pragma once
#include "CvvImage.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Kinect.h>
#include "PictureSynthesis.h"

#include "include.h"

// CKinectBodyIndexDlg �Ի���
class CKinectBodyIndexDlg : public CDialogEx
{
// ����
public:
	CKinectBodyIndexDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_KINECTBODYINDEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void ChangeSize(UINT nID, int xlabel, int ylabel);
	void ShowInDialog(Mat total);
	void GetOut(void);
	void AllScreen(void);
	void ShowImage(IplImage* TheImage, UINT myId);
	void DrawPicture(void);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	CRect m_rect;
	int dialogWidth;
	int dialogHeight;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	IKinectSensor *kinect = NULL;
	CPictureSynthesis pS;
	int numberScenarios;
};
