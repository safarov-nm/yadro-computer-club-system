#include <iostream>
#include <sstream>

#include "computer_club.hpp"

#define NO_TABLE -1

ComputerClub::ComputerClub(int tableCount,
                           const Time& startWorkingTime,
                           const Time& endWorkingTime,
                           int pricePerHour)
    : pricePerHour(pricePerHour), openTime(startWorkingTime), closeTime(endWorkingTime) {
    tables.resize(tableCount);
}

void ComputerClub::printOpenTime() const {
    std::cout << openTime.toString() << std::endl;
}

void ComputerClub::printCloseTime() const {
    std::cout << closeTime.toString() << std::endl;
}

int ComputerClub::findFreeTable() const {

    for (int i = 0; i < (int)tables.size(); ++i)
        if (!tables[i].occupied)
            return i;
    
    return NO_TABLE;

}

void ComputerClub::handleArrival(const Time& eventTime, const std::string& timeString, Client& client) {
   
    if (eventTime < openTime || !(eventTime < closeTime)) {
        outputError(timeString, ErrorMsg::NOT_OPEN_YET);
        return;
    }
    
    if (client.inClub) {
        outputError(timeString, ErrorMsg::YOU_SHALL_NOT_PASS);
        return;
    }
    
    client.inClub = true;   

}

void ComputerClub::handleSitDown(const Time& eventTime, const std::string& timeString, Client& client, int tableNumber) {
        
        tableNumber--;

        if (!client.inClub) {
            outputError(timeString, ErrorMsg::CLIENT_UNKNOWN);
            return;
        }

        if (tables[tableNumber].occupied) {
            outputError(timeString, ErrorMsg::PLACE_IS_BUSY);
            return;
        }

        if (client.table != NO_TABLE) {
            Table& oldTable = tables[client.table];
            int mins = eventTime.differenceInMinutes(oldTable.start);
            oldTable.totalMinutes += mins;
            oldTable.revenue += Time::fromMinutes(mins).calculateRevenue(pricePerHour);
            oldTable.occupied = false;
        }

        client.table = tableNumber;
        client.start = eventTime;
        tables[tableNumber].occupied = true;
        tables[tableNumber].start = eventTime;

}

void ComputerClub::handleWaiting(const Time& eventTime, const std::string& timeString, Client& client, const std::string& clientName) {
        
        if (findFreeTable() != NO_TABLE) {
            outputError(timeString, ErrorMsg::I_CAN_WAIT_NO_LONGER);
            return;
        }

        if (queue.size() >= tables.size()) {
            std::cout << timeString << ' ' << CLIENT_FORCED_OUT << ' ' << clientName << std::endl;
            client.inClub = false;
            return;
        }

        queue.push(clientName);

}

void ComputerClub::handleDeparture(const Time& eventTime, const std::string& timeString, Client& client, const std::string& clientName) {

        if (!client.inClub) {
            outputError(timeString, ErrorMsg::CLIENT_UNKNOWN);
            return;
        }

        if (client.table != NO_TABLE) {

            Table& t = tables[client.table];
            int mins = eventTime.differenceInMinutes(t.start);
            t.totalMinutes += mins;
            t.revenue += Time::fromMinutes(mins)
                             .calculateRevenue(pricePerHour);
            t.occupied = false;

            if (!queue.empty()) {
                std::string next = queue.front();
                queue.pop();

                Client& nextClient = clients[next];
                nextClient.inClub = true;
                nextClient.table = client.table;
                nextClient.start = eventTime;

                t.occupied = true;
                t.start = eventTime;

                std::cout << eventTime.toString()
                          << ' ' << CLIENT_SAT_FROM_QUEUE 
                          << ' ' << next
                          << ' ' << client.table + 1 << std::endl;

            }

        } else {

            std::queue<std::string> tempQueue;

            while (!queue.empty()) {

                std::string qName = queue.front();
                queue.pop();

                if (qName != clientName) {
                    tempQueue.push(qName);
                }
            }

            queue = tempQueue;
        }

        client.inClub = false;
        client.table = NO_TABLE;

}

void ComputerClub::outputError(const std::string& timeString, const std::string& errorMsg) {
    std::cout << timeString << ' ' << ERROR_OCCURRED << ' ' << errorMsg << std::endl;
}

void ComputerClub::processEvent(const std::string& timeString, const std::string& clientName, const int& eventType, int tableNumber) {

    std::cout << timeString << ' ' << eventType << ' ' << clientName 
              << (tableNumber != NO_TABLE ? " " + std::to_string(tableNumber) : "") << std::endl;

    Time eventTime(timeString);
    Client& client = clients[clientName];
    client.name = clientName;

    switch (eventType) {
        case CLIENT_ARRIVED: // 1 — пришёл
            handleArrival(eventTime, timeString, client);
            break;
        case CLIENT_SAT_TABLE: // 2 — сел за стол
            handleSitDown(eventTime, timeString, client, tableNumber);
            break;
        case CLIENT_WAITING: // 3 — ожидание
            handleWaiting(eventTime, timeString, client, clientName);
            break;
        case CLIENT_LEFT: // 4 — ушёл
            handleDeparture(eventTime, timeString, client, clientName);
            break;
    }

}

void ComputerClub::closeClub() {
    // Клиенты за столами
    for (auto& pair : clients) {
        Client& c = pair.second;
        if (c.inClub && c.table != NO_TABLE) {
            Table& t = tables[c.table];
            int mins = closeTime.differenceInMinutes(t.start);
            t.totalMinutes += mins;
            t.revenue += Time::fromMinutes(mins).calculateRevenue(pricePerHour);
            t.occupied = false;
        }
    }
    
    // Выводим всех клиентов в алфавитном порядке
    for (auto& pair : clients) {
       
        Client& c = pair.second;
       
        if (c.inClub) {
            std::cout << closeTime.toString() 
            << ' ' << CLIENT_FORCED_OUT 
            << ' ' << pair.first << std::endl;
            
            c.inClub = false;
            c.table = NO_TABLE;
        }
    }
    
    // Очищаем очередь
    while (!queue.empty()) {
        queue.pop();
    }
}

void ComputerClub::printSummary() const {

    for (int i = 0; i < (int)tables.size(); ++i) {
        Time t = Time::fromMinutes(tables[i].totalMinutes);
        
        std::cout << i + 1 << ' '
                  << tables[i].revenue << ' '
                  << t.toString() << std::endl;
    }
}
