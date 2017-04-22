
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Data/XML/SAXParser.hpp"


#define XML_BEGIN_OBSERVER(cls)                                                                                        \
class cls : public CX::Data::XML::SAXParserObserverBase                                                                \
{

#define XML_END_OBSERVER                                                                                               \
};

#define XML_BEGIN_PARSE                                                                                                \
	virtual void OnElement(const CX::Char *__xml_path__, CX::Bool start, const AttrsVector *__xml_attrs__,              \
	                       const CX::Char *value)                                                                       \
	{                                                                                                                   \
		CX::String __cr_path__;                                                                                          \
		CX::Bool   __full_cmp__;                                                                                         \
                                                                                                                       \
		{

#define XML_END_PARSE                                                                                                  \
		}                                                                                                                \
	}

#define XML_BEGIN(tag, ...)                                                                                            \
		}                                                                                                                \
		__full_cmp__ = CX::False;                                                                                        \
		if (ComparePath(__xml_path__, AddToPath(__cr_path__, tag), &__full_cmp__))                                       \
		{                                                                                                                \
			if (__full_cmp__)                                                                                             \
			{                                                                                                             \
				__AttrValuesVector__ attrs;                                                                                \
				GetAttrValues(attrs, __xml_attrs__, __VA_ARGS__);

#define XML_END                                                                                                        \
			}                                                                                                             \
		}                                                                                                                \
		RemoveFromPath(__cr_path__);                                                                                     \
		{

namespace CX
{

namespace Data
{

namespace XML
{

class SAXParserObserverBase : public ISAXParserObserver
{
public:

	SAXParserObserverBase();

	~SAXParserObserverBase();

	const Status &GetStatus() const;

	virtual void OnBeginParse();

	virtual void OnEndParse();

	virtual void OnStartElement(const Char *szName, const AttrsVector *pVectorAttrs);

	virtual void OnEndElement(const Char *szName, const Char *szText);

protected:

	typedef Vector<const Char *>::Type   __AttrValuesVector__;

	void SetStatus(const Status &status);

	static const Char *AddToPath(String &sPath, const Char *szName);

	static const Char *RemoveFromPath(String &sPath);

	static Bool ComparePath(const Char *szXMLPath, const Char *szCrPath, Bool *pbFullCmp);

	void GetAttrValues(__AttrValuesVector__ &attrs, const AttrsVector *pVectorAttrs, const CX::Char *szArg1 = NULL, 
	                   const CX::Char *szArg2 = NULL, const CX::Char *szArg3 = NULL, const CX::Char *szArg4 = NULL, 
	                   const CX::Char *szArg5 = NULL, const CX::Char *szArg6 = NULL, const CX::Char *szArg7 = NULL, 
	                   const CX::Char *szArg8 = NULL, const CX::Char *szArg9 = NULL, const CX::Char *szArg10 = NULL, 
	                   const CX::Char *szArg11 = NULL, const CX::Char *szArg12 = NULL, const CX::Char *szArg13 = NULL, 
	                   const CX::Char *szArg14 = NULL, const CX::Char *szArg15 = NULL, const CX::Char *szArg16 = NULL);

	virtual void OnElement(const Char *__xml_path__, Bool start, const AttrsVector *__xml_attrs__, 
	                       const Char *value) = 0;

private:

	String __m_xml_path__;
	Status __m_status__;

};

}//namespace XML

}//namespace Data

}//namespace CX
