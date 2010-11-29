// Markup.cpp: implementation of the CMarkup class.
//

#include "stdafx.h"
#include "afxconv.h"
#include "Markup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CMarkup::CMarkup()
{
	m_pDoc = new TiXmlDocument();
	TiXmlDeclaration *pDecl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	m_pDoc->LinkEndChild( pDecl );
	m_pCurrentNode = NULL;
}

CMarkup::~CMarkup()
{
}

TiXmlElement *CMarkup::AddElem( LPCTSTR szName, LPCTSTR szValue)
{
	ASSERT( m_pDoc);
	ASSERT( szName);
	USES_CONVERSION;
	try
	{
		// Create XML element
		TiXmlElement *pXmlElement = new TiXmlElement( T2CA( szName));
		if (pXmlElement == NULL)
			return NULL;
		// Link it as a child of current working element
		if (m_pCurrentNode == NULL)
			// No current node, so use document as root node
			m_pDoc->LinkEndChild( pXmlElement);
		else
			m_pCurrentNode->LinkEndChild( pXmlElement);
		if ((szValue != NULL) && (_tcslen( szValue) != 0))
		{
			// There is text to set between <Element></Element>
			TiXmlText *pXmlText = new TiXmlText( T2CA( szValue));
			if (pXmlText == NULL)
			{
				delete pXmlElement;
				return NULL;
			}
			pXmlElement->LinkEndChild( pXmlText);
		}
		// Set new child element as working node
		m_pCurrentNode = pXmlElement;
		return pXmlElement;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

TiXmlElement* CMarkup::AddElem( LPCTSTR szName, LONG lValue)
{
	CString csValue;

	csValue.Format( _T( "%ld"), lValue);
	return AddElem( szName, csValue);
}

TiXmlElement *CMarkup::AddChildElem( LPCTSTR szName, LPCTSTR szValue)
{
	ASSERT( m_pDoc);
	ASSERT( szName);
	USES_CONVERSION;
	try
	{
		// Create XML element
		TiXmlElement *pXmlElement = new TiXmlElement( T2CA( szName));
		if (pXmlElement == NULL)
			return NULL;
		// Link it as a child of current working element
		if (m_pCurrentNode == NULL)
			// No current node, so use document as root node
			m_pDoc->LinkEndChild( pXmlElement);
		else
			m_pCurrentNode->LinkEndChild( pXmlElement);
		if ((szValue != NULL) && (_tcslen( szValue) != 0))
		{
			// There is text to set between <Element></Element>
			TiXmlText *pXmlText = new TiXmlText( T2CA( szValue));
			if (pXmlText == NULL)
			{
				delete pXmlElement;
				return NULL;
			}
			pXmlElement->LinkEndChild( pXmlText);
		}
		// Do not change current working node
		return pXmlElement;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

TiXmlElement *CMarkup::AddChildElem( LPCTSTR szName, LONG lValue)
{
	CString csValue;

	csValue.Format( _T( "%ld"), lValue);
	return AddChildElem( szName, csValue);
}

BOOL CMarkup::SetAttrib( LPCTSTR szName, LPCTSTR szValue)
{
	ASSERT( szName);
	ASSERT( szValue);
	if (m_pCurrentNode == NULL)
		return FALSE;

	USES_CONVERSION;
	try
	{
		m_pCurrentNode->SetAttribute( T2CA( szName), T2CA(  szValue));
		return TRUE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
}

BOOL CMarkup::SetAttrib( LPCTSTR szName, LONG lValue)
{
	CString csValue;

	csValue.Format( _T( "%ld"), lValue);
	return SetAttrib( szName, csValue);
}

TiXmlElement *CMarkup::IntoElem()
{
	ASSERT( m_pDoc);
	try
	{
		TiXmlElement *pXmlNode;
		if (m_pCurrentNode == NULL)
			pXmlNode = m_pDoc->FirstChildElement();
		else
			pXmlNode = m_pCurrentNode->FirstChildElement();
		if (pXmlNode)
		{
			// There is a child node, set it as working node
			m_pCurrentNode = pXmlNode;
			return pXmlNode;
		}
		// No child node
		return NULL;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

TiXmlElement *CMarkup::OutOfElem()
{
	if (m_pCurrentNode == NULL)
		return NULL;
	try
	{
		if (m_pCurrentNode->Parent() == m_pDoc)
		{
			// root node is document node
			m_pCurrentNode = NULL;
			return NULL;
		}
		TiXmlElement *pXmlNode = (TiXmlElement *)m_pCurrentNode->Parent();
		if (pXmlNode)
		{
			// There is a parent node
			m_pCurrentNode = pXmlNode;
			return pXmlNode;
		}
		// No parent node
		return NULL;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

TiXmlElement *CMarkup::FindFirstElem( LPCTSTR szName, TiXmlElement *pXmlNode)
{
	ASSERT( m_pDoc);
 	ASSERT( szName);
	USES_CONVERSION;
	try
	{
		// Initialize value of search node (doc or current node or provided node)
		TiXmlNode *pSearchNode;
		TiXmlElement *pXmlElement;
		if ((m_pCurrentNode == NULL) && (pXmlNode == NULL))
			pSearchNode = m_pDoc;
		else
		{
			if (pXmlNode != NULL)
				pSearchNode = pXmlNode;
			else
				pSearchNode = m_pCurrentNode;
		}
		// Find first child element
		pXmlElement = pSearchNode->FirstChildElement( T2CA( szName));
		if (pXmlElement)
		{
			// One sibling child node found
			m_pCurrentNode = pXmlElement;
			return pXmlElement;
		}
		// No sibling child found
		return NULL;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

TiXmlElement *CMarkup::FindNextElem( LPCTSTR szName, TiXmlElement *pXmlNode)
{
	ASSERT( m_pDoc);
 	ASSERT( szName);
	USES_CONVERSION;
	try
	{
		// Initialize value of search node (doc or current node or provided node)
		TiXmlNode *pSearchNode;
		TiXmlElement *pXmlElement;
		if ((m_pCurrentNode == NULL) && (pXmlNode == NULL))
			pSearchNode = m_pDoc;
		else
		{
			if (pXmlNode != NULL)
				pSearchNode = pXmlNode;
			else
				pSearchNode = m_pCurrentNode;
		}
		// We are continuing a previous search at same XML tree level
		pXmlElement = pSearchNode->NextSiblingElement( T2CA( szName));
		if (pXmlElement)
		{
			// Next sibling found
			m_pCurrentNode = pXmlElement;
			return pXmlElement;
		}
		// No more element found
		return NULL;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

TiXmlElement *CMarkup::ResetPos( TiXmlElement *pXmlNode)
{
	if (pXmlNode)
	{
		// Set provided node as new working node
		m_pCurrentNode = pXmlNode;
		return pXmlNode;
	}
	// Set current working node to document node
	m_pCurrentNode = NULL;
	return NULL;
}


LPCTSTR CMarkup::GetData( TiXmlElement *pXmlNode)
{
	static CString csResult;
 
	if ((pXmlNode == NULL) && (m_pCurrentNode == NULL))
		return NULL;
    USES_CONVERSION;

	try
	{
		TiXmlElement *pNode;

		if (pXmlNode == NULL)
			pNode = m_pCurrentNode;
		else
			pNode = pXmlNode;
		const char *szValue = pNode->GetText();
		if (szValue)
		{
			csResult.Format( _T( "%s"), CA2T( szValue));
			return csResult;
		}
		return NULL;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

LPCTSTR CMarkup::GetAttrib( LPCTSTR szAttrib)
{
	static CString csResult;

	if (m_pCurrentNode == NULL)
		return NULL;
    USES_CONVERSION;

	try
	{
		const char *szValue = m_pCurrentNode->Attribute( T2CA( szAttrib));
		if (szValue)
		{
			csResult.Format( _T( "%s"), CA2T( szValue));
			return csResult;
		}
		return NULL;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

LPCTSTR CMarkup::GetDoc()
{
	try
	{
		TiXmlPrinter myPrinter;
		static CString csDoc;

		myPrinter.SetIndent( "    " );
		m_pDoc->Accept( &myPrinter );
		csDoc = myPrinter.CStr();
		return csDoc;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return NULL;
	}
}

// Get XML document in TinyXML object
TiXmlDocument *CMarkup::GetTiXmlDocument()
{
	return m_pDoc;
}

// Set XML document from text string
BOOL CMarkup::SetDoc( LPCTSTR szDoc)
{
    USES_CONVERSION;
	try
	{
		TiXmlDocument *pDoc = new TiXmlDocument();

		if (pDoc)
		{
			pDoc->Parse( T2A( szDoc));
			if (m_pDoc)
				delete m_pDoc;
			m_pDoc = pDoc;
			m_pCurrentNode = NULL;
			return TRUE;
		}
		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
}

// Set XML document from TinyXML object
BOOL CMarkup::SetTinyXmlDocument( TiXmlDocument *pDoc)
{
	if (pDoc == NULL)
		return FALSE;
	if (m_pDoc)
		delete m_pDoc;
	m_pDoc = pDoc;
	m_pCurrentNode = NULL;
	return TRUE;
}

BOOL CMarkup::IsWellFormed()
{
	return TRUE;
}
