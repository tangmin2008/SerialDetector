
// SerialDetectorDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define UM_TRAYNOTIFY WM_USER + 11

// CSerialDetectorDlg 对话框
class CSerialDetectorDlg : public CDialogEx
{
	// 构造
public:
	CSerialDetectorDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CSerialDetectorDlg(void);
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALDETECTOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	DECLARE_MESSAGE_MAP()
public:
	NOTIFYICONDATA m_nid;
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	void OnSize(UINT nType, int cx, int cy);
	// 串口列表
	CListCtrl m_SerialList;
	CStringArray m_saTmpCom;
	CButton m_RefreshSerialList;
	CString GetComPortType(CString portName);
	int GetDeviceCOM();
	void Sort();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton2();
};
