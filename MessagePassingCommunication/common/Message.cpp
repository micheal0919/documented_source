#include "Message.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <iostream>

using namespace std;
using namespace rapidxml;

Message::Message()
: m_sender_port(0),
m_receiver_port(0),
m_content_len(0)
{

}


Message::~Message()
{

}


void Message::setCmd(const std::string& cmd)
{
	m_cmd = cmd;
}

std::string Message::getCmd()
{
	return m_cmd;
}

void Message::setSenderAddress(const std::string& address)
{
	m_sender_address = address;
}

std::string Message::getSenderAddress()
{
	return m_sender_address;
}

void Message::setSenderPort(int port)
{
	m_sender_port = port;
}

int Message::getSenderPort()
{
	return m_sender_port;
}

void Message::setReceiverAddress(const std::string& address)
{
	m_receiver_address = address;
}

std::string Message::getReceiverAddress()
{
	return m_receiver_address;
}

void Message::setReiverPort(int port)
{
	m_receiver_port = port;
}

int Message::getReceiverPort()
{
	return m_receiver_port;
}

void Message::setContentLen(int len)
{
	m_content_len = len;
}

int Message::getContentLen()
{
	return m_content_len;
}

void Message::setContent(const std::string& content)
{
	m_content = content;
}

std::string Message::getContent()
{
	return m_content;
}


// serialize to a xml string
std::string Message::serialize()
{
	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	// root node
	xml_node<>* root = doc.allocate_node(node_element, "message");
	doc.append_node(root);

	// head
	xml_node<>* head = doc.allocate_node(node_element, "head");
	
	xml_node<>* cmd = doc.allocate_node(node_element, "cmd");
	cmd->value(doc.allocate_string(m_cmd.c_str()));
	head->append_node(cmd);

	xml_node<>* sender = doc.allocate_node(node_element, "sender");
	sender->append_attribute(doc.allocate_attribute("IP", doc.allocate_string(m_sender_address.c_str())));
	char buf[16];
	sprintf_s(buf, "%d", m_sender_port);
	sender->append_attribute(doc.allocate_attribute("port", doc.allocate_string(buf)));
	head->append_node(sender);

	xml_node<>* receiver = doc.allocate_node(node_element, "receiver");
	receiver->append_attribute(doc.allocate_attribute("IP", doc.allocate_string(m_receiver_address.c_str())));
	sprintf_s(buf, "%d", m_receiver_port);
	receiver->append_attribute(doc.allocate_attribute("port", doc.allocate_string(buf)));
	head->append_node(receiver);

	root->append_node(head);

	// body
	xml_node<>* body = doc.allocate_node(node_element, "body");
	sprintf_s(buf, "%d", m_content_len);
	body->append_attribute(doc.allocate_attribute("len", doc.allocate_string(buf)));

	xml_node<>* content = doc.allocate_node(node_element, "content");
	content->value(doc.allocate_string(m_content.c_str()));
	body->append_node(content);

	root->append_node(body);

	// Print to string using output iterator
	std::string s;
	print(std::back_inserter(s), doc, 0);

	doc.clear();

	return s;
}

// paser from a xml string
bool Message::parse(const std::string& xmlString)
{
	vector<char> xml_copy(xmlString.begin(), xmlString.end());
	xml_copy.push_back('\0');

	// create xml document object and parse cstring
	// character type defaults to char
	rapidxml::xml_document<> doc;
	// 0 means default parse flags
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	xml_node<>* root = doc.first_node("message");
	assert(root);

	// head
	xml_node<>* head = root->first_node("head");
	assert(head);

	xml_node<>* cmd = head->first_node("cmd");
	assert(cmd);

	m_cmd = cmd->value();
	xml_node<>* sender = head->first_node("sender");
	assert(sender);

	xml_attribute<>* attr = sender->first_attribute("IP");
	assert(attr);

	m_sender_address = attr->value();
	attr = sender->first_attribute("port");
	assert(attr);
	assert(attr->value());

	sscanf_s(attr->value(), "%d", &m_sender_port);

	xml_node<>* receiver = head->first_node("receiver");
	assert(receiver);
	attr = receiver->first_attribute("IP");
	assert(receiver);
	m_receiver_address = attr->value();
	attr = receiver->first_attribute("port");
	assert(attr);
	assert(attr->value());
	sscanf_s(attr->value(), "%d", &m_receiver_port);

	// body
	xml_node<>* body = root->first_node("body");
	assert(body);
	attr = body->first_attribute("len");
	assert(attr);
	assert(attr->value());
	sscanf_s(attr->value(), "%d", &m_content_len);
	xml_node<>* content = body->first_node("content");
	assert(content);
	m_content = content->value();
	
	return true;

}

/*
int main()
{
	Message msg;

	string cmd = "bull shit";
	string sender_addr = "127.0.0.1";
	int sender_port = 38000;
	string receiver_addr = "127.0.0.1";
	int receiver_port = 38002;

	string content = "bull shit, bull shit";

	msg.setCmd(cmd);
	msg.setSenderAddress(sender_addr);
	msg.setSenderPort(sender_port);
	msg.setReceiverAddress(receiver_addr);
	msg.setReiverPort(receiver_port);
	msg.setContentLen((int)content.size());
	msg.setContent(content);

	string xml_string = msg.serialize();

	cout << xml_string << endl;

	Message msg2;
	msg2.parse(xml_string);

	assert(msg2.getCmd() == msg.getCmd());
	assert(msg2.getSenderAddress() == msg.getSenderAddress());
	assert(msg2.getSenderPort() == msg.getSenderPort());
	assert(msg2.getReceiverAddress() == msg.getReceiverAddress());
	assert(msg2.getReceiverPort() == msg.getReceiverPort());
	assert(msg2.getContentLen() == msg.getContentLen());
	assert(msg2.getContent() == msg.getContent());

	return 0;

}
*/
