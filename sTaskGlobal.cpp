#include "stdafx.h"

#include <wincrypt.h>
#include <atlenc.h>

// 新しい DDV の定義
// 文字数が a から b の間にあることを確認する
void AFXAPI DDV_MinMaxChars(CDataExchange* pDX, CString const& value, int minChars, int maxChars)
{
	ASSERT(minChars >= 1);        // allow them something
	ASSERT(maxChars >= 1);        // allow them something
	if (pDX->m_bSaveAndValidate && (value.GetLength() > maxChars || value.GetLength() < minChars))
	{	// エラーの表示
		CString prompt;
		prompt.Format("%dから%d文字で入力してください", minChars, maxChars);
		AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		prompt.Empty(); // exception prep
		pDX->Fail();
	}

}


// ハッシュを計算する
// 入力
//   pData : 元データ（文字列等）
//   dwdataLen : 元データのバイト数
// 出力
//  pcbHashData : 計算結果のハッシュ値（バイナリ）
//  dwHashLen : ハッシュ値のバイト数 （MD5は128bitなので16バイト、SHA-1は160bitなので20バイトを指定）
// 制御
//  dwFlag : CryptCreateHash関数のALG_ID（CALG_MD5, CALG_SHA1などを指定）
// 戻り値
//  成功の場合 true
bool CalcHash_Raw(const void * pData, DWORD dwDataLen, BYTE *pcbHashData, DWORD dwHashLen, DWORD dwFlag)
{
	bool ret;
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
	BYTE pbHash[0x7f];

	::ZeroMemory(pcbHashData, dwHashLen);
	if(pData == NULL || dwDataLen == 0)
		return false;

	hHash = NULL;
	hCryptProv = NULL;
	ret = false;
	if(::CryptAcquireContext(&hCryptProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
	{
		if(::CryptCreateHash(hCryptProv,dwFlag,0,0,&hHash))
		{
			if(::CryptHashData(hHash,(BYTE*)pData,dwDataLen,0))
			{
				if(::CryptGetHashParam(hHash,HP_HASHVAL,pbHash,&dwHashLen,0))
				{
					::memcpy(pcbHashData,pbHash,dwHashLen);
					ret = true;
				}
			}
		}
	}

	if(hHash)
		::CryptDestroyHash(hHash);
	if(hCryptProv)
		::CryptReleaseContext(hCryptProv,0);

	if(ret == false)
		::ZeroMemory(pcbHashData,dwHashLen);

	return ret;
}

// 文字列のハッシュを計算し、文字列形式で返す
// 入力
//  pStr : 文字列
//  mode : "MD5", "SHA1"
// 戻り値
//  成功の場合ハッシュ文字列。失敗の場合 ""（長さゼロの文字列）
char * CalcHash_String(const char * pStr, char * mode)
{
	BYTE pcbData[256];	// ハッシュ計算値を受け取る（MD5の場合128bit/8=16bytes, SHA1の場合160bit/8=20bytes必要）
	DWORD dwHashByte;
	DWORD dwFlag;
	static char strHash[256];	// ハッシュを文字列変換したもの（MD5の場合128bit/8*2=32bytes, SHA1の場合160bit/8*2=40bytes必要）

	strcpy(strHash, "");

	if(!_stricmp(mode, "MD5"))
	{
		dwHashByte = 16;
		dwFlag = CALG_MD5;
	}
	else if(!_stricmp(mode, "SHA1"))
	{
		dwHashByte = 20;
		dwFlag = CALG_SHA1;
	}
	else return(strHash);

	if(CalcHash_Raw(pStr, strlen(pStr), pcbData, dwHashByte, dwFlag) == true)
	{
		char strCh[3];
		for(int i = 0; i < (int)dwHashByte; i++)
		{
			sprintf(strCh, _T("%02x"), pcbData[i]);
			strcat(strHash, strCh);
		}
	}

	return(strHash);
}