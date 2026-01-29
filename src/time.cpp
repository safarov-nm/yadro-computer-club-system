#include <iomanip>
#include <sstream>

#include "time.hpp"

Time::Time(int h, int m) : hours(h), minutes(m) {}

Time::Time(const std::string& str) {
    hours = std::stoi(str.substr(0, 2));
    minutes = std::stoi(str.substr(3, 2));
}

bool Time::operator<(const Time& other) const {
    return hours < other.hours ||
          (hours == other.hours && minutes < other.minutes);
}

bool Time::operator<=(const Time& other) const {
    return *this < other || (hours == other.hours && minutes == other.minutes);
}

int Time::toMinutes() const {
    return hours * 60 + minutes;
}

int Time::differenceInMinutes(const Time& other) const {
    return toMinutes() - other.toMinutes();
}

Time Time::fromMinutes(int minutes) {
    return Time(minutes / 60, minutes % 60);
}

int Time::calculateRevenue(int pricePerHour) const {
    int h = toMinutes() / 60;
    if (toMinutes() % 60 != 0)
        h++;
    return h * pricePerHour;
}

std::string Time::toString() const {
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours
       << ":" << std::setw(2) << minutes;
    return ss.str();
}