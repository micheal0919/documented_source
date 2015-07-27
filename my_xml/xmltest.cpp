#include "xmltest.h"
#include <iostream>

using namespace std;


bool xmlTest::CreateXml(const string& xmlName)
{  
	TiXmlDocument *myDocument = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	myDocument->LinkEndChild(decl);
 
	TiXmlElement *RootElement = new TiXmlElement("Persons");
	myDocument->LinkEndChild(RootElement);

	for (int i = 0; i<10; i++)
	{ 
		TiXmlElement *PersonElement = new TiXmlElement("Person");
		RootElement->LinkEndChild(PersonElement);
  
		PersonElement->SetAttribute("ID", i);
		PersonElement->SetAttribute("name", "Obama");
		TiXmlElement *AgeElement = new TiXmlElement("age");
		PersonElement->LinkEndChild(AgeElement);
  
		TiXmlText *AgeContent = new TiXmlText("20");
		AgeElement->LinkEndChild(AgeContent);

	}

	myDocument->SaveFile(xmlName);

	return true;
}

bool xmlTest::ReadXml(const string& xmlName)
{  
	TiXmlDocument *myDocument = new TiXmlDocument(xmlName);
	myDocument->LoadFile();

	TiXmlPrinter printer;
	printer.SetIndent("\t");

	myDocument->Accept(&printer);
	cout << printer.Str() << endl;

	TiXmlDeclaration* decl = myDocument->FirstChild()->ToDeclaration();
	cout << "xml version is " << decl->Version() << endl;
	cout << "xml encoding is " << decl->Encoding() << endl;

	TiXmlElement *RootElement = myDocument->RootElement();
	cout << RootElement->Value() << endl;

	TiXmlNode* pNode = NULL;
	string msg = "";

	for (pNode = RootElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		msg += dumpNode(pNode, 0);
	}

	cout << msg << endl;

	return true;
}

/*
bool xmlTest::Insert(const string& xmlName)
{
	TiXmlDocument *myDocument = new TiXmlDocument(xmlName);
	myDocument->LoadFile();
	if (myDocument == NULL)
	{
		return false;
	}

	TiXmlPrinter printer;
	printer.SetIndent("\t");
	myDocument->Accept(&printer);
	cout << printer.Str() << endl;

	TiXmlDeclaration* decl = myDocument->FirstChild()->ToDeclaration();
	cout << "xml version is " << decl->Version() << endl;
	cout << "xml encoding is " << decl->Encoding() << endl;

	TiXmlElement *RootElement = myDocument->RootElement();

	if (RootElement != NULL)
	{
		TiXmlElement *PersonElement = new TiXmlElement("Person");
		PersonElement->SetAttribute("Id", 1);
		RootElement->LinkEndChild(PersonElement);

		TiXmlText *textElement = new TiXmlText("Jam");
		PersonElement->LinkEndChild(textElement);

		TiXmlElement *TeamElement = new TiXmlElement("team");
		TeamElement->SetAttribute("TID", "001");
		RootElement->LinkEndChild(TeamElement);

		TiXmlElement *teamName = new TiXmlElement("name");
		TiXmlText *nameText = new TiXmlText("workgroup");
		teamName->LinkEndChild(nameText);
		TeamElement->LinkEndChild(teamName);

		TiXmlElement *teamType = new TiXmlElement("type");
		TiXmlText *typeText = new TiXmlText("SI");
		teamType->LinkEndChild(typeText);

		TeamElement->LinkEndChild(teamType);

		myDocument->SaveFile("Person.xml");

		TiXmlPrinter printer;
		printer.SetIndent("\t");

		myDocument->Accept(&printer);
		cout << printer.Str() << endl;

		return true;
	}
	else {
		cout << "the xml file is empty" << endl;

		TiXmlElement *RootElement = new TiXmlElement("Persons");
		myDocument->LinkEndChild(RootElement);
  
		TiXmlElement *PersonElement = new TiXmlElement("Person");
		PersonElement->SetAttribute("Id", 1);
		RootElement->LinkEndChild(PersonElement);

		TiXmlText *textElement = new TiXmlText("Jam");
		PersonElement->LinkEndChild(textElement);

		TiXmlElement *TeamElement = new TiXmlElement("team");
		TeamElement->SetAttribute("TID", "001");
		RootElement->LinkEndChild(TeamElement);
 
		TiXmlElement *teamName = new TiXmlElement("name");
		TiXmlText *nameText = new TiXmlText("workgroup");
		teamName->LinkEndChild(nameText);
		TeamElement->LinkEndChild(teamName);

		TiXmlElement *teamType = new TiXmlElement("type");
		TiXmlText *typeText = new TiXmlText("SI");
		teamType->LinkEndChild(typeText);

		TeamElement->LinkEndChild(teamType);

		myDocument->SaveFile("Person.xml");

		TiXmlPrinter printer;
		printer.SetIndent("\t");
		myDocument->Accept(&printer);
		cout << printer.Str() << endl;

		return true;
	}
	return false;
} */


TiXmlNode* xmlTest::SelectSingleNode(const string& xmlName, const string& nodeName,
	const string& nodeAttrName, const string& nodeAttrValue)
{ 
	TiXmlDocument *xmlDoc = new TiXmlDocument(xmlName);
	if (!xmlDoc->LoadFile())
	{
		return NULL;
	}
	if (xmlDoc == NULL)
	{
		return NULL;
	}

	TiXmlElement *RootElement = xmlDoc->RootElement();
	if (RootElement == NULL)
	{
		cout << "error, line: " << __LINE__ << endl;
		return NULL;
	}

	TiXmlNode * pNode = NULL;
	TiXmlNode * pSelectNode = NULL;
	string msg = "";

	for (pNode = RootElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{

		pSelectNode = selectChildNode(pNode, nodeName, nodeAttrName, nodeAttrValue);
		if (pSelectNode)
		{
			break;
		}
	}

	if (pSelectNode)
	{
		cout << "find the node" << endl;
		cout << "node is " << pSelectNode->Value() << endl;
		return pSelectNode;
	}
	else
	{
		cout << "can not file the code" << endl;
		return NULL;
	}

}

vector<TiXmlNode*> xmlTest::SelectNode(const string& xmlName, const string& nodeName)
{
	vector<TiXmlNode*> res;
	TiXmlDocument *xmlDoc = new TiXmlDocument(xmlName);
	if (!xmlDoc->LoadFile())
	{
		return res;
	}
	if (xmlDoc == NULL)
	{
		return res;
	}

	TiXmlElement *RootElement = xmlDoc->RootElement();
	if (RootElement == NULL)
	{
		cout << "error, line: " << __LINE__ << endl;
		return res;
	}

	TiXmlNode * pNode = NULL;
	TiXmlNode * pSelectNode = NULL;
	string msg = "";

	for (pNode = RootElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		pSelectNode = selectChildNode(pNode, nodeName, "", "");
		if (pSelectNode)
		{
			res.push_back(pSelectNode);
		}
	}

	if (!res.empty())
	{
		cout << "find the node" << endl;
		cout << "node is " << pSelectNode->Value() << endl;
		cout << "node number is " << res.size() << endl;
	}
	else
	{
		cout << "can not file the code" << endl;
	}
	return res;
}

bool xmlTest::InsertNode(const std::string& xmlName, const std::string& nodeName,
	const std::string& nodeAttrName, const std::string& nodeAttrValue,
	const std::string& addedNodeName, const std::string& addedContent)
{
	TiXmlDocument *xmlDoc = new TiXmlDocument(xmlName);
	if (!xmlDoc->LoadFile())
	{
		return false;
	}
	if (xmlDoc == NULL)
	{
		return false;
	}

	TiXmlElement *RootElement = xmlDoc->RootElement();
	if (RootElement == NULL)
	{
		cout << "error, line: " << __LINE__ << endl;
		return false;
	}

	TiXmlNode * pNode = NULL;
	TiXmlNode * pSelectNode = NULL;
	string msg = "";

	for (pNode = RootElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{

		pSelectNode = selectChildNode(pNode, nodeName, nodeAttrName, nodeAttrValue);
		if (pSelectNode)
		{
			break;
		}
	}

	if (pSelectNode)
	{
		cout << "find the node, line:" << __LINE__ << endl;
		cout << "node is " << pSelectNode->Value() << endl;

		TiXmlElement* newNode = new TiXmlElement(addedNodeName);
		newNode->LinkEndChild(new TiXmlText(addedContent));
		pSelectNode->LinkEndChild(newNode);

		string newFileName = "addedNode" + xmlName;
		xmlDoc->SaveFile(newFileName);

		TiXmlPrinter printer;
		printer.SetIndent("\t");
		xmlDoc->Accept(&printer);
		cout << printer.Str() << endl;

		return true;
	}
	else
	{
		cout << "can not file the code" << endl;
		return false;
	}
}

bool xmlTest::RemoveNode(const std::string& xmlName, const std::string& nodeName,
	const std::string& nodeAttrName, const std::string& nodeAttrValue)
{
	TiXmlDocument *xmlDoc = new TiXmlDocument(xmlName);
	if (!xmlDoc->LoadFile())
	{
		return false;
	}
	if (xmlDoc == NULL)
	{
		return false;
	}

	TiXmlElement *RootElement = xmlDoc->RootElement();
	if (RootElement == NULL)
	{
		cout << "error, line: " << __LINE__ << endl;
		return false;
	}

	TiXmlNode * pNode = NULL;
	TiXmlNode * pSelectNode = NULL;
	string msg = "";

	for (pNode = RootElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{

		pSelectNode = selectChildNode(pNode, nodeName, nodeAttrName, nodeAttrValue);
		if (pSelectNode)
		{
			break;
		}
	}

	if (pSelectNode)
	{
		cout << "find the node, line:" << __LINE__ << endl;
		cout << "node is " << pSelectNode->Value() << endl;
		
		TiXmlNode* pParent = pSelectNode->Parent();
		if (NULL == pParent)
		{
			return false;
		}
		pParent->RemoveChild(pSelectNode);

		string newFileName = "removedNode" + xmlName;
		xmlDoc->SaveFile(newFileName);

		TiXmlPrinter printer;
		printer.SetIndent("\t");
		xmlDoc->Accept(&printer);
		cout << printer.Str() << endl;

		return true;
	}
	else
	{
		cout << "can not file the code" << endl;
		return false;
	}
}

std::vector<std::string> xmlTest::getNodeAttributes(const std::string& xmlName, const std::string& nodeName,
	const std::string& nodeAttrName, const std::string& nodeAttrValue)
{
	vector<string> res;
	TiXmlDocument *xmlDoc = new TiXmlDocument(xmlName);
	if (!xmlDoc->LoadFile())
	{
		return res;
	}
	if (xmlDoc == NULL)
	{
		return res;
	}

	TiXmlElement *RootElement = xmlDoc->RootElement();
	if (RootElement == NULL)
	{
		cout << "error, line: " << __LINE__ << endl;
		return res;
	}

	TiXmlNode * pNode = NULL;
	TiXmlNode * pSelectNode = NULL;
	string msg = "";

	for (pNode = RootElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{

		pSelectNode = selectChildNode(pNode, nodeName, nodeAttrName, nodeAttrValue);
		if (pSelectNode)
		{
			break;
		}
	}

	if (pSelectNode)
	{
		cout << "find the node, line:" << __LINE__ << endl;
		cout << "node is " << pSelectNode->Value() << endl;

		TiXmlElement * pElement = pSelectNode->ToElement();
		TiXmlAttribute * pAttr = pElement->FirstAttribute();
		TiXmlAttribute * pNextAttr = NULL;
		if (pAttr != NULL)
		{
			string tmpAttrVal = "";
			string tmpAttrName = "";

			do
			{
				tmpAttrVal = pAttr->Value();
				tmpAttrName = pAttr->Name();
				string attrMsg = "[" + tmpAttrName + "]=" + tmpAttrVal + "\n";
				res.push_back(attrMsg);
			} while (pAttr = pAttr->Next());
		}
		
		cout << "line: " << __LINE__ << endl;
		cout << "the attribute name vale paires are: " << endl;
		for (size_t i = 0; i < res.size(); i++)
		{
			cout << res[i] << endl;
		}

		return res;
	}
	else
	{
		cout << "can not file the code" << endl;
		return res;
	}
}

bool xmlTest::addAttribute(const std::string& xmlName, const std::string& nodeName,
	const std::string& nodeAttrName, const std::string& nodeAttrValue,
	const std::string& addedAtrrName, const std::string& addedAttrValue)
{
	TiXmlDocument *xmlDoc = new TiXmlDocument(xmlName);
	if (!xmlDoc->LoadFile())
	{
		return false;
	}
	if (xmlDoc == NULL)
	{
		return false;
	}

	TiXmlElement *RootElement = xmlDoc->RootElement();
	if (RootElement == NULL)
	{
		cout << "error, line: " << __LINE__ << endl;
		return false;
	}

	TiXmlNode * pNode = NULL;
	TiXmlNode * pSelectNode = NULL;
	string msg = "";

	for (pNode = RootElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{

		pSelectNode = selectChildNode(pNode, nodeName, nodeAttrName, nodeAttrValue);
		if (pSelectNode)
		{
			break;
		}
	}

	if (pSelectNode)
	{
		cout << "find the node, line:" << __LINE__ << endl;
		cout << "node is " << pSelectNode->Value() << endl;

		TiXmlElement* pElement = pSelectNode->ToElement();
		pElement->SetAttribute(addedAtrrName, addedAttrValue);

		string newFileName = "addedAttr" + xmlName;
		xmlDoc->SaveFile(newFileName);

		TiXmlPrinter printer;
		printer.SetIndent("\t");
		xmlDoc->Accept(&printer);
		cout << printer.Str() << endl;

		return true;
	}
	else
	{
		cout << "can not file the code" << endl;
		return false;
	}
}

bool xmlTest::removeAttribute(const std::string& xmlName, const std::string& nodeName,
	const std::string& nodeAttrName, const std::string& nodeAttrValue,
	const std::string& removedAtrrName)
{
	TiXmlDocument *xmlDoc = new TiXmlDocument(xmlName);
	if (!xmlDoc->LoadFile())
	{
		return false;
	}
	if (xmlDoc == NULL)
	{
		return false;
	}

	TiXmlElement *RootElement = xmlDoc->RootElement();
	if (RootElement == NULL)
	{
		cout << "error, line: " << __LINE__ << endl;
		return false;
	}

	TiXmlNode * pNode = NULL;
	TiXmlNode * pSelectNode = NULL;
	string msg = "";

	for (pNode = RootElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{

		pSelectNode = selectChildNode(pNode, nodeName, nodeAttrName, nodeAttrValue);
		if (pSelectNode)
		{
			break;
		}
	}

	if (pSelectNode)
	{
		cout << "find the node, line:" << __LINE__ << endl;
		cout << "node is " << pSelectNode->Value() << endl;

		TiXmlElement* pElement = pSelectNode->ToElement();
		pElement->RemoveAttribute(removedAtrrName);

		string newFileName = "removedAttr" + xmlName;
		xmlDoc->SaveFile(newFileName);

		TiXmlPrinter printer;
		printer.SetIndent("\t");
		xmlDoc->Accept(&printer);
		cout << printer.Str() << endl;

		return true;
	}
	else
	{
		cout << "can not file the code" << endl;
		return false;
	}
}

string xmlTest::dumpNode(TiXmlNode * pNode, int flag)
{
	string msg = "";

	if (pNode == NULL)
	{
		return "";
	}

	TiXmlText * pText = NULL;
	TiXmlNode * pChildNode = NULL;
	int t = pNode->Type();

	switch (t)
	{

		case TiXmlText::TINYXML_TEXT:
		{   
			pText = pNode->ToText();
			string text = pText->Value();
			if (!text.empty())
			{
				msg = msg + "=" + text;
			}
			break;
		}
  
		case TiXmlText::TINYXML_ELEMENT:
		{
			msg = msg + "\r\n";
			int num = flag;

			while (num >= 1)
			{
				msg = msg + "  ";
				num--;
			}

			msg = msg + pNode->Value();

			TiXmlElement * pElement = pNode->ToElement();

			TiXmlAttribute * pAttr = pElement->FirstAttribute();
			TiXmlAttribute * pNextAttr = NULL;
			string tmpAttrMsg = "[";

			//if(pAttr != NULL && !(string(pAttr->Name())).compare("name") && !(string(pAttr->Value())).compare("IDH_CFG_USB"))  
			if (pAttr != NULL)
			{
				string tmpAttrVal = "";
				string tmpAttrName = "";

				do
				{
					tmpAttrVal = pAttr->Value();
					tmpAttrName = pAttr->Name();
					tmpAttrMsg += tmpAttrName + "=" + tmpAttrVal + ","; 
				} while (pAttr = pAttr->Next());

				tmpAttrMsg = tmpAttrMsg.erase(tmpAttrMsg.find_last_of(","));

			}
			tmpAttrMsg += "]";
			msg += tmpAttrMsg;

			break;
		}

		case TiXmlText::TINYXML_DOCUMENT:
		case TiXmlText::TINYXML_COMMENT:
		case TiXmlText::TINYXML_UNKNOWN:
		case TiXmlText::TINYXML_DECLARATION:
		case TiXmlText::TINYXML_TYPECOUNT:
		{
			//Nothing to do  
		}
	}

	for (pChildNode = pNode->FirstChild(); pChildNode != 0; pChildNode = pChildNode->NextSibling())
	{
		msg = msg + dumpNode(pChildNode, flag + 1);
	}

	return msg;
}

TiXmlNode* xmlTest::selectChildNode(TiXmlNode * pNode, const string& nodeName, 
	const string& nodeAttrName, const string& nodeAttrValue)
{
	if (pNode == NULL)
	{
		return NULL;
	}
	TiXmlNode * pSelectedNode = NULL;
	TiXmlNode * pChildNode = NULL;
	int t = pNode->Type();
	switch (t)
	{
	case TiXmlText::TINYXML_DOCUMENT:
	case TiXmlText::TINYXML_DECLARATION:
	case TiXmlText::TINYXML_TEXT:
	case TiXmlText::TINYXML_UNKNOWN:
	case TiXmlText::TINYXML_COMMENT:
		break;
	case TiXmlText::TINYXML_ELEMENT:
		if (pNode->Value() == nodeName)
		{
			//cout << pNode->Value() << endl;  
			if (!nodeAttrName.empty() && !nodeAttrValue.empty())
			{
				TiXmlElement * pElement = pNode->ToElement();

				TiXmlAttribute * pAttr = pElement->FirstAttribute();
				TiXmlAttribute * pNextAttr = NULL;
				if (pAttr != NULL)
				{
					do
					{
						if (pAttr->Name() == nodeAttrName&&pAttr->Value() == nodeAttrValue)
						{
							//cout << pAttr->Value() << endl;  
							return pNode;
						}
					} while (pAttr = pAttr->Next());
				}
			}
			else
			{
				return pNode;
			}

		}
		else
		{
			for (pChildNode = pNode->FirstChild();
				pChildNode != 0;
				pChildNode = pChildNode->NextSibling())
			{
				pSelectedNode = selectChildNode(
					pChildNode,
					nodeName,
					nodeAttrName,
					nodeAttrValue);
				if (pSelectedNode)
				{
					return pSelectedNode;
				}
			}
		}

	default:break;
	}
	return NULL;
}

/*
void xmlTest::makeDemo()
{
	for (int i = 0; i < 100; i++) {
		contact c;
		char buf[8];
		sprintf(buf, "%04d", i);
		c.name = buf;
		c.id = i;
		c.phoneNo = buf;
		c.email = buf;
		m_contacts.addContact(c);
	}

}

bool xmlTest::loadFile(const string& fileName)
{

	m_doc.LoadFile(fileName);
	if (m_doc.Error()) {
		cout << "Line: " << __LINE__ << ", error" << endl;
		return false;
	}

	TiXmlElement* root = m_doc.RootElement();

	return true;
}

bool xmlTest::wirteToFile(const string& fileName)
{
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	m_doc.LinkEndChild(decl);
	
	TiXmlElement* root = new TiXmlElement("contacts");
	m_doc.LinkEndChild(root);

	TiXmlComment* comment = new TiXmlComment();
	string s = "settings for contacts";
	comment->SetValue(s);
	root->LinkEndChild(comment);

	// block: put all contacts info into root element
	{
		list<contact> all_con = m_contacts.getAllContact();
		list<contact>::iterator it = all_con.begin();
		for (; it != all_con.end(); ++it) {
			TiXmlElement* contactElement = new TiXmlElement("contact");
			contactElement->SetAttribute("id", it->id);
			
			TiXmlElement* nameElement = new TiXmlElement("name");
			nameElement->LinkEndChild(new TiXmlText(it->name));
			contactElement->LinkEndChild(nameElement);

			TiXmlElement* phoneElement = new TiXmlElement("phone");
			phoneElement->LinkEndChild(new TiXmlText(it->phoneNo));
			contactElement->LinkEndChild(phoneElement);

			TiXmlElement* emailElement = new TiXmlElement("email");
			emailElement->LinkEndChild(new TiXmlText(it->email));
			contactElement->LinkEndChild(emailElement);

			root->LinkEndChild(contactElement);
		}

	}

	bool res = m_doc.SaveFile(fileName);
	if (!res) {
		cout << "Line: " << __LINE__ << ", error" << endl;
	}

	return res;
}

string xmlTest::getString()
{
	return string();
}

bool xmlTest::loadString(const string& xmlString)
{
	return true;
}

void xmlTest::printParseTree()
{

}
*/
