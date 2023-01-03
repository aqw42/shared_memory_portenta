#include <Arduino.h>
#include <shared_memory.hh>

void setup()
{
    shared_memory.begin();
}

void loop()
{
    shared_memory.add_data("Test", 3);
}