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

## start job 1 in backgroud
./sm_server &
./sm_client

## Stop Job 1 ( sm_server )
fg
<CTRL+C>

```

