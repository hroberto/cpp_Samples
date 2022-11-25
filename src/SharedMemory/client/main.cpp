
#include <iostream>
#include <string>
#include <sys/shm.h>
#include <errno.h>
#include <fmt/format.h>
#include <thread>
#include <cstdlib>
#include <boost/program_options.hpp>
#include "Tools/ProgramOptionsUtils.h"

#include "../server/Book_Type.h"


#define SHM_KEY 0x1234


int main(int argc, char const* argv[]) 
{
    int sharedMemory_id{};

    boost::program_options::options_description optDesc("Opções:");
    optDesc.add_options()
            ("shmems,m", "Show shared memory segments ( ipcs -m )." )
            ("shmem-id,r", boost::program_options::value(&sharedMemory_id), "Remove shared memory segment by id ( ipcrm -m .. )." )
            ("help,h", "Exibe este menu de ajuda");

    auto vm = libapp::parseOptions(argc, argv, optDesc);
    if (vm.count("help")) {
        std::cout << optDesc << "\n";
        return EXIT_SUCCESS;
    }
    if (vm.count("shmems")) {
        system( "ipcs -m" );
        return EXIT_SUCCESS;
    }
    if (vm.count("shmem-id")) {
        system( fmt::format("ipcrm -m {}", sharedMemory_id).c_str() );
        return EXIT_SUCCESS;
    }


    Book_Type bookTop;

    sharedMemory_id = shmget(SHM_KEY, sizeof(Book_Type), 0644|IPC_CREAT);
        if (sharedMemory_id == -1) {
        perror("Shared memory");
        return EXIT_FAILURE;
    }

    const auto sharedMemory_ptr = shmat(sharedMemory_id, NULL, 0);
    if (sharedMemory_ptr == (void *) -1) {
        perror("Shared memory attach");
        return EXIT_FAILURE;
    }


    std::memcpy(sharedMemory_ptr, &bookTop, sizeof( Book_Type ) );

    unsigned int last_sequence{};
    while (true) {
        std::memcpy(&bookTop, sharedMemory_ptr, sizeof( Book_Type ) );

        if( bookTop.sequence != last_sequence ) {
            last_sequence = bookTop.sequence;
                std::cout << fmt::format( "Symbol \"{}\" | Top Bid - value: {}, quantity: {} | Top Ask - value: {}, quantity: {}\n"
                    , bookTop.symbol
                    , bookTop.top_bid[0], bookTop.top_bid[1]
                    , bookTop.top_ask[0], bookTop.top_ask[1]
                    );
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }


    return EXIT_SUCCESS;
}
