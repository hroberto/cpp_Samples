
#include <iostream>
#include <string>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <fmt/format.h>
#include <thread>
#include <cstdlib>
#include <boost/program_options.hpp>
#include "Tools/ProgramOptionsUtils.h"

#include "../server/Book_Type.h"


#define SHM_KEY 0x1234
#define SEM_KEY 0x54321


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


    // Book_Type bookTop;

    sharedMemory_id = shmget(SHM_KEY, sizeof(Book_Type), 0644);
        if (sharedMemory_id == -1) {
        perror("Shared memory");
        return EXIT_FAILURE;
    }

    Book_Type* const sharedMemory_ptr = (Book_Type*) shmat(sharedMemory_id, NULL, 0);
    if (sharedMemory_ptr == (void *) -1) {
        perror("Shared memory attach");
        return EXIT_FAILURE;
    }


    // std::memcpy(sharedMemory_ptr, &bookTop, sizeof( Book_Type ) );

    unsigned int last_sequence{};
    while (true) {
        // std::memcpy(&bookTop, sharedMemory_ptr, sizeof( Book_Type ) );

        if( sharedMemory_ptr->sequence != last_sequence ) {
            auto semaphore_id = semget(SEM_KEY, 1, 0666);
            
            struct sembuf semaphore_ops[2] { 
                {
                .sem_num = 0,       /* Operate on semaphore 0 */
                .sem_op = 0,        /* Wait resource until value to equals zero */
                .sem_flg = 0 }
                , {
                .sem_num = 0,       /* Operate on semaphore 0 */
                .sem_op = 1,        /* Increment value by one  */
                .sem_flg = SEM_UNDO }
            };

            if (semop(semaphore_id, semaphore_ops, 2) == -1) {
                perror("Semaphore Operation: ");
                return EXIT_FAILURE;
            }

            last_sequence = sharedMemory_ptr->sequence;

            std::cout << fmt::format( "Symbol \"{}\" | Top Bid - value: {}, quantity: {} | Top Ask - value: {}, quantity: {}\n"
                , sharedMemory_ptr->symbol
                , sharedMemory_ptr->top_bid[0], sharedMemory_ptr->top_bid[1]
                , sharedMemory_ptr->top_ask[0], sharedMemory_ptr->top_ask[1]
                );

            struct sembuf  semaphore_ops_release{ .sem_num = 0, .sem_op = -1, .sem_flg = 0 };
            semop(semaphore_id, &semaphore_ops_release, 1);

        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }


    return EXIT_SUCCESS;
}
