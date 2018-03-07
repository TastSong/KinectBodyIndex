
// KinectBodyIndexDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KinectBodyIndex.h"
#include "KinectBodyIndexDlg.h"
#include "afxdialogex.h"

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CKinectBodyIndexDlg 对话框



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


// CKinectBodyIndexDlg 消息处理程序

BOOL CKinectBodyIndexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//初始化kinect
	kinect = NULL;
	HRESULT hr = S_OK;
	hr = GetDefaultKinectSensor(&kinect);  //  得到默认的设备  

	if (FAILED(hr) || kinect == NULL)
	{
		//cout << "创建 sensor 失败\n";
		//return;
	}
	if (kinect->Open() != S_OK) // 是否打开成功  
	{
		//cout << "Kinect sensor 没准备好\n";
		//return;
	}

	//this->gC.Open(kinect);
	this->pS.Open(kinect);

	//将窗口设为全屏
	AllScreen();

	SetTimer(1, 33, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKinectBodyIndexDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKinectBodyIndexDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//////////////////////////添加代码////////////////////////////////////
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
	if (pWnd != NULL)     //判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	{
		CRect rec;
		pWnd->GetWindowRect(&rec);  //获取控件变化前的大小
		ScreenToClient(&rec);   //将控件大小装换位在对话框中的区域坐标
		rec.left = rec.left * xlabel / m_rect.Width();  //按照比例调整空间的新位置
		rec.top = rec.top * ylabel / m_rect.Height();
		rec.bottom = rec.bottom * ylabel / m_rect.Height();
		rec.right = rec.right * xlabel / m_rect.Width();
		pWnd->MoveWindow(rec);   //伸缩控件
	}
}
void CKinectBodyIndexDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType != SIZE_MINIMIZED)  //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{
		ChangeSize(IDC_SHOWPICTURE, cx, cy);  //对每一个控件的大小依次做调整

		CRect sizeOfPicture;
		CWnd *pWnd = GetDlgItem(IDC_SHOWPICTURE);//IDC_picture为picture控件ID
		if (pWnd != NULL)
		{
			pWnd->GetWindowRect(&sizeOfPicture);//sizeOfPicture为主显示区域图片控件的大小。
			int x = sizeOfPicture.right;
			int y = sizeOfPicture.bottom;

		}
		GetClientRect(&m_rect);   //最后要更新对话框的大小，当做下一次变化的旧坐标；
	}
}
//在mID为myId的图片控件中显示IplImage*类型的TheImage
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
	//将Mat类型转换为IplImage类型
	IplImage myipl(total);

	IplImage *ipl = &myipl;

	if (!ipl )return;
	//窗口显示图片
	ShowImage(ipl, IDC_SHOWPICTURE);// 利用控件显示图片,传入iplImage*和UNIT类型的参数，在UNIT控件上显示图片，iplIPmage类型决定显示什么图片，函数中需要调整大小与UNIT控件的大小一致

	//ShowImage( ipl4, IDC_NAVIGATE4 );
	//释放内存
	ipl = NULL;

	//ipl4 = NULL;
}



//取消按钮的响应函数里调用的函数，用来销毁二级窗口
void CKinectBodyIndexDlg::GetOut(void)
{

	CDialogEx::OnCancel();
}

void CKinectBodyIndexDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		//this->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);  //将窗口设置为在最前端显示
		DrawPicture();
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}
void CKinectBodyIndexDlg::DrawPicture(void)
{

	Mat test = this->pS.PictureSynthesis();   //********************Mat接口***********************************
	//在窗口显示图片
	ShowInDialog(test);

}
