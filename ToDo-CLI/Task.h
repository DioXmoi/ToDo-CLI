#pragma once

#ifndef _19_46_06_10_2024_TASK_H_
#define _19_46_06_10_2024_TASK_H_

#include <algorithm>
#include <array>
#include <chrono>
#include <stdexcept>
#include <string>
#include <string_view>
#include <iostream>


using ID = int;
using Date = std::chrono::system_clock::time_point;

class Task {
public:

	enum Status {
		ToDo,
		InProgress,
		Done,
		Error,
	};

	Task(ID id, std::string_view description)
		: m_id{ id }
		, m_description{ description }
		, m_status{ ToDo }
		, m_createdAt{ std::chrono::system_clock::now() }
		, m_updateAt{ m_createdAt }
	{
		
	}

	Task(ID id, std::string_view description, Status status, Date createdAt, Date updateAt)
		: m_id{ id }
		, m_description{ description }
		, m_status{ status }
		, m_createdAt{ createdAt }
		, m_updateAt{ updateAt }
	{

	}

	Task(const Task& other) = delete;

	Task(Task&& other) noexcept
		: m_id{ other.m_id }
		, m_description{ std::move(other.m_description) }
		, m_status{ other.m_status }
		, m_createdAt{ std::move(other.m_createdAt) }
		, m_updateAt{ std::move(other.m_updateAt) }
	{
	}

	Task& operator=(const Task& other) = delete;

	Task& operator=(Task&& other) noexcept {
		if (this != &other) {
			m_id = other.m_id;
			m_description = std::move(other.m_description);
			m_status = other.m_status;
			m_createdAt = std::move(other.m_createdAt);
			m_updateAt =std::move(other.m_updateAt);

		}

		return *this;
	}

	~Task() = default;

	ID getID() const { return m_id; }

	Date getCreatedAt() const { return m_createdAt; }

	Date getUpdatedAt() const { return m_updateAt; }

	const std::string& getDescription() const { return m_description; }

	void setDescription(std::string_view description) {
		m_updateAt = std::chrono::system_clock::now();
		m_description = description;
	}
	Status getStatus() const { return m_status; }

	void setStatus(Status status) {
		m_updateAt = std::chrono::system_clock::now();
		m_status = status;
	}

	static Status parseStatus(const std::string& status) {
		auto found{ std::find(s_statusNames.begin(), s_statusNames.end(), status) };
		if (found != s_statusNames.end()) {
			return static_cast<Status>(std::distance(s_statusNames.begin(), found));
		}

		return Status::Error;
	}

	static std::string statusName(Status status) {
		if (Status::ToDo > status || status > Status::Error) {
			throw std::out_of_range("Invalid status value");
		}

		return s_statusNames[status];
	}


private:

	static constexpr std::array s_statusNames{ "ToDo", "InProgress", "Done", "Error" };

	ID m_id;
	std::string m_description;
	Status m_status;
	Date m_createdAt;
	Date m_updateAt;

};

#endif //!_19_46_06_10_2024_TASK_H_