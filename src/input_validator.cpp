#include "input_validator.hpp"

bool isCorrectClientName(const std::string& name) {

    if (name.empty()) return false;

    for (char c : name) {
        if (!((c >= 'a' && c <= 'z') ||
              (c >= '0' && c <= '9') ||
              c == '_' || c == '-')) {
            return false;
        }
    }

    return true;
}

bool isCorrectTime(const std::string& time) {
    const size_t TIME_LENGTH = 5;
    const size_t COLON_INDEX = 2;

    if (time.size() != TIME_LENGTH || time[COLON_INDEX] != ':') {
        return false;
    }

    for (const auto &position : {0, 1, 3, 4}) {
        if (!std::isdigit(time[position])) {
            return false;
        }
    }

    int hours = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3, 2));

    return (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59);
}

bool isCorrectEventType(const int& eventType) {
    return (eventType >= 1 && eventType <= 4) || (eventType >= 11 && eventType <= 13);
}

bool isCorrectTableNumber(const int& tableNumber, const int& numberOfTables) {
    return tableNumber >= 1 && tableNumber <= numberOfTables;
}
