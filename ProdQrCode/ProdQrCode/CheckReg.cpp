// CheckReg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProdQrCode.h"
#include "CheckReg.h"
#include "afxdialogex.h"

#include "AES.h"
// CCheckReg 对话框

IMPLEMENT_DYNAMIC(CCheckReg, CDialogEx)

CCheckReg::CCheckReg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckReg::IDD, pParent)
{
}

CCheckReg::~CCheckReg()
{
}

void CCheckReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckReg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCheckReg::OnBnClickedOk)
END_MESSAGE_MAP()


void getCpuInfo(unsigned int cpuinfo[4], unsigned int fn_id)
{
	unsigned int deax, debx, decx, dedx;
#ifdef WIN32
	__asm
	{
		mov eax, fn_id; 将参数赋值给eax
			cpuid; 执行cpuid指令
			mov deax, eax; 将寄存器值赋值给临时变量
			mov debx, ebx
			mov decx, ecx
			mov dedx, edx
	}

#elif defined(__GNUC__)
	__asm__("cpuid"
		:"=a"(deax),
		"=b"(debx),
		"=c"(decx),
		"=d"(dedx)
		: "a"(fn_id));
#endif
	cpuinfo[0] = deax;
	cpuinfo[1] = debx;
	cpuinfo[2] = dedx;
	cpuinfo[3] = decx;
}

std::string getCpuStr(){
	unsigned int cpuInfo[4] = { 0 };
	getCpuInfo(cpuInfo, 0);
	std::string cpuStr;
	for (int i = 0; i < 4; i++){
		cpuStr += std::to_string(cpuInfo[i]);
	}
	return cpuStr;
}

void encodeAes(char *source, char *dest){
	char miwen_hex[1024] = { 0 };
	//char result[1024];
	unsigned char key[] = "@!#rssrhtmy#!@";
	AES aes(key);
	aes.Cipher(source, miwen_hex);
	strcpy(dest, miwen_hex);
}


// CCheckReg 消息处理程序
#include "Md5.h"

std::string GetMd5Str(std::string Input){
	char Result[33] = { 0 };
	int Len = Input.length();
	unsigned char dst[16];
	char tmp[3];

	MD5_CTX  mc;
	MD5Init(&mc);
	MD5Update(&mc, (unsigned char *)Input.c_str(), Len);
	MD5Final(&mc, dst);

	for (int i = 0; i<16; i++)
	{
		sprintf(tmp, "%02X", dst[i]);
		tmp[2] = 0;
		strncat(Result, tmp, 2);
	}
	Result[32] = 0;
	std::string retStr;
	retStr = Result;
	return retStr;
}

void CCheckReg::OnBnClickedOk()
{
	CString writeStr;
	GetDlgItemText(IDC_REGISTER_CODE, writeStr);
	BOOL bWrite = WriteValueReg(writeStr);
	if (writeStr.Find(m_EncodeKey) != -1){
		MessageBox(L"注册成功!");
		CDialogEx::OnOK();
	}
	else
	{
		MessageBox(L"注册失败!");
	}
	
}


BOOL CCheckReg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	USES_CONVERSION;
	m_OriginKey = getCpuStr();
	CString macShowStr;
	std::string md5Str = GetMd5Str(m_OriginKey);
	macShowStr = md5Str.c_str();
	SetDlgItemText(IDC_MACHINE_CODE, macShowStr);
	if (macShowStr.IsEmpty()){
		char dst[1024] = { 0 };
	}

	char dst[1024] = { 0 };
	std::string originStr;
	originStr = T2A(macShowStr);
	encodeAes((char *)(originStr).c_str(), dst);
	m_EncodeKey = dst;
	CString regResult = ReadRegister();
	if (regResult.Find(m_EncodeKey) != -1){
		MessageBox(L"未注册，请联系管理员注册！");
		
	}
	else
	{
		CDialogEx::OnOK();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

CString CCheckReg::ReadRegister(){
	HKEY hKey = NULL;
	TCHAR *lpszSubKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszSubKey, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);

	if (lRet == ERROR_SUCCESS){
		DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
		lRet = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount, &maxValueNameLen, &maxValueDataLen, NULL, NULL);
		if (lRet == ERROR_SUCCESS) {
			DWORD dwType = 0;
			BYTE *lpData = new BYTE[maxValueDataLen + 1];
			ZeroMemory(lpData, maxValueDataLen + 1);
			lRet = RegQueryValueEx(hKey, _T("CheakStr"), NULL, &dwType, lpData, &maxValueDataLen);
			if (lRet == ERROR_SUCCESS){
				CString strValue;
				strValue.Format(_T("%s"), lpData);
				//MessageBox(strValue);
				delete[] lpData;

				return strValue;
			}
			else{
				//未注册
				return L"";
			}

		}
		RegCloseKey(hKey);
	}
	else{
		return L"";
	}

}



BOOL CCheckReg::WriteValueReg(CString strParam){
	HKEY hKey = NULL;
	TCHAR *lpszSubKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszSubKey, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	if (lRet == ERROR_SUCCESS) {

		lRet = RegSetValueEx(hKey, _T("CheakStr"), 0, REG_SZ, (LPBYTE)strParam.GetBuffer(), strParam.GetLength()*sizeof(TCHAR));
		if (lRet == ERROR_SUCCESS) {
			return TRUE;
		}
		else{
			return FALSE;
		}

		RegCloseKey(hKey);
	}
}