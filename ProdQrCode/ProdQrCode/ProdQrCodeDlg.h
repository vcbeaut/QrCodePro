
// ProdQrCodeDlg.h : ͷ�ļ�
//

#pragma once
#include <string>

// CProdQrCodeDlg �Ի���
class CProdQrCodeDlg : public CDialogEx
{
// ����
public:
	CProdQrCodeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PRODQRCODE_DIALOG };

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
	afx_msg void OnBnClickedOk();
	void ShowQrCode(CString pathStr);
	void SetCheckStatus(BOOL statu = TRUE);
};
