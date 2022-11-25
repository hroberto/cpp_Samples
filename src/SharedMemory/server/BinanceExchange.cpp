#include "BinanceExchange.h"

#include <functional>
#include <thread>
#include <cassert>
#include <Poco/Format.h>

#include "Tools/JSonUtils.h"
#include "Tools/StringUtils.h"





BinanceExchange::BinanceExchange(const std::string& nameService, const EventData_Type& event)
    : _nameService{nameService} , _fn_eventData{ event }
{
    setCallback(std::bind(&BinanceExchange::on_message, this, std::placeholders::_1, std::placeholders::_2));
}


/**
 * @brief Destroy the Binance Exchange:: Binance Exchange object
 * 
 */
BinanceExchange::~BinanceExchange() 
{
    stop();
}


/**
 * @brief 
 * 
 * @param sessionName 
 * @param jsonResult 
 */
void BinanceExchange::on_message(const std::string& sessionName, const std::string& jsonResult) 
{
    rapidjson::Document docJson;
    if (docJson.Parse<0>(jsonResult.c_str()).HasParseError()) {
        std::cerr << fmt::format("BinanceExchange | [{}] - Parser Error Json - sessionName \"{}\" - \" offset: {} => {}\" ", _nameService, sessionName, docJson.GetErrorOffset(), rapidjson::GetParseError_En(docJson.GetParseError()) ) << std::endl;
        return;
    }

    assert( docJson.HasMember( "s" ));
    const std::string symbol_id = docJson["s"].GetString();

    assert( docJson.HasMember( "e" ));
    if (strcmp( docJson["e"].GetString(), "depthUpdate") != 0 ) {        
        std::cerr << fmt::format( "BinanceExchange | [{}] - Unexpected message (\"{}\")  - symbol_id \"{}\" - JSon := \"{}\" ", _nameService,  docJson["e"].GetString(), symbol_id, jsonResult ) << std::endl;
        return;
    }

    const auto bid = docJson["b"].GetArray();
    const auto ask = docJson["a"].GetArray();
    
    const auto get_json_value = [](const auto& fd) -> double { 
        if( fd.IsInt() ) 
            return fd.GetInt();
        else if( fd.IsString() )
            return  std::strtold(fd.GetString(), NULL);

        assert( fd.IsDouble() );
        return fd.GetDouble();
    };

    std::map< double, double >  v_bid, v_ask;
    for( auto& it : bid ) {
        if( get_json_value(it[1]) == 0) {
            continue;
        }
        v_bid.insert( std::make_pair( get_json_value(it[0]), get_json_value(it[1]) ));
    }
    for( auto& it : ask ) {
        if( get_json_value(it[1]) == 0) {
            continue;
        }
        v_ask.insert( std::make_pair( get_json_value(it[0]), get_json_value(it[1]) ));
    }

    _fn_eventData( symbol_id, v_bid, v_ask );
};


/**
 * @brief 
 * 
 */
void BinanceExchange::run()
{
    setHostAndPort(_config.host, _config.port);

    for( const auto& symbol_id : _config.symbols ) {
        std::string uri_target = Poco::format(_config.uri, symbol_id );

        this->SubscribeCryptoCurrency( symbol_id, uri_target );
    }

    _runner = true;

    while( _runner ) {
        std::this_thread::sleep_for( _intervalSleep );
    }
}