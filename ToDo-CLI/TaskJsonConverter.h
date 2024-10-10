#pragma once

#ifndef _16_21_07_10_2024_JSON_H_
#define _16_21_07_10_2024_JSON_H_

#include "Task.h"

#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>




Date stringToTimePoint(const std::string& strDate) {
	using namespace std::chrono;

	std::tm t{ };
	std::istringstream stream{ strDate };
	stream >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");

	return sys_days{ year{ t.tm_year + 1900 } / (t.tm_mon + 1) / t.tm_mday } +
		hours{ t.tm_hour } + minutes{ t.tm_min } + seconds{ t.tm_sec };
}

class TaskJsonConverter {

public:
	static std::string to(const Task& task) {
		std::ostringstream stream;
		stream << "{\n" <<
			"\t\"id\" : " << std::to_string(task.getID()) << ",\n" <<
			"\t\"description\" : \"" << task.getDescription() << "\",\n" <<
			"\t\"status\" : \"" << nameStatusTask[task.getStatus()] << "\",\n" <<
			"\t\"createdAt\" : \"" << task.getCreatedAt() << "\",\n" <<
			"\t\"updateAt\" : \"" << task.getUpdatedAt() << "\"" << "\n" <<
			"}";

		return stream.str();
	}

	static Task parse(const std::string& json) {
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
				stream.seekg(3, std::ios::cur);

				std::string value;
				if (stream.get() == '\"') {
					std::getline(stream, value, '\"');
				}
				else {
					stream.seekg(-1, std::ios::cur);
					stream >> value;
				}

				if (key == "id") {
					id = std::stoi(value);
				}
				else if (key == "description") {
					description = value;
				}
				else if (key == "status") {
					for (std::size_t i{ 0 }; i < nameStatusTask.size(); ++i) {
						if (value == nameStatusTask[i]) {
							status = static_cast<Task::Status>(i);
							break;
						}
					}
				}
				else if (key == "createdAt") {
					createdAt = stringToTimePoint(value);
				}
				else if (key == "updateAt") {
					updateAt = stringToTimePoint(value);
				}
			}
		}

		return Task{ id, description, status, createdAt, updateAt };
	}

private:
	static constexpr std::array nameStatusTask{ "ToDo", "InProgress", "Done" };

};

#endif // !_16_21_07_10_2024_JSON_H_