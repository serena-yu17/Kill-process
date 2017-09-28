
// Kill thunderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Kill thunder.h"
#include "Kill thunderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKillthunderDlg dialog



CKillthunderDlg::CKillthunderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KILLTHUNDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKillthunderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKillthunderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CKillthunderDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CKillthunderDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CKillthunderDlg message handlers

BOOL CKillthunderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetDlgItemText(IDC_EDIT1, L"ThunderPlatform.exe");

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKillthunderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKillthunderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int killing = 0;
WCHAR* filename = new WCHAR[100];

void CKillthunderDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CWnd* lbl = GetDlgItem(IDC_STATIC);
	lbl->SetWindowTextW(L"当前: 禁止运行");
	GetDlgItemText(IDC_EDIT1, filename, 100);
	killing = 1;
	AfxBeginThread(KThread, this);
}


void CKillthunderDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CWnd* lbl = GetDlgItem(IDC_STATIC);
	lbl->SetWindowTextW(L"当前: 允许运行");
	killing = 0;
}

int strcpr(WCHAR* a, WCHAR* b)
{
	int i = 0;
	while (a[i])
	{
		if (a[i] != b[i] && abs(a[i] - b[i]) != 32)
			return 0;
		i++;
	}
	return 1;
}

void killProcByName(WCHAR* fileName)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcpr(fileName, pEntry.szExeFile))
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
				break;
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}


UINT KThread(LPVOID pParam)
{
	while (killing)
	{
		killProcByName(filename);
		Sleep(2000);
	}
	return 0;
}
