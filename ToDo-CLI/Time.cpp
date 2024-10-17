#include "Time.h"

#include <chrono>
#include <ctime>

namespace Time {
	Date parseDate(const std::string& strDate) {
		using namespace std::chrono;

		std::tm t{ };
		std::istringstream stream{ strDate };
		stream >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");

		return sys_days{ year{ t.tm_year + 1900 } / (t.tm_mon + 1) / t.tm_mday } +
			hours{ t.tm_hour } + minutes{ t.tm_min } + seconds{ t.tm_sec };
	}

	LocalDate toLocal(Date utc) {
		auto const time = std::chrono::current_zone()->to_local(utc);

		return time;
	}

}