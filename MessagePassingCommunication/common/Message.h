#pragma once
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>

class Message
{
public:
	Message();
	~Message();

	void setCmd(const std::string& cmd);
	std::string getCmd();

	void setSenderAddress(const std::string& address);
	std::string getSenderAddress();

	void setSenderPort(int port);
	int getSenderPort();

	void setReceiverAddress(const std::string& address);
	std::string getReceiverAddress();

	void setReiverPort(int port);
	int getReceiverPort();

	void setContentLen(int len);
	int getContentLen();

	void setContent(const std::string& content);
	std::string getContent();

	// serialize to a xml string
	std::string serialize();

	// paser from a xml string
	bool parse(const std::string& xmlString);

private:
	// head
	std::string m_cmd;
	std::string m_sender_address;
	int m_sender_port;
	std::string m_receiver_address;
	int m_receiver_port;

	// body
	int m_content_len;
	std::string m_content;

	// string mxl
	std::string m_xml_string;
};

#endif // __MESSAGE_H__
