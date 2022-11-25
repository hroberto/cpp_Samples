# cpp_Samples
Some code examples in C++.


# Documments references

- [DEV - Installing Environmet Developer](https://github.com/hroberto/wiki/blob/main/devops/DEV%20-%20Installing%20Environmet%20Developer.md)
- [TOOLS - Install Docker](https://github.com/hroberto/wiki/blob/main/devops/Docker%20-%20Install.md)
- [TOOLS - Redis basics guide](https://github.com/hroberto/wiki/blob/main/devops/Redis%20-%20Guia%20Basico.md)
- [DOCS - Inter Process Communication - Shared Memory](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm)


# Clone the project

```bash
git clone git@github.com:hroberto/cpp_Samples.git
cd cpp_Samples

cmake -DCMAKE_BUILD_TYPE=Debug -S . -B buildDebug
cd buildDebug
make
cd ../bin/Debug/SharedMemory/

ipcs --all
# ------ Message Queues --------
# key        msqid      owner      perms      used-bytes   messages    
# 
# ------ Shared Memory Segments --------
# key        shmid      owner      perms      bytes      nattch     status      
# 
# ------ Semaphore Arrays --------
# key        semid      owner      perms      nsems     
# 


## start job 1 in backgroud
./sm_server

ipcs --all
# ------ Message Queues --------
# key        msqid      owner      perms      used-bytes   messages    

# ------ Shared Memory Segments --------
# key        shmid      owner      perms      bytes      nattch     status      
# 0x00001234 3          ********   644        64         2                       

# ------ Semaphore Arrays --------
# key        semid      owner      perms      nsems     
# 0x00054321 1          ********   666        1  

./sm_client

## Stop Job 1 ( sm_server )
fg
<CTRL+C>


## clear all 
ipcrm  --shmem-id 3
ipcrm  --semaphore-id 1

ipcs --all
# ------ Message Queues --------
# key        msqid      owner      perms      used-bytes   messages    

# ------ Shared Memory Segments --------
# key        shmid      owner      perms      bytes      nattch     status      

# ------ Semaphore Arrays --------
# key        semid      owner      perms      nsems     
#

```

