#include "Time.h"
#include "TaskManager.h"
#include "File.h"
#include "Utf8Converter.h"
#include <algorithm>
#include <array>
#include <iostream>


enum CommandTypes {
    List,
    Add,
    Update,
    Delete,
    MarkInProgress,
    MarkDone,
    Max_Types,
};

static constexpr std::array commandsName{ "list", "add", "update", "delete", "mark-in-progress", "mark-done" };
static_assert(std::ssize(commandsName) == CommandTypes::Max_Types);

static CommandTypes parseCommandTypes(std::string_view command);

static void handleList(const std::vector<std::string>& arguments, const std::vector<Task>& tasks);
static void handleAdd(const std::vector<std::string>& arguments, std::vector<Task>& tasks);
static void handleUpdate(const std::vector<std::string>& arguments, std::vector<Task>& tasks);
static void handleDelete(const std::vector<std::string>& arguments, std::vector<Task>& tasks);
static void handleMarkInProgress(const std::vector<std::string>& arguments, std::vector<Task>& tasks);
static void handleMarkDone(const std::vector<std::string>& arguments, std::vector<Task>& tasks);

static std::ostream& operator<<(std::ostream& out, const Task& task);


namespace TaskManager {

    std::vector<std::string> parseArguments(int argc, wchar_t* argv[]) {
        std::vector<std::string> arguments;
        arguments.reserve(argc);

        for (int i{ 1 }; i < argc; ++i) {
            arguments.emplace_back(Utf8Converter::utf8_encode(argv[i]));
        }

        return arguments;
    }

    void handleCommand(const std::vector<std::string>& arguments, std::vector<Task>& tasks) {
        if (arguments.empty()) { return; } // If there are no arguments, then exit immediately

        CommandTypes type{ parseCommandTypes(arguments.front()) };
        switch (type) {
        case CommandTypes::List:
            handleList(arguments, tasks);
            break;
        case CommandTypes::Add:
            handleAdd(arguments, tasks);
            break;
        case CommandTypes::Update:
            handleUpdate(arguments, tasks);
            break;
        case CommandTypes::Delete:
            handleDelete(arguments, tasks);
            break;
        case CommandTypes::MarkInProgress:
            handleMarkInProgress(arguments, tasks);
            break;
        case CommandTypes::MarkDone:
            handleMarkDone(arguments, tasks);
            break;
        default:
            std::cerr << "Error: invalid argument.\n";
            break;
        }
    }
}


static CommandTypes parseCommandTypes(std::string_view command) {
    auto found{ std::find(commandsName.begin(), commandsName.end(), command) };
    if (found != commandsName.end()) {
        return static_cast<CommandTypes>(std::distance(commandsName.begin(), found));
    }

    return CommandTypes::Max_Types;
}

static bool isOnlyOneArgument(const std::vector<std::string>& arguments) {
    return arguments.size() == 1;
}

static std::ostream& operator<<(std::ostream& out, const Task& task) {
    static constexpr std::array nameStatusTask{ "ToDo", "InProgress", "Done" };

    return out << "Id: " << task.getID() <<
        "\nDescription: " << task.getDescription() <<
        "\nStatus: " << nameStatusTask[task.getStatus()] <<
        "\nCreatedAt: " << Time::toLocal(task.getCreatedAt()) <<
        "\nUpdateAt: " << Time::toLocal(task.getUpdatedAt()) << "\n";
}

static void printAllTasks(const std::vector<Task>& tasks) {
    for (const auto& task : tasks) {
        std::cout << task << "\n";
    }
}

static void printTasksOnStatus(const std::vector<Task>& tasks, Task::Status status) {
    for (const auto& task : tasks) {
        if (task.getStatus() == status) {
            std::cout << task << "\n";
        }
    }
}

static void handleList(const std::vector<std::string>& arguments, const std::vector<Task>& tasks) {
    if (isOnlyOneArgument(arguments)) {
        printAllTasks(tasks);
        return;
    }

    static constexpr std::size_t indexStatus{ 1 };
    Task::Status status{ Task::parseStatus(arguments[indexStatus]) };
    if (status == Task::Status::Error) {
        std::cerr << "Error: invalid second argument.\n";
        return;
    }

    printTasksOnStatus(tasks, status);
}

static bool isDescriptionOfTask(const std::vector<std::string>& arguments, std::size_t indexDescription) {
    return arguments.size() >= indexDescription;
}

static void handleAdd(const std::vector<std::string>& arguments, std::vector<Task>& tasks) {
    static constexpr std::size_t indexDescription{ 1 };
    if (!isDescriptionOfTask(arguments, indexDescription)) {
        std::cerr << "Error: the task description is missing.\n";
        return;
    }

    int id{ 1 }; // id of the first task
    if (!tasks.empty()) {
        id = tasks.back().getID() + 1; 
    }

    tasks.emplace_back(Task{ id, arguments[indexDescription]});
}

static int isValideID(const std::vector<std::string>& arguments, std::size_t indexID) {
    static constexpr int error{ -1 };//Error code if there is no argument or it is impossible to parse a string into a number
    if (arguments.size() < indexID) {
        std::cerr << "Error: The task ID is missing.\n";
        return error;
    }

    int id;
    try {
        id = std::stoi(arguments[indexID]);
    }
    catch (std::exception&) {
        std::cerr << "Error: cannot be converted - " << arguments[indexID] << "to a number.\n";
        return error;
    }

    return id;
}

static auto findTaskById(std::vector<Task>& tasks, int id) {
    auto pred{ [id](const Task& task) -> bool { return task.getID() == id; } };
    return std::find_if(tasks.begin(), tasks.end(), pred);
}

static void handleUpdate(const std::vector<std::string>& arguments, std::vector<Task>& tasks) {
    static constexpr std::size_t indexID{ 1 };
    static constexpr std::size_t indexDescription{ 2 };
    static constexpr int error{ -1 };

    int id{ isValideID(arguments, indexID) };

    if (id != error) {
        if (!isDescriptionOfTask(arguments, indexDescription)) {
            std::cerr << "Error: the task description is missing.\n";
            return;
        }

        auto found{ findTaskById(tasks, id) };
        if (found != tasks.end()) {
            found -> setDescription(arguments[indexDescription]);
        }
        else {
            std::cerr << "The task was not found with the ID - " << id << '\n';
        }
    }
}

static void handleDelete(const std::vector<std::string>& arguments, std::vector<Task>& tasks) {
    static constexpr std::size_t indexID{ 1 };
    static constexpr int error{ -1 };

    int id{ isValideID(arguments, indexID) };

    if (id != error) {
        auto found{ findTaskById(tasks, id) };
        if (found != tasks.end()) {
            tasks.erase(found);
        }
        else {
            std::cerr << "The task was not found with the ID - " << id << '\n';
        }
    }
}

static void handleMarkInProgress(const std::vector<std::string>& arguments, std::vector<Task>& tasks) {
    static constexpr std::size_t indexID{ 1 };
    static constexpr int error{ -1 };

    int id{ isValideID(arguments, indexID) };
    if (id != error) {
        auto found{ findTaskById(tasks, id) };
        if (found != tasks.end()) {
            found -> setStatus(Task::Status::InProgress);
        }
        else {
            std::cerr << "The task was not found with the ID - " << id << '\n';
        }
    }
}

static void handleMarkDone(const std::vector<std::string>& arguments, std::vector<Task>& tasks) {
    static constexpr std::size_t indexID{ 1 };
    static constexpr int error{ -1 };

    int id{ isValideID(arguments, indexID) };
    if (id != error) {
        auto found{ findTaskById(tasks, id) };
        if (found != tasks.end()) {
            found -> setStatus(Task::Status::Done);
        }
        else {
            std::cerr << "The task was not found with the ID - " << id << '\n';
        }
    }
}