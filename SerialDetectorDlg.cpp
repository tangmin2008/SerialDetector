
// SerialDetectorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDetector.h"
#include "SerialDetectorDlg.h"
#include "afxdialogex.h"
#include "Dbt.h"
#include "setupapi.h"
#include "conio.h"

#pragma comment(lib, "setupapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSerialDetectorDlg �Ի���



CSerialDetectorDlg::CSerialDetectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERIALDETECTOR_DIALOG, pParent)
{
	//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(&m_nid, 0, sizeof(m_nid)); // Initialize NOTIFYICONDATA struct
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
}

CSerialDetectorDlg::~CSerialDetectorDlg(void) {
	m_nid.hIcon = NULL;
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}
void CSerialDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SerialList);
	DDX_Control(pDX, IDC_BUTTON1, m_RefreshSerialList);
}

BEGIN_MESSAGE_MAP(CSerialDetectorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSerialDetectorDlg::OnBnClickedButton1)
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDCANCEL, &CSerialDetectorDlg::OnBnClickedCancel)
	ON_MESSAGE(UM_TRAYNOTIFY, &CSerialDetectorDlg::OnTrayNotify)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CSerialDetectorDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


BOOL CSerialDetectorDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	switch (nEventType)
	{
		//���ڱ��Ƴ�  
	case DBT_DEVICEREMOVECOMPLETE:
		GetDeviceCOM();
		//���ڲ�����߱�Ϊ��Ч
	case DBT_DEVICEARRIVAL:
		//EnumSerialPort(m_saTmpCom);
		GetDeviceCOM();
		ShowWindow(SW_SHOW);
		break;

	default:
		break;
	}

	return TRUE;
}

// CSerialDetectorDlg ��Ϣ�������

BOOL CSerialDetectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	//AllocConsole();
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

	m_nid.hWnd = GetSafeHwnd();
	m_nid.uCallbackMessage = UM_TRAYNOTIFY;

	// Set tray icon and tooltip
	m_nid.hIcon = m_hIcon;
	// Set tray notification tip information
	CString strToolTip = _T("����֡�⹤��");
	_tcsncpy_s(m_nid.szTip, strToolTip, strToolTip.GetLength());
	Shell_NotifyIcon(NIM_ADD, &m_nid);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_SerialList.DeleteColumn(0);
	m_SerialList.InsertColumn(0, (LPCWSTR)_T("Port"), LVCFMT_LEFT, 160, -1);
	m_SerialList.InsertColumn(1, (LPCWSTR)_T("Device Name"), LVCFMT_LEFT, 500, -1);
	GetDeviceCOM();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSerialDetectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialDetectorDlg::OnPaint()
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
HCURSOR CSerialDetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CSerialDetectorDlg::GetComPortType(CString portName)
{
	portName = _T("COM3");

	return _T("");
}

void CSerialDetectorDlg::OnBnClickedButton1()
{
	m_SerialList.DeleteAllItems();
	for (int i = 0; i < m_saTmpCom.GetSize(); i++)
	{
		m_SerialList.InsertItem(i, m_saTmpCom.GetAt(i));
	}
	GetDeviceCOM();
}


void CSerialDetectorDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

LRESULT CSerialDetectorDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_RBUTTONUP:				// �Ҽ�����ʱ�����˵�
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);                    // �õ����λ��
		CMenu menu;
		menu.CreatePopupMenu();                    // ����һ������ʽ�˵�
		menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�˳�"));//Appends a new item to the end of this menu
													 //Displays a floating pop-up menu at the specified location and tracks the selection of items on the pop-up menu
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
		break;
	}
	case WM_LBUTTONDBLCLK:                            // ˫������Ĵ���
		this->ShowWindow(SW_SHOWNORMAL);         // ��ʾ������	
		break;
	}

	return 0;
}

void CSerialDetectorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED) {
		ShowWindow(SW_HIDE);
	}
}

#ifndef GUID_DEVCLASS_PORTS
DEFINE_GUID(GUID_DEVCLASS_PORTS, 0x4D36E978, 0xE325, 0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);
#endif

int CSerialDetectorDlg::GetDeviceCOM()
{
	int iPortNum = -1;
	// �õ��豸��Ϣ�� 
	HDEVINFO hDevInfo = SetupDiGetClassDevs((LPGUID)&GUID_DEVCLASS_PORTS, NULL, 0, DIGCF_PRESENT/*|DIGCF_ALLCLASSES*/);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(hDevInfo);
		return iPortNum;
	}
	TCHAR szBuf[MAX_PATH];
	SP_DEVINFO_DATA spDevInfoData = { sizeof(SP_DEVINFO_DATA) };
	// ��ʼ�о��豸 
	DWORD i = 0;
	CString str1, mfgstr, str2;
	m_SerialList.DeleteAllItems();
	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
	{
		// �õ��豸�ͺ� 
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)szBuf, MAX_PATH, NULL))
		{
			str2.Format(_T("%s"), (LPWSTR)szBuf);
			int len = str2.ReverseFind(_T('('));
			CString comportname1 = str2.Right(str2.GetLength() - len - 1);
			CString comportname = comportname1.Left(comportname1.GetLength() - 1);
			m_SerialList.InsertItem(i, comportname);
		}
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_MFG, NULL, (PBYTE)szBuf, MAX_PATH, NULL))
		{
			mfgstr.Format(_T("[%s]"), (LPWSTR)szBuf);
		}

		// �õ��豸���� 
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)szBuf, MAX_PATH, NULL))
		{
			mfgstr.AppendFormat(_T(" %s"), (LPWSTR)szBuf);
			m_SerialList.SetItemText(i, 1, mfgstr);
		}
	}

	Sort();

	return iPortNum;
}

static int CALLBACK CompareFunc(LPARAM   lParam1, LPARAM   lParam2, LPARAM   lParamSort)
{
	CListCtrl *pListCtrl = (CListCtrl *)lParamSort;//{�����涼�ǹ̶��﷨,�ʵ��˽�
	LVFINDINFO findInfo;
	findInfo.flags = LVFI_PARAM;
	findInfo.lParam = lParam1;
	int iItem1 = pListCtrl->FindItem(&findInfo, -1);
	findInfo.lParam = lParam2;
	int iItem2 = pListCtrl->FindItem(&findInfo, -1);

	CString strItem1 = pListCtrl->GetItemText(iItem1, 0);
	CString strItem2 = pListCtrl->GetItemText(iItem2, 0);

	return strItem1.CompareNoCase(strItem2) >= 0;

}
void CSerialDetectorDlg::Sort()
{
	int count = m_SerialList.GetItemCount();
	for (int i = 0; i < count; i++)
		m_SerialList.SetItemData(i, i);
	m_SerialList.SortItems(CompareFunc, (LPARAM)(&m_SerialList));

}



void CSerialDetectorDlg::OnBnClickedButton2()
{
	ShowWindow(SW_HIDE);
}
