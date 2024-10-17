#pragma once

#ifndef _20_45_12_10_2024_TIME_H_
#define _20_45_12_10_2024_TIME_H_

#include <chrono>

namespace Time {

	using Date = std::chrono::system_clock::time_point;
	using LocalDate = std::chrono::local_time<std::chrono::system_clock::duration>;

	Date parseDate(const std::string& strDate);
	LocalDate toLocal(Date utc);
}

#endif // !_20_45_12_10_2024_TIME_H_
