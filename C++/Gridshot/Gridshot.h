#pragma once

#include <ctime>
#include <mutex>
#include <string>
#include <random>
#include <thread>
#include <chrono>
#include <cstdlib>

#include "include/Colors.h"
#include "include/Enemy.h"
#include "include/Soldier.h"
#include "include/GameField.h"
#include "include/Projectile.h"
#include "include/FieldObject.h"
#include "include/DisplayStrings.h"
#include PLATFORM_HEADER


namespace Time = std::chrono;
namespace ThisThread = std::this_thread;

using Thread = std::thread;