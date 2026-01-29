#ifndef TIME_HPP
#define TIME_HPP

#include <string>

class Time {
private:
    int hours;
    int minutes;

public:
    Time(int h = 0, int m = 0);
    Time(const std::string& str);

    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;

    int toMinutes() const;
    int differenceInMinutes(const Time& other) const;

    static Time fromMinutes(int minutes);
    int calculateRevenue(int pricePerHour) const;

    std::string toString() const;
};

#endif // TIME_HPP