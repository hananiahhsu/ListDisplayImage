
// GetPicFromUrl_MFCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ScrollDialog.h"


// CGetPicFromUrl_MFCDlg �Ի���
class CMFCDlg : public CDialog
{
// ����
public:
	CMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GETPICFROMURL_MFC_DIALOG };

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
public:
	CListCtrl m_list;
	CImageList m_imageList;

	ULONG_PTR m_gdiplusToken;

};
