#include "Application.h"
#include "Order.h"
#include <quickfix/Session.h>
#include <quickfix/config.h>
#include <quickfix/fix50sp2/ExecutionReport.h>
#include <quickfix/MessageCracker.h>

FixServer::~FixServer() {}
void FixServer::onCreate(const FIX::SessionID&) {
	std::cout<< "this is the first threshold" << std::endl;
}
void FixServer::onLogon(const FIX::SessionID& sessionID) {
	std::cout << "This is the second threshold: " << sessionID << std::endl;
}
void FixServer::onLogout(const FIX::SessionID& sessionID) {
	std::cout << "This is the third threshold: " << sessionID << std::endl;
}
void FixServer::fromAdmin(const FIX::Message&, const FIX::SessionID&) {
	std::cout<<"THis is a test to see if the function from admin is run" << std::endl;
}
void FixServer::toAdmin(FIX::Message& message, const FIX::SessionID&) {
	std::cout<< message << std::endl;
}
void FixServer::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) {
	std::cout << "Message received: " << message << std::endl;
}
void FixServer::toApp(FIX::Message& message, const FIX::SessionID& sessionID) {
	std::cout<<"THis is a test to see if the function toapp is run" << std::endl;
	FIX::PossDupFlag possDupFlag(false);
	message.setField(possDupFlag);
	std::cout << "Message sent: " << message << std::endl;
}


