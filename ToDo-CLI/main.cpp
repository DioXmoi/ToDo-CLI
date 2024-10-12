#include "Task.h"
#include "TaskJsonConverter.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

//Нужно сделать для пользователя отображения времени в виде local time
//Вывод на консоль не работает 
//Сделать тесты для TaskJsonConverter
//Можно создать класс который будет представлять файл

void createFile(const std::wstring& name) {
    std::wofstream file{ name };
    file.imbue(std::locale("en_US.UTF-8"));
    if (!file.is_open()) {
        std::cerr << "File create error!\n";
        std::exit(1);
    }

    file.close();
}

std::wstring readFile(const std::wstring& name) {
    std::wifstream in{ name };
    in.imbue(std::locale("en_US.UTF-8"));
    if (!in.is_open()) {
        createFile(name);

        in.open(name);
        if (!in.is_open()) {
            std::cerr << "File opening error!\n";
            std::exit(1);
        }
    }

    std::wostringstream buffer{ };

    buffer << in.rdbuf();

    in.close();

    return buffer.str();
}

std::vector<Task> convertTasksFromJsonArray(const std::wstring& jsonArray) {
    std::vector<Task> tasks{ };
    std::size_t curIndex{ 0 };
    std::size_t endIndex{ 0 };
    while (curIndex < jsonArray.size()) {
        endIndex = jsonArray.find_first_of(L"}", curIndex);
        if (endIndex == std::string::npos) {
            break;
        }

        std::wstring str{ jsonArray.substr(curIndex, endIndex - curIndex) };
        tasks.emplace_back(TaskJsonConverter::parse(str));
        curIndex = endIndex + 1;
    }

    return tasks;
}

void writeFile(const std::vector<Task>& tasks, const std::wstring& name) {
    std::wofstream out{ name };

    out.imbue(std::locale("en_US.UTF-8"));
    if (!out.is_open()) {
        std::cerr << "File opening error!\n";
        std::exit(1);
    }

    out << L"[\n";
    for (std::size_t i{ 0 }; i < tasks.size(); ++i) {
        out << TaskJsonConverter::to(tasks[i]);

        if (i != tasks.size() - 1) {
            out << L",\n\n";
        }
    }

    out << L"\n]";

    out.flush();

    out.close();
}


std::wostream& operator<<(std::wostream& out, const Task& task) {
    static constexpr std::array nameStatusTask{ L"ToDo", L"InProgress", L"Done" };

    return out << L"Id: " << task.getID() << L"\n" <<
        L"Description: " << task.getDescription() << L"\n" <<
        L"Status: " << nameStatusTask[task.getStatus()] << L"\n" <<
        L"CreatedAt: " << task.getCreatedAt() << L"\n" <<
        L"UpdateAt: " << task.getUpdatedAt() << L"\n";
}

void print(const std::vector<Task>& tasks, bool(*func)(const Task&)) {
    std::wcout.imbue(std::locale("en_US.UTF-8"));
    for (const auto& task : tasks) {
        if (func(task)) {
            std::wcout << task << L'\n';
        }
    }
}

int wmain(int argc, wchar_t* argv[]) {
    std::wcout.imbue(std::locale("en_US.UTF-8"));
    std::wcout << L"我叫阿列克谢 Հայաստան Россия Österreich Ελλάδα भारत\n";

    static const std::wstring fileName{ L"Tasks.json" };
    if (argc == 1) { return 0; } // Если нет аргументов то выход

    std::wstring jsonArray{ readFile(fileName) };
    std::vector<Task> tasks{ convertTasksFromJsonArray(jsonArray) };
    //Можно все лямбды вынести в отдельную функцию
    if (std::wcscmp(argv[1], L"list") == 0) {
        if (argc == 2) {
            print(tasks, [](const Task& task) -> bool { return true || task.ToDo == 0; });
        }
        else if (std::wcscmp(argv[2], L"done") == 0) {
            print(tasks, [](const Task& task) -> bool { return task.getStatus() == Task::Status::Done; });
        }
        else if (std::wcscmp(argv[2], L"todo") == 0) {
            print(tasks, [](const Task& task) -> bool { return task.getStatus() == Task::Status::ToDo; });
        }
        else if (std::wcscmp(argv[2], L"in-progress") == 0) {
            print(tasks, [](const Task& task) -> bool { return task.getStatus() == Task::Status::InProgress; });
        }

        //иначе должна быть ошибка  
    }
    else if (std::wcscmp(argv[1], L"add") == 0) {
        int id{ 0 };
        if (!tasks.empty()) {
            id = tasks.back().getID();
        }
        //Нужно добавить логику если отсутствует второй аргумент argv[2]
        tasks.emplace_back(Task{ id + 1, argv[2] });
    }
    else if (std::wcscmp(argv[1], L"update") == 0) {
        //Нужно добавить логику если отсутствует второй аргумент argv[2](или он является не числом) и третий argv[3]
        int id{ std::stoi(argv[2]) };
        //сделать чтобы возвращала функция
        auto pred{ [id](const Task& task) -> bool { return task.getID() == id; } };
        auto found{ std::find_if(tasks.begin(), tasks.end(), pred) };
        if (found != tasks.end()) {
            found -> setDescription(argv[3]);
        }
        else {
            std::cerr << "The task was not found with the ID - " << id << '\n';
        }
    }
    else if (std::wcscmp(argv[1], L"delete") == 0) {
        //Нужно добавить логику если отсутствует второй аргумент argv[2] или он является не числом 

        int id{ std::stoi(argv[2]) };
        //сделать чтобы возвращала функция 
        auto pred{ [id](const Task& task) -> bool { return task.getID() == id; } };
        auto found{ std::find_if(tasks.begin(), tasks.end(), pred) };
        if (found != tasks.end()) {
            tasks.erase(found);
        }
        else {
            std::cerr << "The task was not found with the ID - " << id << '\n';
        }
    }
    else if (std::wcscmp(argv[1], L"mark-in-progress") == 0) {
        int id{ std::stoi(argv[2]) };
        //Нужно добавить логику если отсутствует второй аргумент argv[2]
        //сделать чтобы возвращала функция 
        auto pred{ [id](const Task& task) -> bool { return task.getID() == id; } };
        auto found{ std::find_if(tasks.begin(), tasks.end(), pred) };
        if (found != tasks.end()) {
            found->setStatus(Task::Status::InProgress);
        }
        else {
            std::cerr << "The task was not found with the ID - " << id << '\n';
        }
    }
    else if (std::wcscmp(argv[1], L"mark-done") == 0) {
        int id{ std::stoi(argv[2]) };
        //Нужно добавить логику если отсутствует второй аргумент argv[2]
        //сделать чтобы возвращала функция 
        auto pred{ [id](const Task& task) -> bool { return task.getID() == id; } };
        auto found{ std::find_if(tasks.begin(), tasks.end(), pred) };
        if (found != tasks.end()) {
            found -> setStatus(Task::Status::Done);
        }
        else {
            std::cerr << "The task was not found with the ID - " << id << '\n';
        }
    }

    writeFile(tasks, fileName);

    return 0;
}