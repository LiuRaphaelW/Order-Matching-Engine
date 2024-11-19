#include <quickfix/Application.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/FileStore.h>
#include <quickfix/FileLog.h>
#include <quickfix/SocketInitiator.h>
#include "Application.h"
#include <quickfix/fix42/OrderCancelRequest.h>
#include <quickfix/fix42/OrderCancelReplaceRequest.h>
#include <quickfix/fix42/MarketDataRequest.h>

FixClient::~FixClient(){}

void FixClient::onCreate(const FIX::SessionID&) {
	std::cout<<"phase 1"<<std::endl;
}
void FixClient::onLogon(const FIX::SessionID& sessionID) {
	std::cout << "Client logged on: " << sessionID << std::endl;
}
void FixClient::onLogout(const FIX::SessionID& sessionID) {
	std::cout << "Client logged out:  EEEEEEE " << sessionID << std::endl;
}
void FixClient::fromAdmin(const FIX::Message&, const FIX::SessionID&) {
	std::cout<<"THis is a test to see if the function from admin is run" << std::endl;
}
void FixClient::toAdmin(FIX::Message&, const FIX::SessionID&) {
	std::cout<<"THis is a test to see if the function to admin is run" << std::endl;
}
void FixClient::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) {
	std::cout << "Message received: " << message << std::endl;
}
void FixClient::toApp(FIX::Message& message, const FIX::SessionID& sessionID) {
	std::cout<<"THis is a test to see if the function toApp is run" << std::endl;
	FIX::PossDupFlag possDupFlag(false);
	message.setField(possDupFlag);
	std::cout << "Message sent: " << message << std::endl;
}
void FixClient::run() {
	while(true) {
		try{
			char action;
			FixClient::printActions();
			std::cin >> action;
			switch (action) {
			case'1': case'2': case'3': case'4': case'5': break;
			default: throw std::exception();
			}
			if(action == '1') {
				FixClient::takeOrder();
			}
			else if (action == '2') {
				FixClient::cancelOrder();
			}
			else if (action == '3') {
				FixClient::replaceOrder();
			}
			else if (action == '4') {
				FixClient::marketDataRequest();
			}
			else {
				break;
			}
		}
		catch (std::exception & e) {
			std::cout<< "Unable to send message: "<< e.what() << std::endl;
		}
	}
}
void FixClient::printActions() {
	std::cout << std::endl
	<< "1) Enter Order" << std::endl
	<< "2) Cancel Order" << std::endl
	<< "3) Replace Order" << std::endl
	<< "4) Market data Test" << std::endl
	<< "5) Quit" << std::endl
	<< "Action? :";
}

void FixClient::takeOrder() {
	std::cout << "\nNewOrderSingle\n";
	FIX::Message order;
	order = NewOrderSingle();
	if ( confirmation( "Send order?" ) ) {
		FIX::Session::sendToTarget( order );
	}
	else {
		std::cout << "Order Not Sent" << std::endl;
	}
}

void FixClient::cancelOrder() {
	std::cout << "\nOrderCancelRequest\n";
	FIX::Message cancel;
	cancel = OrderCancelRequest();
	if( confirmation ("Cancel order?") ) {
		FIX::Session::sendToTarget( cancel );
	}
}

void FixClient::replaceOrder() {
	std::cout << "\nCancelReplaceRequest\n";
	FIX::Message replace;
	replace = CancelReplaceRequest();
	if( confirmation ( "Replace Order?" ) ) {
		FIX::Session::sendToTarget (replace) ;
	}
}

void FixClient::marketDataRequest() {
	std::cout << "\nMarketDataRequest\n";
	FIX::Message md;
	md = MarketDataRequest();
	FIX::Session::sendToTarget( md );

}

bool FixClient::confirmation(std::string confirmationMessage) {
	std::cout<< confirmationMessage << " (Y for yes): " << std::endl;
	char response;
	std::cin >> response;
	return response == 'Y' || response == 'y';
}

void FixClient::getHeader(FIX::Header& header) {
	header.setField( askSenderCompID() );
	header.setField( askTargetCompID() );

	if ( confirmation( "Use a TargetSubID?" ) ) {
		header.setField( askTargetSubID() );
	}
}

std::string FixClient::ask(std::string question) {
	std::cout << question << std::endl;
	std::string response;
	std::cin >> response;
	while(!confirmation ( "Is this right :" + response)) {
		std::cin >> response;
	}
	return response;
}
FIX42::NewOrderSingle FixClient::NewOrderSingle() {
	FIX::OrdType ordType;
	FIX42::NewOrderSingle newOrderSingle(
		askClOrdID(), FIX::HandlInst('1'), askSymbol(), askSide(),
		FIX::TransactTime(), ordType = askOrdType() );

	newOrderSingle.set( askOrderQty() );
	newOrderSingle.set( askTimeInForce() );
	if ( ordType == FIX::OrdType_LIMIT || ordType == FIX::OrdType_STOP_LIMIT )
		newOrderSingle.set( askPrice() );
	if ( ordType == FIX::OrdType_STOP || ordType == FIX::OrdType_STOP_LIMIT )
		newOrderSingle.set( askStopPx() );

	getHeader( newOrderSingle.getHeader() );
	return newOrderSingle;

}


FIX42::OrderCancelRequest FixClient::OrderCancelRequest() {
	FIX42::OrderCancelRequest orderCancelRequest( askOrigClOrdID(),
	      askClOrdID(), askSymbol(), askSide(), FIX::TransactTime() );

	orderCancelRequest.set(askOrderQty());
	getHeader( orderCancelRequest.getHeader() );
	return orderCancelRequest;
}

FIX42::OrderCancelReplaceRequest FixClient::CancelReplaceRequest() {
	FIX42::OrderCancelReplaceRequest cancelReplaceRequest(
		askOrigClOrdID(), askClOrdID(),
		FIX::HandlInst('1'), askSymbol(),
		askSide(), FIX::TransactTime(), askOrdType() );

	if ( confirmation( "New price" ) )
		cancelReplaceRequest.set( askPrice() );
	if ( confirmation( "New quantity" ) )
		cancelReplaceRequest.set( askOrderQty() );

	  getHeader( cancelReplaceRequest.getHeader() );
	  return cancelReplaceRequest;
}

FIX42::MarketDataRequest FixClient::MarketDataRequest() {
	  FIX::MDReqID mdReqID( "MARKETDATAID" );
	  FIX::SubscriptionRequestType subType( FIX::SubscriptionRequestType_SNAPSHOT );
	  FIX::MarketDepth marketDepth( 0 );

	  FIX42::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;
	  FIX::MDEntryType mdEntryType( FIX::MDEntryType_BID );
	  marketDataEntryGroup.set( mdEntryType );

	  FIX42::MarketDataRequest::NoRelatedSym symbolGroup;
	  FIX::Symbol symbol( "LNUX" );
	  symbolGroup.set( symbol );

	  FIX42::MarketDataRequest message( mdReqID, subType, marketDepth );
	  message.addGroup( marketDataEntryGroup );
	  message.addGroup( symbolGroup );

	  getHeader( message.getHeader() );

	  std::cout << message.toXML() << std::endl;
	  std::cout << message.toString() << std::endl;

	  return message;
}

FIX::SenderCompID FixClient::askSenderCompID()
{
  std::string response;
  std::cout << std::endl << "SenderCompID: ";
  std::cin >> response;
  while(!confirmation ( "\nIs this right :" + response)) {
  		std::cout << std::endl << "SenderCompID: ";
  		std::cin >> response;
  }
  return FIX::SenderCompID( response );
}

FIX::TargetCompID FixClient::askTargetCompID()
{
  std::string response;
  std::cout << std::endl << "TargetCompID: ";
  std::cin >> response;
  while(!confirmation ( "\nIs this right :" + response)) {
    		std::cout << std::endl << "SenderCompID: ";
    		std::cin >> response;
  }
  return FIX::TargetCompID( response );
}

FIX::TargetSubID FixClient::askTargetSubID()
{
  std::string response;
  std::cout << std::endl << "TargetSubID: ";
  std::cin >> response;
  while(!confirmation ( "\nIs this right :" + response)) {
      		std::cout << std::endl << "SenderCompID: ";
      		std::cin >> response;
  }
  return FIX::TargetSubID( response );
}

FIX::ClOrdID FixClient::askClOrdID()
{
  std::string response;
  std::cout << std::endl << "ClOrdID: ";
  std::cin >> response;
  while(!confirmation ( "\nIs this right :" + response)) {
      		std::cout << std::endl << "SenderCompID: ";
      		std::cin >> response;
  }
  return FIX::ClOrdID( response );
}

FIX::OrigClOrdID FixClient::askOrigClOrdID()
{
	std::string response;
	std::cout << std::endl << "OrigClOrdID: ";
	std::cin >> response;
	while(!confirmation ( "\nIs this right :" + response)) {
		std::cout << std::endl << "SenderCompID: ";
		std::cin >> response;
	}
	return FIX::OrigClOrdID( response );
}

FIX::Symbol FixClient::askSymbol()
{
  std::string response;
  std::cout << std::endl << "Symbol: ";
  std::cin >> response;
  while(!confirmation ( "\nIs this right :" + response)) {
        		std::cout << std::endl << "SenderCompID: ";
        		std::cin >> response;
  }
  return FIX::Symbol( response );
}

FIX::Side FixClient::askSide()
{
  char response;
  response = '9';
  while(!(int(response - '0') < 8) || !(int(response - '0') > 0)) {
	  std::cout << std::endl
	  << "Please enter a valid response" << std::endl
	  << "1) Buy" << std::endl
	  << "2) Sell" << std::endl
	  << "3) Sell Short" << std::endl
	  << "4) Sell Short Exempt" << std::endl
	  << "5) Cross" << std::endl
	  << "6) Cross Short" << std::endl
	  << "7) Cross Short Exempt" << std::endl
	  << "Side: ";

	  std::cin >> response;
  }
  switch ( response )
  {
  	  case '1': return FIX::Side( FIX::Side_BUY );
  	  case '2': return FIX::Side( FIX::Side_SELL );
  	  case '3': return FIX::Side( FIX::Side_SELL_SHORT );
  	  case '4': return FIX::Side( FIX::Side_SELL_SHORT_EXEMPT );
  	  case '5': return FIX::Side( FIX::Side_CROSS );
  	  case '6': return FIX::Side( FIX::Side_CROSS_SHORT );
  	  case '7': return FIX::Side( 'A' );
      default: throw std::exception();
  }
}

FIX::OrderQty FixClient::askOrderQty()
{
	long value;
	std::cout << std::endl << "OrderQty: ";
	std::cin >> value;
	while(!confirmation ( "\nIs this right :" + std::to_string(value))) {
		std::cout << std::endl << "SenderCompID: ";
		std::cin >> value;
	}
	return FIX::OrderQty( value );
}

FIX::OrdType FixClient::askOrdType()
{
	char response;
	response = '9';
	while(!(int(response - '0') < 5) || !(int(response - '0') > 0)) {
		std::cout << std::endl
		<< "Please enter a valid response" << std::endl
		<< "1) Market" << std::endl
		<< "2) Limit" << std::endl
		<< "3) Stop" << std::endl
		<< "4) Stop Limit" << std::endl
		<< "OrdType: ";

		std::cin >> response;
	}
	switch ( response )
	{
	case '1': return FIX::OrdType( FIX::OrdType_MARKET );
	case '2': return FIX::OrdType( FIX::OrdType_LIMIT );
	case '3': return FIX::OrdType( FIX::OrdType_STOP );
	case '4': return FIX::OrdType( FIX::OrdType_STOP_LIMIT );
	default: throw std::exception();
	}
}

FIX::Price FixClient::askPrice()
{
	double value;
	std::cout << std::endl << "Price: ";
	std::cin >> value;
	while(!confirmation ( "\nIs this right :" + std::to_string(value))) {
		std::cout << std::endl << "SenderCompID: ";
		std::cin >> value;
	}
	return FIX::Price( value );
}

FIX::StopPx FixClient::askStopPx()
{
	double value;
	std::cout << std::endl << "StopPx: ";
	std::cin >> value;
	while(!confirmation ( "\nIs this right :" + std::to_string(value))) {
		std::cout << std::endl << "SenderCompID: ";
		std::cin >> value;
	}
	return FIX::StopPx( value );
}

FIX::TimeInForce FixClient::askTimeInForce()
{
  char response;
  response = '9';
  	while(!(int(response - '0') < 6) || !(int(response - '0') > 0)) {
  		std::cout << std::endl
  		<< "Please enter a valid response" << std::endl
		  << "1) Day" << std::endl
		  << "2) IOC" << std::endl
		  << "3) OPG" << std::endl
		  << "4) GTC" << std::endl
		  << "5) GTX" << std::endl
		  << "TimeInForce: ";

  		std::cin >> response;
  	}


  switch ( response )
  {
    case '1': return FIX::TimeInForce( FIX::TimeInForce_DAY );
    case '2': return FIX::TimeInForce( FIX::TimeInForce_IMMEDIATE_OR_CANCEL );
    case '3': return FIX::TimeInForce( FIX::TimeInForce_AT_THE_OPENING );
    case '4': return FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_CANCEL );
    case '5': return FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_CROSSING );
    default: throw std::exception();
  }
}



