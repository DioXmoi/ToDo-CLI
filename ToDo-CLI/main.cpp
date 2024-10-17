#include "Task.h"
#include "TaskManager.h"
#include "TaskJsonConverter.h"
#include "File.h"


#include <iostream>
#include <vector>
#include <string>

#include <Windows.h>

//Console output does not work in foreign languages 
//Make tests for all classes

int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, "");

    std::chrono::system_clock::time_point t{ std::chrono::system_clock::now() };

    static const std::string fileName{ "Tasks.json" };

    File file{ fileName };
 
    std::string jsonArray{ file.read() };
    
    std::vector<Task> tasks{ TaskJsonConverter::parseArray(jsonArray) };

    std::vector<std::string> arguments{ TaskManager::parseArguments(argc, argv) };

    TaskManager::handleCommand(arguments, tasks);

    std::string text{ TaskJsonConverter::serialize(tasks) }; 

    file.write(text);

    return 0;
}