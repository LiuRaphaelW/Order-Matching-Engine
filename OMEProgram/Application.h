#ifndef OMEPROGRAM_APPLICATION_H
#define OMEPROGRAM_APPLICATION_H

#include "IDGenerator.h"
#include "Order.h"
#include "OrderMatcher.h"
#include <queue>
#include <iostream>

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Values.h>
#include <quickfix/Utility.h>
#include <quickfix/Mutex.h>

#include <quickfix/fix50/NewOrderSingle.h>
#include <quickfix/fix50/OrderCancelRequest.h>
#include <quickfix/fix50/MarketDataRequest.h>

class FixServer : public FIX::Application , FIX::MessageCracker {
public:
    ~FixServer();
    void onCreate(const FIX::SessionID&);
    void onLogon(const FIX::SessionID& sessionID);
    void onLogout(const FIX::SessionID& sessionID);
    void fromAdmin(const FIX::Message&, const FIX::SessionID&);
    void toAdmin(FIX::Message&, const FIX::SessionID&);
    void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID);
    void toApp(FIX::Message& message, const FIX::SessionID& sessionID);
    



    void onMessage(const FIX42::NewOrderSingle&,
                   const FIX::SessionID&);
    void onMessage(const FIX42::OrderCancelRequest&,
                   const FIX::SessionID&);
    void onMessage(const FIX42::MarketDataRequest&,
                   const FIX::SessionID&);
    void processOrder(const Order&);
    void processCancel(const std::string& id,
                       const std::string& symbol,
                       Order::Side);
    void updateOrder(const Order&, char status);

    void rejectOrder(const Order& order)
    {
        updateOrder(order,
                    FIX::OrdStatus_REJECTED);
    }

    void acceptOrder(const Order& order)
    {
        updateOrder(order,
                    FIX::OrdStatus_NEW);
    }

    void fillOrder(const Order& order)
    {
        updateOrder(order,
                    order.isFilled() ? FIX::OrdStatus_FILLED
                                     : FIX::OrdStatus_PARTIALLY_FILLED);
    }

    void cancelOrder(const Order& order)
    {
        updateOrder(order,
                    FIX::OrdStatus_CANCELED);
    }

    void rejectOrder(const FIX::SenderCompID&,
                     const FIX::TargetCompID&,
                     const FIX::ClOrdID& clOrdID,
                     const FIX::Symbol& symbol,
                     const FIX::Side& side,
                     const std::string& message);

    Order::Side convert(const FIX::Side&) const;

    Order::Type convert(const FIX::OrdType&) const;

    FIX::Side convert(Order::Side) const;

    FIX::OrdType convert(Order::Type) const;

    OrderMatcher m_orderMatcher;
    IDGenerator m_generator;

public:

    const OrderMatcher& orderMatcher() const
    {
        return m_orderMatcher;
    }


};
#endif

