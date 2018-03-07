
// KinectBodyIndexDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KinectBodyIndex.h"
#include "KinectBodyIndexDlg.h"
#include "afxdialogex.h"

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKinectBodyIndexDlg �Ի���



CKinectBodyIndexDlg::CKinectBodyIndexDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKinectBodyIndexDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKinectBodyIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKinectBodyIndexDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CKinectBodyIndexDlg ��Ϣ�������

BOOL CKinectBodyIndexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ��kinect
	kinect = NULL;
	HRESULT hr = S_OK;
	hr = GetDefaultKinectSensor(&kinect);  //  �õ�Ĭ�ϵ��豸  

	if (FAILED(hr) || kinect == NULL)
	{
		//cout << "���� sensor ʧ��\n";
		//return;
	}
	if (kinect->Open() != S_OK) // �Ƿ�򿪳ɹ�  
	{
		//cout << "Kinect sensor û׼����\n";
		//return;
	}

	//this->gC.Open(kinect);
	this->pS.Open(kinect);

	//��������Ϊȫ��
	AllScreen();

	SetTimer(1, 33, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CKinectBodyIndexDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CKinectBodyIndexDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CKinectBodyIndexDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//////////////////////////��Ӵ���////////////////////////////////////
void CKinectBodyIndexDlg::AllScreen(void)
{
	dialogWidth = GetSystemMetrics(SM_CXSCREEN);
	dialogHeight = GetSystemMetrics(SM_CYSCREEN);
	CRect rcTemp;
	rcTemp.BottomRight() = CPoint(dialogWidth, dialogHeight);
	rcTemp.TopLeft() = CPoint(0, 0);
	MoveWindow(&rcTemp);
}

void CKinectBodyIndexDlg::ChangeSize(UINT nID, int xlabel, int ylabel)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL)     //�ж��Ƿ�Ϊ�գ���Ϊ�ڴ��ڴ�����ʱ��Ҳ�����OnSize���������Ǵ�ʱ�����ؼ���û�д�����PwndΪ��
	{
		CRect rec;
		pWnd->GetWindowRect(&rec);  //��ȡ�ؼ��仯ǰ�Ĵ�С
		ScreenToClient(&rec);   //���ؼ���Сװ��λ�ڶԻ����е���������
		rec.left = rec.left * xlabel / m_rect.Width();  //���ձ��������ռ����λ��
		rec.top = rec.top * ylabel / m_rect.Height();
		rec.bottom = rec.bottom * ylabel / m_rect.Height();
		rec.right = rec.right * xlabel / m_rect.Width();
		pWnd->MoveWindow(rec);   //�����ؼ�
	}
}
void CKinectBodyIndexDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (nType != SIZE_MINIMIZED)  //�жϴ����ǲ�����С���ˣ���Ϊ������С��֮�� �����ڵĳ��Ϳ����0����ǰһ�α仯��ʱ�ͻ���ֳ���0�Ĵ������
	{
		ChangeSize(IDC_SHOWPICTURE, cx, cy);  //��ÿһ���ؼ��Ĵ�С����������

		CRect sizeOfPicture;
		CWnd *pWnd = GetDlgItem(IDC_SHOWPICTURE);//IDC_pictureΪpicture�ؼ�ID
		if (pWnd != NULL)
		{
			pWnd->GetWindowRect(&sizeOfPicture);//sizeOfPictureΪ����ʾ����ͼƬ�ؼ��Ĵ�С��
			int x = sizeOfPicture.right;
			int y = sizeOfPicture.bottom;

		}
		GetClientRect(&m_rect);   //���Ҫ���¶Ի���Ĵ�С��������һ�α仯�ľ����ꣻ
	}
}
//��mIDΪmyId��ͼƬ�ؼ�����ʾIplImage*���͵�TheImage
void CKinectBodyIndexDlg::ShowImage(IplImage* TheImage, UINT myId)
{
	CDC* pDC = GetDlgItem(myId)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;

	GetDlgItem(myId)->GetClientRect(&rect);

	SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);

	CvvImage cimg;
	cimg.CopyOf(TheImage);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}


void CKinectBodyIndexDlg::ShowInDialog(Mat total)
{
	//��Mat����ת��ΪIplImage����
	IplImage myipl(total);

	IplImage *ipl = &myipl;

	if (!ipl )return;
	//������ʾͼƬ
	ShowImage(ipl, IDC_SHOWPICTURE);// ���ÿؼ���ʾͼƬ,����iplImage*��UNIT���͵Ĳ�������UNIT�ؼ�����ʾͼƬ��iplIPmage���;�����ʾʲôͼƬ����������Ҫ������С��UNIT�ؼ��Ĵ�Сһ��

	//ShowImage( ipl4, IDC_NAVIGATE4 );
	//�ͷ��ڴ�
	ipl = NULL;

	//ipl4 = NULL;
}



//ȡ����ť����Ӧ��������õĺ������������ٶ�������
void CKinectBodyIndexDlg::GetOut(void)
{

	CDialogEx::OnCancel();
}

void CKinectBodyIndexDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
		//this->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);  //����������Ϊ����ǰ����ʾ
		DrawPicture();
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}
void CKinectBodyIndexDlg::DrawPicture(void)
{

	Mat test = this->pS.PictureSynthesis();   //********************Mat�ӿ�***********************************
	//�ڴ�����ʾͼƬ
	ShowInDialog(test);

}
