#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>

#include <quickfix/fix42/NewOrderSingle.h>

#include <quickfix/SessionSettings.h>
#include <quickfix/FileStore.h>
#include <quickfix/FileLog.h>
#include <quickfix/SocketInitiator.h>


class FixClient : public FIX::Application, public FIX::MessageCracker {
public:
	const int version = 42;


    ~FixClient();
    void onCreate(const FIX::SessionID&);
    void onLogon(const FIX::SessionID& sessionID);
    void onLogout(const FIX::SessionID& sessionID);
    void fromAdmin(const FIX::Message&, const FIX::SessionID&);
    void toAdmin(FIX::Message&, const FIX::SessionID&);
    void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID);
    void toApp(FIX::Message& message, const FIX::SessionID& sessionID);

    FIX42::NewOrderSingle NewOrderSingle();
    FIX42::OrderCancelRequest OrderCancelRequest();
    FIX42::OrderCancelReplaceRequest CancelReplaceRequest();
    FIX42::MarketDataRequest MarketDataRequest();

    void run();
    void printActions();
    void takeOrder();
    void cancelOrder();
    void replaceOrder();
    void marketDataRequest();
    bool confirmation(std::string confirmationMessage);
    std::string ask(std::string question);
    void getHeader(FIX::Header& header);

    FIX::SenderCompID askSenderCompID();
    FIX::TargetCompID askTargetCompID();
    FIX::TargetSubID askTargetSubID();
    FIX::ClOrdID askClOrdID();
    FIX::OrigClOrdID askOrigClOrdID();
    FIX::Symbol askSymbol();
    FIX::Side askSide();
    FIX::OrderQty askOrderQty();
    FIX::OrdType askOrdType();
    FIX::Price askPrice();
    FIX::StopPx askStopPx();
    FIX::TimeInForce askTimeInForce();
};
