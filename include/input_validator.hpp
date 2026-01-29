#ifndef INPUT_VALIDATOR_HPP
#define INPUT_VALIDATOR_HPP

#include <string>

bool isCorrectClientName(const std::string& name);
bool isCorrectEventType(const int& eventType);
bool isCorrectTableNumber(const int& tableNumber, const int& numberOfTables);
bool isCorrectTime(const std::string& time);

#endif // INPUT_VALIDATOR_HPP