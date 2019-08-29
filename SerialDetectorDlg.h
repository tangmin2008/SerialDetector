
// SerialDetectorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define UM_TRAYNOTIFY WM_USER + 11

// CSerialDetectorDlg �Ի���
class CSerialDetectorDlg : public CDialogEx
{
	// ����
public:
	CSerialDetectorDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSerialDetectorDlg(void);
	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALDETECTOR_DIALOG };
#endif

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
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	DECLARE_MESSAGE_MAP()
public:
	NOTIFYICONDATA m_nid;
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	void OnSize(UINT nType, int cx, int cy);
	// �����б�
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
