// dummyz@126.com

#ifndef _CMDLINE_PARSER_INC_
#define _CMDLINE_PARSER_INC_

#include <list>
#include <atlcoll.h>
#include <ShlObj.h>
#include <ShellAPI.h>

class CCmdLineParser  
{
public:
	enum {
		DEF_CUR_DIR		= 0x0001, // �ڵ�ǰĿ¼����
		DEF_ENV_VAR		= 0x0002, // ��Ĭ�ϵĻ�����������
		DEF_EXT_NAME	= 0x0004, // ʹ��Ĭ�Ͽ�ִ�е���չ��
		NEED_PARAM		= 0x0008,  // ��������
		DEF_FLAG		= DEF_CUR_DIR | DEF_ENV_VAR | DEF_EXT_NAME | NEED_PARAM
	};

public:
	CCmdLineParser(DWORD dwFlag = DEF_FLAG);
	CCmdLineParser(LPCTSTR lpCmdLine /*dwFlag = DEF_CUR_DIR | DEF_ENV_VAR | DEF_EXT_NAME | NEED_PARAM*/);
	virtual ~CCmdLineParser();
	
	BOOL	DoParser(LPCTSTR lpCmdLine, 
		LPCTSTR lpCurDir = NULL, 
		LPCTSTR lpEnvVars = NULL, 
		LPCTSTR lpExtNames = NULL,
		LPCTSTR lpParentPath = NULL
		);

	void	SetFlag(DWORD dwFlag) {
		m_dwFlag = dwFlag;
	}

	LPCTSTR GetCmd() const {
		return (m_szCmd[0] != 0) ? m_szCmd : NULL;
	}

	LPCTSTR GetParam() const {
		return m_lpParam;
	}

private:
	BOOL	ExpandExtNames(LPTSTR lpFilePath, LPCTSTR lpExtNames);
	BOOL	ExpandEnvVars(LPTSTR lpFilePath, LPCTSTR lpEnvVars, LPCTSTR lpExtNames);

	LPTSTR	ExpandMacro(LPCTSTR lpCmdBegin);
	LPTSTR	ExpandMacro2(LPCTSTR lpCmdBegin);
	LPTSTR	ExpandRelative(LPCTSTR lpCmdBegin, LPCTSTR lpCurDir, BOOL bQuotation);
	BOOL	ExpandAbsolute(LPCTSTR lpCmdBegin, LPCTSTR lpExtNames, BOOL bQuotation);

	void	ExpandParam(LPCTSTR lpCmdEnd);

private:
	TCHAR	m_szCmd[MAX_PATH + 10]; // �༸���ֽڷ�ֹ���

	LPTSTR	m_lpParam;
	DWORD	m_dwFlag;
};

DWORD GetFileListFromCmd(CCmdLineParser& cmdParser, CAtlArray<CString>& fileArray, LPCTSTR lpCurDir);

class CRunoptCmdLineParser
{
public:
	CRunoptCmdLineParser(LPCTSTR lpCmdLine) :m_Parser(lpCmdLine)
	, m_lpCmdLine(lpCmdLine)
	, m_bFileExist(TRUE)
	{

	}
	virtual ~CRunoptCmdLineParser(){}

	LPCTSTR	GetCmd()
	{
		if (m_Parser.GetCmd() == NULL )
		{
			m_bFileExist = FALSE;
			int nArgCount = 0;
			LPWSTR* lpParsedCmdLine = ::CommandLineToArgvW(m_lpCmdLine,&nArgCount); 
			if ( lpParsedCmdLine != NULL )
				return lpParsedCmdLine[0];
			else
				return NULL;
		}
		else
			return m_Parser.GetCmd();
	}

	BOOL IsPEFileExist()
	{
		return m_bFileExist;
	}

protected:
	CCmdLineParser	m_Parser;
	BOOL			m_bFileExist;
	LPCTSTR			m_lpCmdLine;
};

#endif /* _CMDLINE_PARSER_INC_ */