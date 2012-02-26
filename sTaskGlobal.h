
void AFXAPI DDV_MinMaxChars(CDataExchange* pDX, CString const& value, int minChar, int maxChar);
bool CalcHash_Raw(const void * pData, DWORD dwDataLen, BYTE *pcbHashData, DWORD dwHashLen, DWORD dwFlag);
char * CalcHash_String(const char * pStr, char * mode);
