#include "Task.h"
#include "TaskJsonConverter.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

//нужно сделать для пользователя отображения времени в виде local time

// Сделать тесты для TaskJsonConverter


std::string readJsonArray() {
    std::ifstream in{ "Tasks.json" };
    if (!in.is_open()) {
        std::cerr << "File opening error!\n";
        std::exit(1);
    }

    std::ostringstream buffer{ };

    buffer << in.rdbuf();

    in.close();

    return buffer.str();
}

std::vector<Task> convertTasksFromJsonArray(const std::string& jsonArray) {
    std::vector<Task> tasks{ };
    std::size_t curIndex{ 0 };
    std::size_t endIndex{ 0 };
    while (curIndex < jsonArray.size()) {
        endIndex = jsonArray.find_first_of("}", curIndex);
        if (endIndex == std::string::npos) {
            break;
        }

        std::string str{ jsonArray.substr(curIndex, endIndex - curIndex) };
        tasks.emplace_back(TaskJsonConverter::parse(str));
        curIndex = endIndex + 1;
    }

    return tasks;
}

void writeJsonArray(const std::vector<Task>& tasks) {
    std::ofstream out;
    out.open("Tasks.json");
    if (!out.is_open()) {
        std::cerr << "File opening error!\n";
        std::exit(1);
    }

    out << "[\n";
    for (std::size_t i{ 0 }; i < tasks.size(); ++i) {
        out << TaskJsonConverter::to(tasks[i]);
        if (i != tasks.size() - 1) {
            out << ",\n\n";
        }
    }

    out << "\n]";

    out.close();
}

int main(int argc, char* argv) {
    if (argc == 1) { return 0; }
    
    std::string jsonArray{ readJsonArray() };
    std::vector<Task> tasks{ convertTasksFromJsonArray(jsonArray) };


    writeJsonArray(tasks);

    return 0;
}
//"update" 2 "add second task"
//Добавление, обновление и удаление задач
//Пометьте задачу как выполняемую или выполненную
//Список всех задач
//Список всех выполненных задач
//Список всех задач, которые не выполнены
//Список всех выполняемых задач