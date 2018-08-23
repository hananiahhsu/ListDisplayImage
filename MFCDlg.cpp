
// GetPicFromUrl_MFCDlg.cpp : 实现文件
//63-65重要  GDI+初始化功能

#include "stdafx.h"
#include "GetPicFromUrl_MFC.h"
#include "MFCDlg.h"
#include "resource.h"
#include <string>
using namespace std;
#include <afxinet.h>

#include "atlimage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	SetWindowPos(NULL, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMFCDlg 对话框




CMFCDlg::CMFCDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	ULONG_PTR m_gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

BOOL ShowImage(CDC* pDC, CString strPath, int x, int y)
{

	IPicture *pPic = NULL;
	OleLoadPicturePath(CComBSTR(strPath.GetBuffer()), (LPUNKNOWN)NULL, 0, 0, IID_IPicture, (LPVOID*)&pPic);
	if (NULL == pPic)
	{
		return FALSE;
	}

	// 获取图像宽和高,注意这里的宽和高不是图像的分辨率  
	OLE_XSIZE_HIMETRIC hmWidth;
	OLE_YSIZE_HIMETRIC hmHeight;
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	// 将图像宽度和高度单位转化为像素单位  
	//#define HIMETRIC_PER_INCH 2540  
	//int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);  
	//int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);  

	// 获取显示图片窗口的宽度和高度  
	RECT rtWnd;
	pDC->GetWindow()->GetWindowRect(&rtWnd);
	int iWndWidth = rtWnd.right - rtWnd.left;
	int iWndHeight = rtWnd.bottom - rtWnd.top;

	if (FAILED(pPic->Render(*pDC, x, y, iWndWidth, iWndHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))
	{
		pPic->Release();
		return false;
	}

	//记得释放资源，不然会导致内存泄露  
	pPic->Release();

	return true;
}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMFCDlg 消息处理程序

BOOL CMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CBitmap bitmap;
	
	m_imageList.Create(50, 50, ILC_COLOR32 | ILC_COLORDDB | ILC_MASK, 8, 8);

	m_list.SetImageList(&m_imageList, LVSIL_NORMAL);

	//63 - 65行重要  GDI + 初始化功能，如果不加，无法添加非bmp文件

	CString FileName = _T("j:\\123.jpg");
	HBITMAP       HBitmap;
	Gdiplus::Bitmap bmp(FileName.AllocSysString());
	int sourceWidth = 50;//非bmp文件的大小与创建的CImageList保持一致
	int sourceHeight = 50;//非bmp文件的大小与创建的CImageList保持一致
	Bitmap* pThumbnail = (Bitmap*)bmp.GetThumbnailImage(sourceWidth, sourceHeight, NULL, NULL); //设定缩略图的大小
	pThumbnail->GetHBITMAP(Color(255, 255, 255), &HBitmap);
	CBitmap *pImage = CBitmap::FromHandle(HBitmap);

	CRect cr;
	cr.top = 900;
	cr.left = 300;
	GetDlgItem(IDC_BUTTON1)->MoveWindow(&cr);

	int index=m_imageList.Add(pImage, (255, 255, 255));
	int count = m_imageList.GetImageCount();
	for (int i = 0; i < 10; i++)
	{
		m_list.InsertItem(i, _T("1111111"), index);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

