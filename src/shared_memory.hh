#ifndef SHARED_MEMORY_HH
#define SHARED_MEMORY_HH

#include <map>
#include <vector>
#include <any>
#include <mutex>

#define SRAM_START_ADDRESS ((uint32_t)0x38000000)
#define ARRAY_SIZE 16000

class SharedMemory
{
public:
    void begin();
    void add_data(std::string key, std::any object);
    std::any get_data(std::string key);

private:
    std::map<std::string, std::any> * data = (std::map<std::string, std::any> *)SRAM_START_ADDRESS + sizeof(std::mutex);
    std::mutex *access = SRAM_START_ADDRESS;
};

extern SharedMemory shared_memory;

#endif