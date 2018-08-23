
// GetPicFromUrl_MFCDlg.cpp : ʵ���ļ�
//63-65��Ҫ  GDI+��ʼ������

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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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


// CMFCDlg �Ի���




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

	// ��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���  
	OLE_XSIZE_HIMETRIC hmWidth;
	OLE_YSIZE_HIMETRIC hmHeight;
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	// ��ͼ���Ⱥ͸߶ȵ�λת��Ϊ���ص�λ  
	//#define HIMETRIC_PER_INCH 2540  
	//int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);  
	//int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);  

	// ��ȡ��ʾͼƬ���ڵĿ�Ⱥ͸߶�  
	RECT rtWnd;
	pDC->GetWindow()->GetWindowRect(&rtWnd);
	int iWndWidth = rtWnd.right - rtWnd.left;
	int iWndHeight = rtWnd.bottom - rtWnd.top;

	if (FAILED(pPic->Render(*pDC, x, y, iWndWidth, iWndHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))
	{
		pPic->Release();
		return false;
	}

	//�ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶  
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


// CMFCDlg ��Ϣ�������

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	CBitmap bitmap;
	
	m_imageList.Create(50, 50, ILC_COLOR32 | ILC_COLORDDB | ILC_MASK, 8, 8);

	m_list.SetImageList(&m_imageList, LVSIL_NORMAL);

	//63 - 65����Ҫ  GDI + ��ʼ�����ܣ�������ӣ��޷���ӷ�bmp�ļ�

	CString FileName = _T("j:\\123.jpg");
	HBITMAP       HBitmap;
	Gdiplus::Bitmap bmp(FileName.AllocSysString());
	int sourceWidth = 50;//��bmp�ļ��Ĵ�С�봴����CImageList����һ��
	int sourceHeight = 50;//��bmp�ļ��Ĵ�С�봴����CImageList����һ��
	Bitmap* pThumbnail = (Bitmap*)bmp.GetThumbnailImage(sourceWidth, sourceHeight, NULL, NULL); //�趨����ͼ�Ĵ�С
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

