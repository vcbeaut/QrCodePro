
// ProdQrCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProdQrCode.h"
#include "ProdQrCodeDlg.h"
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
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProdQrCodeDlg 对话框



CProdQrCodeDlg::CProdQrCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProdQrCodeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APPLE);
	
}

void CProdQrCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProdQrCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CProdQrCodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()




// CProdQrCodeDlg 消息处理程序



BOOL CProdQrCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	
	SetCheckStatus();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}





void CProdQrCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProdQrCodeDlg::OnPaint()
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
HCURSOR CProdQrCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include "include\qrcodeexport.h"
#include "UrlEncode.h"

#pragma comment(lib,"QrCodeDll.lib")


void CProdQrCodeDlg::ShowQrCode(CString pathStr){
	CImage img;
	img.Load(pathStr);
	CRect rt;
	GetDlgItem(IDC_PIC_CODE)->GetClientRect(&rt);
	img.Draw(GetDlgItem(IDC_PIC_CODE)->GetDC()->m_hDC, rt.left, rt.top, rt.Width(), rt.Height());
}

void CProdQrCodeDlg::SetCheckStatus(BOOL statu) {
	((CButton *)GetDlgItem(IDC_CHECK_QWORD))->SetCheck(BST_CHECKED);
	((CButton *)GetDlgItem(IDC_CHECK_STARTPRICE))->SetCheck(BST_CHECKED);
	((CButton *)GetDlgItem(IDC_CHECK_ENDPRICE))->SetCheck(BST_CHECKED);
	((CButton *)GetDlgItem(IDC_CHECK_LOC))->SetCheck(BST_CHECKED);
}

void CProdQrCodeDlg::OnBnClickedOk()
{
	TCHAR projPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, projPath);
	CString fileNameW = projPath;
	fileNameW = fileNameW + L"\\qrcode.bmp";
	USES_CONVERSION;
	char *fileNameA = T2A(fileNameW);
	
	//免运费：service_myf
	//天猫：tab_mall
	//全球购：service_hwsp
	CString qWord, startPrice, endPrice, loc, filter;
	CString urlstr = L"https://s.m.taobao.com/h5?style=list";
	if (((CButton *)GetDlgItem(IDC_CHECK_QWORD))->GetCheck() == BST_CHECKED) {
		GetDlgItemText(IDC_EDIT_QWORD, qWord);
		if (qWord.IsEmpty()) {
			MessageBox(L"请输入关键词");
			return;
		}
		else{
			std::string wordStrA = UrlEncode(UnicodeToUtf8(qWord.GetBuffer()));
			qWord = wordStrA.c_str();
			qWord.Replace(L"+", L"%20");
			urlstr = urlstr + L"&q=" + qWord;
		}
	}

	if (((CButton *)GetDlgItem(IDC_CHECK_STARTPRICE))->GetCheck() == BST_CHECKED) {
		GetDlgItemText(IDC_EDIT_STARTPRICE, startPrice);
		if (startPrice.IsEmpty()) {
			MessageBox(L"请输入最低价格");
			return;
		}
		else {
			urlstr = urlstr + L"&start_price=" +startPrice;
		}
	}
	
	if (((CButton *)GetDlgItem(IDC_CHECK_ENDPRICE))->GetCheck() == BST_CHECKED) {
		GetDlgItemText(IDC_EDIT_ENDPRICE, endPrice);
		if (endPrice.IsEmpty()) {
			MessageBox(L"请输入最高价格");
			return;
		}
		else {
			urlstr = urlstr + L"&end_price=" + endPrice;
		}
	}

	if (((CButton *)GetDlgItem(IDC_CHECK_LOC))->GetCheck() == BST_CHECKED) {
		GetDlgItemText(IDC_EDIT_LOC, loc);
		if (loc.IsEmpty()) {
			MessageBox(L"请输入地区");
			return;
		}
		else {
			std::string wordStrA = UrlEncode(UnicodeToUtf8(loc.GetBuffer()));
			loc = wordStrA.c_str();
			loc.Replace(L"+", L"%20");
			urlstr = urlstr + L"&loc=" + loc;
		}
	}
	
	if (((CButton *)GetDlgItem(IDC_CHECK_SERVICE_MYF))->GetCheck() == BST_CHECKED) {
		if (filter.IsEmpty())
			filter = filter + L"service_myf";
		else
			filter = filter + L";service_myf";
	}
	if (((CButton *)GetDlgItem(IDC_CHECK_TAB_MALL))->GetCheck() == BST_CHECKED) {
		if (filter.IsEmpty())
			filter = filter + L"tab_mall";
		else
			filter = filter + L";tab_mall";
	}
	if (((CButton *)GetDlgItem(IDC_CHECK_SERVICE_HWSP))->GetCheck() == BST_CHECKED) {
		if (filter.IsEmpty())
			filter = filter + L"service_hwsp";
		else
			filter = filter + L";service_hwsp";
	}
	if (!filter.IsEmpty())
		urlstr = urlstr + L"&filter=" + filter;
	
	
	//CString configStr = projPath;
	//configStr = configStr + L"\\config.ini";
	//CString urlstr,dataStr1, dataStr2, dataStr3;
	//GetPrivateProfileString(L"QRCODE", L"SAMPLE", NULL, urlstr.GetBuffer(MAX_PATH), MAX_PATH, configStr);
	/*GetPrivateProfileString(L"QRCODE", L"DATA1", NULL, dataStr1.GetBuffer(), MAX_PATH, configStr);
	GetPrivateProfileString(L"QRCODE", L"DATA2", NULL, dataStr2.GetBuffer(), MAX_PATH, configStr);
	GetPrivateProfileString(L"QRCODE", L"DATA3", NULL, dataStr3.GetBuffer(), MAX_PATH, configStr);*/
	//urlstr.ReleaseBuffer();	
	//if (urlstr.IsEmpty()){
	//	WritePrivateProfileString(L"QRCODE", L"SAMPLE", L"", configStr);
	//	/*WritePrivateProfileString(L"QRCODE", L"DATA1", L"", configStr);
	//	WritePrivateProfileString(L"QRCODE", L"DATA2", L"", configStr);
	//	WritePrivateProfileString(L"QRCODE", L"DATA3", L"", configStr);*/
	//	MessageBox(L"请配置config.ini文件");
	//	return;
	//}
	//CString sourceDataW = urlstr;
	////sourceDataW.Format(L"", qWord, startPrice, endPrice);
	//sourceDataW.Replace(L"DATA1",qWord);
	//sourceDataW.Replace(L"DATA2", startPrice);
	//sourceDataW.Replace(L"DATA3", endPrice); 
	//sourceDataW.Replace(L"filter", filter);
	//char *sourceData = T2A(sourceDataW);
	char *sourceData = T2A(urlstr);
	CreateQrCode(sourceData, fileNameA);
	ShowQrCode(fileNameW);
	MessageBox(L"恭喜!!!生成成功");
}
