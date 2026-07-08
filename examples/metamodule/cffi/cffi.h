#include <thread>
#include "../../../include/include.h"
#include <functional>
#include <iostream>
#include <fstream>
#include "logic.h"
extern "C" void* construct();
extern "C" void trait_a(void* arg0, int arg1, bool arg2);
extern "C" float trait_b(void* arg0, double arg1);
