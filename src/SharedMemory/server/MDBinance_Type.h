
#ifndef __MDBINANCE_TYPE_H__
#define __MDBINANCE_TYPE_H__

#include <string>


typedef struct {
    std::string host{"stream.binance.com"};
    std::string port{"9443"};
    std::string uri{"/ws/%s@depth@100ms"};
    std::vector<std::string> symbols{"btcusdt"};
} MDBinance_Type;

#endif
