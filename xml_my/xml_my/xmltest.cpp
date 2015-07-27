#include "xmltest.h"
#include "rapidxml_print.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>

using namespace std;
using namespace rapidxml;

bool xmlTest::CreateXml(const string& xmlName)
{  
//	cout << __FILE__ << ", line: " << __LINE__ << endl;
	// xml declaration
	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

//	cout << __FILE__ << ", line: " << __LINE__ << endl;

	// root node
	xml_node<>* root = doc.allocate_node(node_element, "Persons");
	doc.append_node(root);
//	cout << __FILE__ << ", line: " << __LINE__ << endl;

	char buf[15];
	for (int i = 0; i < 10; i++)
	{ 
//		cout << __FILE__ << ", line: " << __LINE__ << endl;
		xml_node<>* child = doc.allocate_node(node_element, "Person");
//		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "%d", i);
//		doc.allocate_string();
		child->append_attribute(doc.allocate_attribute("ID", doc.allocate_string(buf)));
		child->append_attribute(doc.allocate_attribute("name", "Obama"));
		root->append_node(child);
//		cout << __FILE__ << ", line: " << __LINE__ << endl;

		xml_node<>* ageElement = doc.allocate_node(node_element, "age");
		ageElement->value("20");
		child->append_node(ageElement); 
//		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}
//	cout << __FILE__ << ", line: " << __LINE__ << endl;
	cout << doc << endl;

	// Save to file
	std::ofstream file_stored(xmlName);
	file_stored << doc;
	file_stored.close();
	doc.clear();

	return true;
}

bool xmlTest::ReadXml(const string& xmlName)
{
	// file to string
	std::ifstream fin(xmlName);
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string xml = ss.str();
//	cout << xml << endl;
	vector<char> xml_copy(xml.begin(), xml.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	cout << doc << endl;

	// since we have parsed the XML declaration, it is the first node
	// (otherwise the first node would be our root node)
	xml_node<> *dcl = doc.first_node();
	cout << "xml file version is " << dcl->first_attribute("version")->value() << endl;
	cout << "xml file encoding is " << dcl->first_attribute("encoding")->value() << endl;

	xml_node<> *root = doc.first_node()->next_sibling();
	assert(root != NULL);
	cout << "xml file root node is " <<root->name() << endl;

	xml_node<char> *pNode = NULL;
	string msg = "";
//	cout << __FILE__ << ", line: " << __LINE__ << endl;
	for (pNode = root->first_node(); pNode != NULL; pNode = pNode->next_sibling())
	{
//		cout << __FILE__ << ", line: " << __LINE__ << endl;
		msg += dumpNode(pNode, 0);
	}
//	cout << __FILE__ << ", line: " << __LINE__ << endl;
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


xml_node<>* xmlTest::SelectSingleNode(const string& xmlName, const string& nodeName,
	const string& nodeAttrName, const string& nodeAttrValue)
{
//	cout << __FILE__ << ", line: " << __LINE__ << endl;
	// file to string
	std::ifstream fin(xmlName);
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string xml = ss.str();
	//	cout << xml << endl;
	vector<char> xml_copy(xml.begin(), xml.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

//	cout << doc << endl;

	// since we have parsed the XML declaration, it is the first node
	// (otherwise the first node would be our root node)
	xml_node<> *root = doc.first_node()->next_sibling();
	assert(root != NULL);

	xml_node<>* pNode = NULL;
	xml_node<>* pSelectNode = NULL;
	string msg = "";
//	cout << __FILE__ << ", line: " << __LINE__ << endl;
	for (pNode = root->first_node(); pNode; pNode = pNode->next_sibling())
	{
//		cout << __FILE__ << ", line: " << __LINE__ << endl;
		pSelectNode = selectChildNode(pNode, nodeName, nodeAttrName, nodeAttrValue);
		if (pSelectNode)
		{
			break;
		}
	}

	if (pSelectNode)
	{
		cout << "find the node" << endl;
		cout << "node name is " << pSelectNode->name() 
			<< ", nodeAttrName is " << nodeAttrName
			<< ", nodeAttrValue is " << nodeAttrValue 
			<< endl;

		return pSelectNode;
	}
	else
	{
		cout << "can not find the code" << endl;
		return NULL;
	}

}

vector<xml_node<>*> xmlTest::SelectNode(const string& xmlName, const string& nodeName)
{
	vector<xml_node<>*> res;
	// file to string
	std::ifstream fin(xmlName);
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string xml = ss.str();
	//	cout << xml << endl;
	vector<char> xml_copy(xml.begin(), xml.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	//	cout << doc << endl;

	// since we have parsed the XML declaration, it is the first node
	// (otherwise the first node would be our root node)
	xml_node<> *root = doc.first_node()->next_sibling();
	assert(root != NULL);

	xml_node<>* pNode = NULL;
	xml_node<>* pSelectNode = NULL;
	string msg = "";

	for (pNode = root->first_node(); pNode; pNode = pNode->next_sibling())
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
		cout << "node is " << pSelectNode->value() << endl;
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
	// file to string
	std::ifstream fin(xmlName);
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string xml = ss.str();
	//	cout << xml << endl;
	vector<char> xml_copy(xml.begin(), xml.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	//	cout << doc << endl;

	// since we have parsed the XML declaration, it is the first node
	// (otherwise the first node would be our root node)
	xml_node<> *root = doc.first_node()->next_sibling();
	assert(root != NULL);

	xml_node<>* pNode = NULL;
	xml_node<>* pSelectNode = NULL;
	string msg = "";

	for (pNode = root->first_node(); pNode; pNode = pNode->next_sibling())
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
		cout << "node is " << pSelectNode->name() << endl;

		xml_node<>* child = doc.allocate_node(node_element, addedNodeName.c_str());
		child->value(addedContent.c_str());
		pSelectNode->append_node(child);

		// Save to file
		string newFileName = "addedNode" + xmlName;
		std::ofstream file_stored(newFileName);
		file_stored << doc;
		file_stored.close();

		cout << doc << endl;

		doc.clear();

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
	// file to string
	std::ifstream fin(xmlName);
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string xml = ss.str();
	//	cout << xml << endl;
	vector<char> xml_copy(xml.begin(), xml.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	//	cout << doc << endl;

	// since we have parsed the XML declaration, it is the first node
	// (otherwise the first node would be our root node)
	xml_node<> *root = doc.first_node()->next_sibling();
	assert(root != NULL);


	xml_node<>* pNode = NULL;
	xml_node<>* pSelectNode = NULL;
	string msg = "";

	for (pNode = root->first_node(); pNode; pNode = pNode->next_sibling())
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
		cout << "node is " << pSelectNode->name() << endl;
		
		xml_node<>* pParent = pSelectNode->parent();
		if (NULL == pParent)
		{
			return false;
		}
		pParent->remove_node(pSelectNode);

		// Save to file
		string newFileName = "removedNode" + xmlName;
		std::ofstream file_stored(newFileName);
		file_stored << doc;
		file_stored.close();

		cout << doc << endl;

		doc.clear();

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
	// file to string
	std::ifstream fin(xmlName);
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string xml = ss.str();
	//	cout << xml << endl;
	vector<char> xml_copy(xml.begin(), xml.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	//	cout << doc << endl;

	// since we have parsed the XML declaration, it is the first node
	// (otherwise the first node would be our root node)
	xml_node<> *root = doc.first_node()->next_sibling();
	assert(root != NULL);


	xml_node<>* pNode = NULL;
	xml_node<>* pSelectNode = NULL;
	string msg = "";

	for (pNode = root->first_node(); pNode; pNode = pNode->next_sibling())
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
		cout << "node is " << pSelectNode->name() << endl;

		for (rapidxml::xml_attribute<char> * attr = pNode->first_attribute();
			attr != NULL;
			attr = attr->next_attribute())
		{
			string tmpAttrVal = attr->value();
			string tmpAttrName = attr->name();
			string attrMsg = "[" + tmpAttrName + "]=" + tmpAttrVal + "\n";
			res.push_back(attrMsg);
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
	// file to string
	std::ifstream fin(xmlName);
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string xml = ss.str();
	//	cout << xml << endl;
	vector<char> xml_copy(xml.begin(), xml.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	//	cout << doc << endl;

	// since we have parsed the XML declaration, it is the first node
	// (otherwise the first node would be our root node)
	xml_node<> *root = doc.first_node()->next_sibling();
	assert(root != NULL);

	xml_node<>* pNode = NULL;
	xml_node<>* pSelectNode = NULL;
	string msg = "";

	for (pNode = root->first_node(); pNode; pNode = pNode->next_sibling())
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
		cout << "node is " << pSelectNode->name() << endl;
		pSelectNode->append_attribute(doc.allocate_attribute(addedAtrrName.c_str(), addedAttrValue.c_str()));

		// Save to file
		string newFileName = "addedAttr" + xmlName;
		std::ofstream file_stored(newFileName);
		file_stored << doc;
		file_stored.close();

		cout << doc << endl;

		doc.clear();

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
	// file to string
	std::ifstream fin(xmlName);
	std::stringstream ss;
	ss << fin.rdbuf();
	std::string xml = ss.str();
	//	cout << xml << endl;
	vector<char> xml_copy(xml.begin(), xml.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	//	cout << doc << endl;

	// since we have parsed the XML declaration, it is the first node
	// (otherwise the first node would be our root node)
	xml_node<> *root = doc.first_node()->next_sibling();
	assert(root != NULL);

	xml_node<>* pNode = NULL;
	xml_node<>* pSelectNode = NULL;
	string msg = "";

	for (pNode = root->first_node(); pNode; pNode = pNode->next_sibling())
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
		cout << "node is " << pSelectNode->name() << endl;

		pSelectNode->remove_attribute(pSelectNode->first_attribute(removedAtrrName.c_str()));

		// Save to file
		string newFileName = "removedAttr" + xmlName;
		std::ofstream file_stored(newFileName);
		file_stored << doc;
		file_stored.close();

		cout << doc << endl;

		doc.clear();

		return true;

		return true;
	}
	else
	{
		cout << "can not file the code" << endl;
		return false;
	}
}

string xmlTest::dumpNode(xml_node<>* pNode, int flag)
{
//	cout << __FILE__ << ", line: " << __LINE__ << endl;
	string msg = "";

	if (pNode == NULL)
	{
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		return "";
	}


	xml_node<>* pChildNode = NULL;
	int t = pNode->type();

	switch (t)
	{

	//case node_data:
	//	{   
	//		cout << __FILE__ << ", line: " << __LINE__ << endl;
	//		string text = pNode->value();
	//		if (!text.empty())
	//		{
	//			msg = msg + "=" + text;
	//		}
	//		break;
	//	}
  
	case node_element:
		{
//			cout << __FILE__ << ", line: " << __LINE__ << endl;
			msg = msg + "\r\n";
			int num = flag;

			while (num >= 1)
			{
				msg = msg + "  ";
				num--;
			}

			msg = msg + pNode->name();
			string tmpAttrMsg = "[";

//			cout << __FILE__ << ", line: " << __LINE__ << endl;
			for (rapidxml::xml_attribute<char> * attr = pNode->first_attribute();
				attr != NULL;
				attr = attr->next_attribute())
			{
				string tmpAttrVal = attr->value();
				string tmpAttrName = attr->name();
				tmpAttrMsg += tmpAttrName + "=" + tmpAttrVal + " ";
				
			}
//			tmpAttrMsg = tmpAttrMsg.erase(tmpAttrMsg.find_last_of(","));
			tmpAttrMsg += "]";
			string value = pNode->value();
			if (!value.empty())
			{
				tmpAttrMsg = tmpAttrMsg + " = " + value;
			}
			msg += tmpAttrMsg;

			break;
		}

	default:
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		break;
	}

	for (pChildNode = pNode->first_node(); pChildNode != 0; pChildNode = pChildNode->next_sibling())
	{
//		cout << __FILE__ << ", line: " << __LINE__ << endl;
		msg = msg + dumpNode(pChildNode, flag + 1);
	}

	return msg;
}

xml_node<>* xmlTest::selectChildNode(xml_node<>* pNode, const string& nodeName,
	const string& nodeAttrName, const string& nodeAttrValue)
{
//	cout << __FILE__ << ", line: " << __LINE__ << endl;
	if (pNode == NULL)
	{
		return NULL;
	}
	xml_node<>* pSelectedNode = NULL;
	xml_node<>* pChildNode = NULL;
	int t = pNode->type();
	switch (t)
	{
	case node_element:
//		cout << __FILE__ << ", line: " << __LINE__ << endl;
		if (pNode->name() == nodeName)
		{
//			cout << __FILE__ << ", line: " << __LINE__ << endl;
			//cout << pNode->Value() << endl;  
			if (!nodeAttrName.empty() && !nodeAttrValue.empty())
			{
//				cout << __FILE__ << ", line: " << __LINE__ << endl;
				for (rapidxml::xml_attribute<char> * attr = pNode->first_attribute();
					attr != NULL;
					attr = attr->next_attribute())
				{
					if (attr->name() == nodeAttrName && attr->value() == nodeAttrValue)
					{
//						cout << __FILE__ << ", line: " << __LINE__ << endl;
						return pNode;
					}
				}

			}
			else
			{
//				cout << __FILE__ << ", line: " << __LINE__ << endl;
				return pNode;
			}

		}
		else
		{
//			cout << __FILE__ << ", line: " << __LINE__ << endl;
			for (pChildNode = pNode->first_node();
				pChildNode != 0;
				pChildNode = pChildNode->next_sibling())
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

	default:
//		cout << __FILE__ << ", line: " << __LINE__ << endl;
		break;
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
