#ifndef COMPUTER_CLUB_HPP
#define COMPUTER_CLUB_HPP

#include <map>
#include <queue>
#include <string>
#include <vector>

#include "time.hpp"

enum EventType {
    CLIENT_ARRIVED = 1,      // Входящее: клиент пришел
    CLIENT_SAT_TABLE = 2,    // Входящее: клиент сел за стол
    CLIENT_WAITING = 3,      // Входящее: клиент ожидает
    CLIENT_LEFT = 4,         // Входящее: клиент ушёл
    CLIENT_FORCED_OUT = 11,  // Исходящее: клиент ушёл (принудительно)
    CLIENT_SAT_FROM_QUEUE = 12, // Исходящее: клиент сел из очереди
    ERROR_OCCURRED = 13      // Исходящее: ошибка
};

namespace ErrorMsg {
    const std::string NOT_OPEN_YET = "NotOpenYet";
    const std::string YOU_SHALL_NOT_PASS = "YouShallNotPass";
    const std::string CLIENT_UNKNOWN = "ClientUnknown";
    const std::string PLACE_IS_BUSY = "PlaceIsBusy";
    const std::string I_CAN_WAIT_NO_LONGER = "ICanWaitNoLonger!";

    const std::string CLIENT_IS_AT_TABLE = "IAlreadySittingTable!";
    const std::string CLIENT_IS_IN_QUEUE = "IAlreadyInQueue!";
}

class ComputerClub {

public:

    ComputerClub(int tableCount, const Time& startWorkingTime, const Time& endWorkingTime, int pricePerHour);

    void closeClub();
    void printOpenTime() const;
    void printCloseTime() const;
    void printSummary() const;
    
    void processEvent(const std::string& timeString, const std::string& clientName, const int& eventType, int tableNumber = -1);

private:

    struct Client {
        std::string name;
        bool inClub = false;
        int table = -1;
        Time start;
    };

    struct Table {
        bool occupied = false;
        Time start;
        int totalMinutes = 0;
        int revenue = 0;
    };

    int pricePerHour;
    Time openTime;
    Time closeTime;

    std::map<std::string, Client> clients;
    std::vector<Table> tables;
    std::queue<std::string> queue;

    int findFreeTable() const;

    void handleArrival(const Time& eventTime, const std::string& timeString, Client& client);
    void handleSitDown(const Time& eventTime, const std::string& timeString, Client& client, int tableNumber);
    void handleWaiting(const Time& eventTime, const std::string& timeString, Client& client, const std::string& clientName);
    void handleDeparture(const Time& eventTime, const std::string& timeString, Client& client, const std::string& clientName);

    void outputError(const std::string& timeString, const std::string& errorMsg);

};

#endif // COMPUTER_CLUB_HPP