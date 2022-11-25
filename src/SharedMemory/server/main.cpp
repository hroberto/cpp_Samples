
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <rapidjson/document.h>
#include <sys/shm.h>
#include<errno.h>

#include "Tools/JSonUtils.h"
#include "Tools/ProgramOptionsUtils.h"

#include "BinanceExchange.h"
#include "MDBinance_Type.h"
#include "Book_Type.h"


#define SHM_KEY 0x1234


int main(int argc, char const* argv[]) 
{
    assert( argc != 0 );

    try {
        MDBinance_Type md_config;

        boost::program_options::options_description optDesc("Opções:");
        optDesc.add_options()
                ("host,s", boost::program_options::value(&md_config.host), fmt::format( "Host of WebSocket. Default:= {}", md_config.host ).c_str() )
                ("port,p", boost::program_options::value(&md_config.port ), fmt::format( "Port of WebSocket. Default:= {}", md_config.port ).c_str() )
                ("uri,u", boost::program_options::value(&md_config.uri ), fmt::format( "URI of WebSocket. Default:= {}", md_config.uri ).c_str() )
                ("help,h", "Exibe este menu de ajuda");

        auto vm = libapp::parseOptions(argc, argv, optDesc);
        if (vm.count("help")) {
            std::cout << optDesc << "\n";
            return EXIT_SUCCESS;
        }


        Book_Type bookTop;

        const auto sharedMemory_id = shmget(SHM_KEY, sizeof(Book_Type), 0644|IPC_CREAT);
         if (sharedMemory_id == -1) {
            perror("Shared memory");
            return EXIT_FAILURE;
        }

        const auto sharedMemory_ptr = shmat(sharedMemory_id, NULL, 0);
        if (sharedMemory_ptr == (void *) -1) {
            perror("Shared memory attach");
            return EXIT_FAILURE;
        }


        BinanceExchange wsBinance( "BinanceExchange", [&](const std::string symbol_id, const BinanceExchange::ValueList_Type& map_bid, const  BinanceExchange::ValueList_Type& map_ask) {
            // std::cout << fmt::format( "Symbol \"{}\" | Top Bid - value: {}, quantity: {} | Top Ask - value: {}, quantity: {}\n"
            //             , symbol_id
            //             , map_bid.begin()->first, map_bid.begin()->second
            //             , map_ask.begin()->first, map_ask.begin()->second
            //             );


            strcpy(bookTop.symbol, symbol_id.c_str());
            bookTop.top_bid[0] = map_bid.begin()->first;
            bookTop.top_bid[1] = map_bid.begin()->second; 
            bookTop.top_ask[0] = map_ask.begin()->first;
            bookTop.top_ask[1] = map_ask.begin()->second;
            bookTop.sequence++;

            std::memcpy(sharedMemory_ptr, &bookTop, sizeof( Book_Type ) );

        } );

        wsBinance.setConfig( md_config );

        std::thread(
            &BinanceExchange::run, 
            &wsBinance
        ).join();


        if (shmdt(sharedMemory_ptr) == -1) {
            perror("shmdt");
            return EXIT_FAILURE;
        }
        if (shmctl(sharedMemory_id, IPC_RMID, 0) == -1) {
            perror("shmctl");
            return EXIT_FAILURE;
        }

    } catch (std::exception& e) {
        std::cerr << "Main | Exception - " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
