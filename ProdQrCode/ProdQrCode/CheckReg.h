#pragma once

#include <string>
// CCheckReg �Ի���

class CCheckReg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckReg)

public:
	CCheckReg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckReg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHECKREG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	CString ReadRegister();
	BOOL WriteValueReg(CString strParam);
	std::string m_OriginKey;
	CString m_EncodeKey;
};
