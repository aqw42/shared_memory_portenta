#include "shared_memory.hh"
#include <Arduino.h>
#include <mbed.h>

using namespace mbed;
using namespace rtos;

SharedMemory shared_memory;

void MPU_config()
{
  /*
  ** Posted by hoih-prog in https://github.com/arduino/ArduinoCore-mbed/issues/369#issuecomment-983225458
  */

  MPU_Region_InitTypeDef MPU_InitStruct;

  HAL_MPU_Disable();
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SRAM_START_ADDRESS;
  // Size of the region to protect, 64K for SRAM4
  MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;                   // Important to access more memory
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void SharedMemory::begin()
{
  #ifdef CORE_CM7
    MPU_config();
    &access = std::mutex();
    &data = std::map<std::string, std::any>();
    bootM4();
    Serial.print("SharedMemory: SRAM table addr : ");
    Serial.println(SRAM_START_ADDRESS, HEX); // MUST HAVE, ELSE CRASH
    memset(data, 0, sizeof(*data));
  #endif
}

void SharedMemory::add_data(std::string key, std::any object)
{
  std::lock_guard<std::mutex> guard(access);
  map[key] = object;
}

std::any SharedMemory::get_data(std::string key)
{
  std::lock_guard<std::mutex> guard(access);
  return map[key];
}
