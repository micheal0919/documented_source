#ifndef __XML_TEST__
#define __XML_TEST__

#include "rapidxml.hpp"
#include <string>
#include <list>
#include <vector>

class contact
{
public:
	std::string name;
	int id;
	std::string phoneNo;
	std::string email;
};

class contactManager
{
public:
	void addContact(const contact& c)
	{
		m_contacts.push_back(c);
	}

	std::list<contact> getAllContact()
	{
		return m_contacts;
	}

private:
	std::list<contact> m_contacts;
};

class xmlTest
{
public:
	xmlTest() {}
	~xmlTest() {}

	bool CreateXml(const std::string& xmlName);
	bool ReadXml(const std::string& xmlName);
	rapidxml::xml_node<>* SelectSingleNode(const std::string& xmlName, const std::string& nodeName,
		const std::string& nodeAttrName, const std::string& nodeAttrValue);
	std::vector<rapidxml::xml_node<>*> SelectNode(const std::string& xmlName, const std::string& nodeName);
	bool InsertNode(const std::string& xmlName, const std::string& nodeName,
		const std::string& nodeAttrName, const std::string& nodeAttrValue,
		const std::string& addedNodeName, const std::string& addedContent);
	bool RemoveNode(const std::string& xmlName, const std::string& nodeName,
		const std::string& nodeAttrName, const std::string& nodeAttrValue);
	std::vector<std::string> getNodeAttributes(const std::string& xmlName, const std::string& nodeName,
		const std::string& nodeAttrName, const std::string& nodeAttrValue);
	bool addAttribute(const std::string& xmlName, const std::string& nodeName,
		const std::string& nodeAttrName, const std::string& nodeAttrValue, 
		const std::string& addedAtrrName, const std::string& addedAttrValue);
	bool removeAttribute(const std::string& xmlName, const std::string& nodeName,
		const std::string& nodeAttrName, const std::string& nodeAttrValue,
		const std::string& removedAtrrName);

private:
	std::string dumpNode(rapidxml::xml_node<>* pNode, int flag);

	rapidxml::xml_node<>* selectChildNode(rapidxml::xml_node<>* pNode, const std::string& nodeName,
		const std::string& nodeAttrName, const std::string& nodeAttrValue);
};

#endif // __XML_TEST__
