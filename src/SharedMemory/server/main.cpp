
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <rapidjson/document.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>

#include "Tools/JSonUtils.h"
#include "Tools/ProgramOptionsUtils.h"

#include "BinanceExchange.h"
#include "MDBinance_Type.h"
#include "Book_Type.h"


#define SHM_KEY 0x1234
#define SEM_KEY 0x54321


bool remove_all_semaphores(const key_t &semaphore_key);
bool remove_all_sharedMemory(const key_t &sharedMemory_key);
bool semaphore_lock(const int &semaphore_id);
bool semaphore_unlock(const int &semaphore_id);


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

        remove_all_semaphores( SEM_KEY );
        remove_all_sharedMemory( SEM_KEY );

        // std::cout << "|> Limit on the total amount of shared memory: " << SHMALL << std::endl;
        // std::cout << "|> Maximum size in bytes for a shared memory segment: " << SHMMAX  << std::endl;
        
        const auto sharedMemory_id = shmget(SHM_KEY, sizeof(Book_Type), 0644|IPC_CREAT);
         if (sharedMemory_id == -1) {
            perror("Shared memory");
            return EXIT_FAILURE;
        }

        Book_Type* const sharedMemory_ptr = (Book_Type*) shmat(sharedMemory_id, NULL, 0);
        if (sharedMemory_ptr == (void *) -1) {
            perror("Shared memory attach");


            return EXIT_FAILURE;
        }

        int semaphore_id = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
        
        BinanceExchange wsBinance( "BinanceExchange", [&](const std::string symbol_id, const BinanceExchange::ValueList_Type& map_bid, const  BinanceExchange::ValueList_Type& map_ask) {
            semaphore_lock( semaphore_id );

            strcpy(sharedMemory_ptr->symbol, symbol_id.c_str());
            sharedMemory_ptr->top_bid[0] = map_bid.begin()->first;
            sharedMemory_ptr->top_bid[1] = map_bid.begin()->second; 
            sharedMemory_ptr->top_ask[0] = map_ask.begin()->first;
            sharedMemory_ptr->top_ask[1] = map_ask.begin()->second;
            sharedMemory_ptr->sequence++;

            semaphore_unlock( semaphore_id );
        } );

        wsBinance.setConfig( md_config );

        std::thread(
            &BinanceExchange::run, 
            &wsBinance
        ).detach();

        std::cout << "Type any key to quit" << std::endl;
        std::getchar();


        if (semctl(semaphore_id, 0, IPC_RMID) == -1) {
            perror("Remove Semaphore: Semaphore CTL: ");
            return EXIT_FAILURE;
        }

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



bool remove_all_semaphores(const key_t& semaphore_key ) {
    int semaphore_id{};

    while( ( semaphore_id = semget(semaphore_key, 0, IPC_STAT) ) >= 0  ) {
        std::cout << "|> remove semaphore - id = " << semaphore_id << std::endl;
        const int retval = semctl(semaphore_id, 1, IPC_RMID);
       
        if (retval == -1) {
            perror("Remove Semaphore: Semaphore CTL: ");
            return false;
        }
    }
    return true;
}

bool remove_all_sharedMemory(const key_t& sharedMemory_key ) {
    int shm_id{};
    struct shmid_ds info;

    while (( shm_id = shmctl(0, SHM_INFO, &info) ) > 0 ) {
        std::cout << "|> remove shared memory - id = " << shm_id   << std::endl;
        const int retval = shmctl(shm_id, IPC_RMID, 0);
       
        if (retval == -1) {
            perror("Remove SharedMemory: SharedMemory CTL: ");
            return false;
        }
    }
    return true;
}

bool semaphore_lock( const int& semaphore_id) {
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
        return false;
    }

    return true;
}

bool semaphore_unlock( const int& semaphore_id) {
    struct sembuf  semaphore_ops_release{ .sem_num = 0, .sem_op = -1, .sem_flg = 0 };

    /* unlock semaphore */
    if (semop(semaphore_id, &semaphore_ops_release, 1) == -1) {
        perror("Semaphore Operation: ");
        return false;
    }
    return true;
}

