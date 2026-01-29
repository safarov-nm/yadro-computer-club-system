#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "computer_club.hpp"
#include "input_validator.hpp"

#define EXPECTED_ARG_COUNT 2
#define MINIMUM_PRICE 1
#define MINIMUM_NUMBER_OF_TABLES 1

#if defined(_WIN32) || defined(_WIN64)
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

int main(int argc, char* argv[]) {
    
    if (argc != EXPECTED_ARG_COUNT) {
        std::cout << "The number of arguments is greater or less than the allowed value. "
        "Please make sure that you pass only one argument to the "
        "program - a text file with input data.";
        exit(1);
    }

    std::ifstream ifs(argv[1], std::ios::in);
    if (!ifs.is_open()) {
        std::cout << "File opening error. Try again.";
        exit(1);
    }

    int numberOfTables;
    ifs >> numberOfTables;
    if (numberOfTables < MINIMUM_NUMBER_OF_TABLES) {
        std::cout << numberOfTables << std::endl;
        std::cout << "An incorrect number of computers was specified. "
        "Please make sure that the value is positive.";
        exit(1);
    }

    std::string startWorkingTime, endWorkingTime;
    ifs >> startWorkingTime >> endWorkingTime;

    if (!isCorrectTime(startWorkingTime) || !isCorrectTime(endWorkingTime)) {
        std::cout << startWorkingTime << ' ' << endWorkingTime << std::endl;
        std::cout << "Time format error. "
                "Make sure that it is written in 24-hour format (XX:XX) and does not contain invalid values." << std::endl;
        exit(1);
    } 

    int pricePerHour;
    ifs >> pricePerHour;
    if (pricePerHour < MINIMUM_PRICE) {
        std::cout << pricePerHour << std::endl;
        std::cout << "The price cannot be negative. Try again." << std::endl;
        exit(1);
    }

    ComputerClub club(numberOfTables,
                      Time(startWorkingTime),
                      Time(endWorkingTime),
                      pricePerHour);

    club.printOpenTime();

    std::string line;
    std::getline(ifs, line); // съесть \n

    Time previousTime;

    std::string timeString, clientName;
    int eventType;

    while (std::getline(ifs, line)) {
        
        if (line.empty()) continue;

        std::stringstream ss(line);
        ss >> timeString >> eventType >> clientName;

        if (!isCorrectTime(timeString)) {
            system(CLEAR_SCREEN);
            std::cout << timeString << std::endl;
            std::cout << "Time format error. Make sure that it is "
            "written in 24-hour format (XX:XX) and does not contain invalid values." << std::endl;
            exit(1);
        }

        if (!(previousTime <= Time(timeString))) {
            system(CLEAR_SCREEN);
            std::cout << timeString << std::endl;
            std::cout << "The time of the current event cannot be less than the time of the previous event" << std::endl;
            exit(1);
        }

        if (!isCorrectClientName(clientName)) {
            system(CLEAR_SCREEN);
            std::cout << clientName << std::endl;
            std::cout << "Incorrect name format. Customer names must be a"
            "combination of characters from the alphabet 'a..z', '0..9', '_', '-'" << std::endl;
            exit(1);
        }

        if (!isCorrectEventType(eventType)) {
            system(CLEAR_SCREEN);
            std::cout << eventType << std::endl;
            std::cout << "Unknown command. Try again." << std::endl;
            exit(1);
        }

        if (eventType == CLIENT_SAT_TABLE) {
            int tableNumber;
            ss >> tableNumber;

            if (!isCorrectTableNumber(tableNumber, numberOfTables)) {
                system(CLEAR_SCREEN);
                std::cout << tableNumber << std::endl;
                std::cout << "Incorrect table number. Make sure that the value is at least 1, "
                "but not more than the total number of tables in the club." << std::endl;
                exit(1);
            }

            club.processEvent(timeString, clientName, eventType, tableNumber);
            previousTime = timeString;
            continue;

        }

        club.processEvent(timeString, clientName, eventType);

        previousTime = timeString;
    }

    club.closeClub();
    club.printCloseTime();
    club.printSummary();

    ifs.close();

    return 0;
}
