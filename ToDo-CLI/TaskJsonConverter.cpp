#include "TaskJsonConverter.h"


#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>


std::string TaskJsonConverter::serialize(const Task& task) {
	std::ostringstream stream{ };
	stream << "{\n" <<
		"\t\"id\": " << std::to_string(task.getID()) << ",\n" <<
		"\t\"description\": \"" << task.getDescription() << "\",\n" <<
		"\t\"status\": \"" << Task::statusName(task.getStatus()) << "\",\n" <<
		"\t\"createdAt\": \"" << task.getCreatedAt() << "\",\n" <<
		"\t\"updateAt\": \"" << task.getUpdatedAt() << "\"\n}";

	return stream.str();
}

std::string TaskJsonConverter::serialize(const std::vector<Task>& tasks) {
	std::ostringstream stream{ };
	for (const auto& task : tasks) {
		stream << TaskJsonConverter::serialize(task) << '\n';
	}

	return stream.str();
}

Task TaskJsonConverter::parse(const std::string& json) {
	ID id{};
	std::string description{};
	Task::Status status{};
	Date createdAt{};
	Date updateAt{};

	char ch;
	std::istringstream stream(json);
	while (stream >> ch) {
		if (ch == '\"') {
			std::string key;
			std::getline(stream, key, '\"');
			stream >> ch;
			stream >> ch;

			std::string value;
			if (ch == '\"') {
				std::getline(stream, value, '\"');
			}
			else {
				stream.putback(ch);
				stream >> value;
			}

			if (key == "id") {
				id = std::stoi(value);
			}
			else if (key == "description") {
				description = value;
			}
			else if (key == "status") {
				status = Task::parseStatus(key);
			}
			else if (key == "createdAt") {
				createdAt = Time::parseDate(value);
			}
			else if (key == "updateAt") {
				updateAt = Time::parseDate(value);
			}
		}
	}

	return Task{ id, description, status, createdAt, updateAt };
}

std::vector<Task> TaskJsonConverter::parseArray(const std::string& jsonArray) {
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