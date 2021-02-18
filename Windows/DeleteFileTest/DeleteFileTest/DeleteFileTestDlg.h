
// DeleteFileTestDlg.h: 头文件
//

#pragma once


// CDeleteFileTestDlg 对话框
class CDeleteFileTestDlg : public CDialogEx
{
// 构造
public:
	CDeleteFileTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELETEFILETEST_DIALOG };
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
	DECLARE_MESSAGE_MAP()
public:
	CMenu m_Menu;
	afx_msg void OnBnClickedButton1();
	void TraverseFolder(LPCTSTR szName, LPCTSTR filetype); // 遍历文件夹
	CString m_str;  // 文件路径
	CString m_type; // 文件类型
	BOOL m_project; // 是否选中删除项目
	int del_flag;
	int errorNum;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnAbout();
	afx_msg void OnExit();
};
