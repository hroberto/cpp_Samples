#ifndef __BINANCE_EXCHANGE_H__
#define __BINANCE_EXCHANGE_H__


#include <chrono>
#include <rapidjson/document.h>

#include "Web/ClientExchange.h"
#include "MDBinance_Type.h"



class BinanceExchange : public ClientExchange {
public:
    typedef std::map< double, double > ValueList_Type;
    typedef std::function< void(std::string, ValueList_Type, ValueList_Type) > EventData_Type;


    BinanceExchange(const std::string& nameService, const EventData_Type& event);
    virtual ~BinanceExchange();

    void setConfig(const MDBinance_Type& config) { _config = config; }

    void on_message(const std::string& sessionName, const std::string& jsonResult);
    void run();

private:
    bool _runner{ false };
    std::chrono::milliseconds _intervalSleep{ 100 };
    const std::string _nameService;

    MDBinance_Type _config;
    EventData_Type _fn_eventData{ nullptr };

};

#endif // __BINANCE_EXCHANGE_H__