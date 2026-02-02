#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "computer_club.hpp"
#include "input_validator.hpp"

#define EXPECTED_ARG_COUNT 2

#define MINIMUM_PRICE 1
#define MINIMUM_NUMBER_OF_TABLES 1

int main(int argc, char* argv[]) {
    
    if (argc != EXPECTED_ARG_COUNT) {
        return 0;
    }

    std::ifstream ifs(argv[1], std::ios::in);
    if (!ifs.is_open()) {
        return 0;
    }

    int numberOfTables;
    ifs >> numberOfTables;
    if (numberOfTables < MINIMUM_NUMBER_OF_TABLES) {
        std::cout << numberOfTables << std::endl;
        return 0;
    }

    std::string startWorkingTime, endWorkingTime;
    ifs >> startWorkingTime >> endWorkingTime;

    if (!isCorrectTime(startWorkingTime) || !isCorrectTime(endWorkingTime)) {
        std::cout << startWorkingTime << ' ' << endWorkingTime << std::endl;
        return 0;
    } 

    int pricePerHour;
    ifs >> pricePerHour;
    if (pricePerHour < MINIMUM_PRICE) {
        std::cout << pricePerHour << std::endl;
        return 0;
    }

    std::string line;
    std::getline(ifs, line); 

    std::vector<std::string> eventLines;
    Time previousTime;

    std::string timeString, clientName;
    int eventType;

    while (std::getline(ifs, line)) {
        
        if (line.empty()) continue;

        std::stringstream ss(line);
        if (!(ss >> timeString >> eventType >> clientName)) {
            std::cout << line << std::endl;
            return 0;
        }

        if (!isCorrectTime(timeString)) {
            std::cout << line << std::endl;
            return 0;
        }

        Time currentTime(timeString);
        if (!(previousTime <= currentTime)) {
            std::cout << line << std::endl;
            return 0;
        }

        if (!isCorrectClientName(clientName)) {
            std::cout << line << std::endl;
            return 0;
        }

        if (!isCorrectEventType(eventType)) {
            std::cout << line << std::endl;
            return 0;
        }

        if (eventType == CLIENT_SAT_TABLE) {
            int tableNumber;
            
            if (!(ss >> tableNumber)) {
                std::cout << line << std::endl;
                return 0;
            }

            if (!isCorrectTableNumber(tableNumber, numberOfTables)) {
                std::cout << line << std::endl;
                return 0;
            }
        } else {
            std::string extra;

            if (ss >> extra) {
                std::cout << line << std::endl;
                return 0;
            }
        }

        previousTime = currentTime;

        eventLines.push_back(line);
    }

    ComputerClub club(numberOfTables,
                      Time(startWorkingTime),
                      Time(endWorkingTime),
                      pricePerHour);

    club.printOpenTime(); 

    for (const auto& line : eventLines) {
        std::stringstream ss(line);
        ss >> timeString >> eventType >> clientName;

        if (eventType == CLIENT_SAT_TABLE) {
            int tableNumber;
            ss >> tableNumber;
            club.processEvent(timeString, clientName, eventType, tableNumber);
        } else {
            club.processEvent(timeString, clientName, eventType);
        }
    }

    club.closeClub();
    club.printCloseTime();
    club.printSummary();

    ifs.close();

    return 0;
}
