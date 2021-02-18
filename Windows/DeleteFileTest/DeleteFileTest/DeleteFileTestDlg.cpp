
// DeleteFileTestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DeleteFileTest.h"
#include "DeleteFileTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDeleteFileTestDlg 对话框



CDeleteFileTestDlg::CDeleteFileTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELETEFILETEST_DIALOG, pParent)
	, m_str(_T(""))
	, m_type(_T(""))
	, m_project(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	errorNum = 0;
}

void CDeleteFileTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_type);
	DDX_Text(pDX, IDC_EDIT2, m_str);
	DDX_Check(pDX, IDC_CHECK1, m_project);
}

BEGIN_MESSAGE_MAP(CDeleteFileTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDeleteFileTestDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CDeleteFileTestDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &CDeleteFileTestDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK1, &CDeleteFileTestDlg::OnBnClickedCheck1)
	ON_COMMAND(ID_32771, &CDeleteFileTestDlg::OnAbout)
	ON_COMMAND(ID_32772, &CDeleteFileTestDlg::OnExit)
END_MESSAGE_MAP()

VOID 
CDeleteFileTestDlg::TraverseFolder(
	_In_ LPCTSTR szName,
	_In_ LPCTSTR filetype
)
/*++

Routine Description:

	 Traverse Folder

Arguments:

	 szName   - 删除完整路径 (eg. E:\test\)
     filetype - 删除文件类型 (eg. .txt .exe .xxx)

Return Value:

	None.

--*/
{
	WIN32_FIND_DATAW findFileData = { 0 };
	CString fileRootPath = szName;
	CString filePath = fileRootPath + _T("\\*.*"); // E:\\test\\*.*
	TCHAR szFind[MAX_PATH] = { _T("\0") };
	CString input_type = filetype;
	BOOL bRet;

	HANDLE hwnd = FindFirstFile(filePath, &findFileData);
	if (hwnd == INVALID_HANDLE_VALUE) {
		MessageBox(_T("该文件路径不存在"), _T("错误"), NULL);
		del_flag = 1;
		return;
	}

	while (true)
	{
		if (_tcscmp(findFileData.cFileName, _T(".")) == 0 || // 过滤当前目录
			_tcscmp(findFileData.cFileName, _T("..")) == 0)  // 过滤上级目录
		{
			if (!FindNextFile(hwnd, &findFileData))
			{
				break; // 没有文件了直接退出
			}
			continue;  // 操作下一个文件
		}

		CString totalpath = fileRootPath + _T("\\") + findFileData.cFileName; // 文件完整路径
		int index = totalpath.ReverseFind(_T('.')); // 找到"."在字符串中的位置,这里是转义"\\"和其他符号之后的字符串
		CString path_type = totalpath.Right(totalpath.GetLength() - index); // 获得文件后缀名(eg .txt .exe .xxx)
		
		if (input_type.Find(path_type) != -1) { // 寻找删除的类型,可删除多个类型
			DeleteFile(totalpath); // 删除该类型的文件
		}

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { // 如果为目录
			_tcscpy_s(szFind, MAX_PATH, szName);
			_tcscat_s(szFind, _T("\\"));
			_tcscat_s(szFind, findFileData.cFileName);
			TraverseFolder(szFind, input_type); // 遍历调用
		}

		if (!FindNextFile(hwnd, &findFileData)) {
			errorNum++;
			break;
		}
	}
}


// CDeleteFileTestDlg 消息处理程序

BOOL CDeleteFileTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	m_Menu.LoadMenu(IDR_MENU1);  //  IDR_MENU1
	SetMenu(&m_Menu);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDeleteFileTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDeleteFileTestDlg::OnPaint()
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
HCURSOR CDeleteFileTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void 
CDeleteFileTestDlg::OnBnClickedButton1(
	VOID
)
/*++

Routine Description:

	 delete button callback function.

Arguments:

	 None.

Return Value:

	 None.

--*/
{
	// TODO: 在此添加控件通知处理程序代码
	errorNum = 0;
	if (m_project && m_type != L".obj.tlog.lastbuildstate.idb.pdb.pch.res.ilk.exe.sdf.ipch.log") { 
		m_type = m_type + L".obj.tlog.lastbuildstate.idb.pdb.pch.res.ilk.exe.sdf.ipch.log";
	}

	if (m_str == L"" || m_type == L"") {
		MessageBox(_T("请输入文件路径或类型"), _T("提示"), NULL);
		return;
	}

	TraverseFolder(m_str, m_type);
	
	if (del_flag != 1) {
		MessageBox(_T("已删除该类型文件"), _T("提示"), NULL);
	}
}

void 
CDeleteFileTestDlg::OnEnChangeEdit2(
	VOID
)
/*++

Routine Description:

	 update data.

Arguments:

	 None.

Return Value:

	 None.

--*/
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void 
CDeleteFileTestDlg::OnEnChangeEdit1(
	VOID
)
/*++

Routine Description:

	 update data.

Arguments:

	 None.

Return Value:

	 None.

--*/
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void 
CDeleteFileTestDlg::OnBnClickedCheck1(
	VOID
)
/*++

Routine Description:

	 Check C/C++ project button, if clicked add some types to m_type.

Arguments:

	 None.

Return Value:

	 None.

--*/
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	// 删除C、C++项目,文件类型设置如下
	if (m_project) {
		m_type = _T(".obj.tlog.lastbuildstate.idb.pdb.pch.res.ilk.exe.sdf.ipch.log");
	}
	else {
		m_type = _T("");
	}
}

void 
CDeleteFileTestDlg::OnAbout(
	VOID
)
/*++

Routine Description:

	 Pop adbout menu.

Arguments:

	 None.

Return Value:

	 None.

--*/
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void 
CDeleteFileTestDlg::OnExit(
	VOID
)
/*++

Routine Description:

	 Exit.

Arguments:

	 None.

Return Value:

	 None.

--*/
{
	// TODO: 在此添加命令处理程序代码
	exit(0);
}
