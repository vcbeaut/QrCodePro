#pragma once

#include <string>
// CCheckReg 对话框

class CCheckReg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckReg)

public:
	CCheckReg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckReg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHECKREG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	CString ReadRegister();
	BOOL WriteValueReg(CString strParam);
	std::string m_OriginKey;
	CString m_EncodeKey;
};
