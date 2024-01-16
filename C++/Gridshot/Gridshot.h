#pragma once

#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

#include PLATFORM_HEADER

#include "src/Colors.h"
#include "src/GameField.h"
#include "src/Projectile.h"
#include "src/FieldObject.h"

namespace Time = std::chrono;
namespace ThisThread = std::this_thread;

using Thread = std::thread;
using Mutex = std::mutex;
using MutexLock = std::lock_guard<std::mutex>;
using SharedProjectile = std::shared_ptr<Projectile>;