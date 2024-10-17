#pragma once

#ifndef _15_20_16_10_2024_TASKMANAGER_H_
#define _15_20_16_10_2024_TASKMANAGER_H_

#include "Task.h"
#include <vector>
#include <string>

namespace TaskManager {

    std::vector<std::string> parseArguments(int argc, wchar_t* argv[]);

    void handleCommand(const std::vector<std::string>& arguments, std::vector<Task>& tasks);
}

#endif // !_15_20_16_10_2024_TASKMANAGER_H_