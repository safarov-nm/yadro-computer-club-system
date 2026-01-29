#include "computer_club.hpp"
#include <gtest/gtest.h>
#include <sstream>

// Тест 1: Клиент приходит и уходит
TEST(ComputerClubTest, ClientComesAndLeaves) {
    ComputerClub club(2, Time("09:00"), Time("19:00"), 10);
    
    testing::internal::CaptureStdout();
    club.processEvent("09:30", "client1", CLIENT_ARRIVED);
    club.processEvent("09:40", "client1", CLIENT_SAT_TABLE, 1);
    club.processEvent("10:00", "client1", CLIENT_LEFT);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("client1"), std::string::npos);
}

// Тест 2: Ошибка "NotOpenYet"
TEST(ComputerClubTest, NotOpenYet) {
    ComputerClub club(1, Time("09:00"), Time("19:00"), 10);
    
    testing::internal::CaptureStdout();
    club.processEvent("08:30", "client1", CLIENT_ARRIVED);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("NotOpenYet"), std::string::npos);
}

// Тест 3: Ошибка "ClientUnknown"
TEST(ComputerClubTest, ClientUnknown) {
    ComputerClub club(1, Time("09:00"), Time("19:00"), 10);
    
    testing::internal::CaptureStdout();
    club.processEvent("09:30", "client1", CLIENT_SAT_TABLE, 1);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("ClientUnknown"), std::string::npos);
}

// Тест 4: Ошибка "PlaceIsBusy"
TEST(ComputerClubTest, PlaceIsBusy) {
    ComputerClub club(2, Time("09:00"), Time("19:00"), 10);
    
    testing::internal::CaptureStdout();
    club.processEvent("09:30", "client1", CLIENT_ARRIVED);
    club.processEvent("09:35", "client1", CLIENT_SAT_TABLE, 1);
    club.processEvent("09:40", "client2", CLIENT_ARRIVED);
    club.processEvent("09:45", "client2", CLIENT_SAT_TABLE, 1); // Стол уже занят!
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("PlaceIsBusy"), std::string::npos);
}

// Тест 5: Очередь работает
TEST(ComputerClubTest, QueueWorks) {
    ComputerClub club(1, Time("09:00"), Time("19:00"), 10); // Только 1 стол!
    
    testing::internal::CaptureStdout();
    // Клиент1 садится за стол
    club.processEvent("09:30", "client1", CLIENT_ARRIVED);
    club.processEvent("09:35", "client1", CLIENT_SAT_TABLE, 1);
    
    // Клиент2 встаёт в очередь
    club.processEvent("09:40", "client2", CLIENT_ARRIVED);
    club.processEvent("09:45", "client2", CLIENT_WAITING);
    
    // Клиент1 уходит, Клиент2 должен сесть
    club.processEvent("10:00", "client1", CLIENT_LEFT);
    std::string output = testing::internal::GetCapturedStdout();
    
    // Проверяем, что клиент2 сел (событие 12)
    EXPECT_NE(output.find("12 client2"), std::string::npos);
}

// Тест 6: Закрытие клуба выгоняет всех
TEST(ComputerClubTest, CloseClubKicksEveryone) {
    ComputerClub club(2, Time("09:00"), Time("18:00"), 10);
    
    testing::internal::CaptureStdout();
    club.processEvent("10:00", "client1", CLIENT_ARRIVED);
    club.processEvent("10:05", "client1", CLIENT_SAT_TABLE, 1);
    club.processEvent("11:00", "client2", CLIENT_ARRIVED);
    club.processEvent("11:05", "client2", CLIENT_SAT_TABLE, 2);
    
    club.closeClub();
    std::string output = testing::internal::GetCapturedStdout();
    
    // Должны быть события 11 для обоих клиентов
    EXPECT_NE(output.find("11 client1"), std::string::npos);
    EXPECT_NE(output.find("11 client2"), std::string::npos);
}

TEST(ComputerClubTest, QueueFullClientLeavesImmediately) {
    // По ТЗ: если в очереди уже N клиентов (N = количество столов), 
    // новый клиент сразу уходит (событие 11)
    ComputerClub club(1, Time("09:00"), Time("19:00"), 10); // 1 стол
    
    testing::internal::CaptureStdout();
    
    // Клиент1 занимает стол
    club.processEvent("09:00", "client1", CLIENT_ARRIVED);
    club.processEvent("09:05", "client1", CLIENT_SAT_TABLE, 1);
    
    // Клиент2 встаёт в очередь (очередь: 1 клиент)
    club.processEvent("09:10", "client2", CLIENT_ARRIVED);
    club.processEvent("09:15", "client2", CLIENT_WAITING);
    
    // Клиент3 пытается встать в очередь -> очередь полна -> сразу уходит
    club.processEvent("09:20", "client3", CLIENT_ARRIVED);
    club.processEvent("09:25", "client3", CLIENT_WAITING);
    
    std::string output = testing::internal::GetCapturedStdout();
    
    // Клиент3 должен сразу уйти (событие 11)
    EXPECT_NE(output.find("09:25 11 client3"), std::string::npos);
}

TEST(ComputerClubTest, ClientChangesTableSuccess) {
    ComputerClub club(3, Time("09:00"), Time("19:00"), 10);
    
    testing::internal::CaptureStdout();
    
    // Клиент1 садится за стол 1
    club.processEvent("09:00", "client1", CLIENT_ARRIVED);
    club.processEvent("09:05", "client1", CLIENT_SAT_TABLE, 1);
    
    // Клиент2 садится за стол 2
    club.processEvent("09:10", "client2", CLIENT_ARRIVED);
    club.processEvent("09:15", "client2", CLIENT_SAT_TABLE, 2);
    
    // Клиент1 пересаживается за стол 3
    club.processEvent("09:20", "client1", CLIENT_SAT_TABLE, 3);
    
    std::string output = testing::internal::GetCapturedStdout();
    
    // Не должно быть ошибок
    EXPECT_EQ(output.find("13"), std::string::npos);
    // Стол 1 должен освободиться
    // Клиент1 должен сидеть за столом 3
}

TEST(ComputerClubTest, ICanWaitNoLongerWithFreeTables) {
    ComputerClub club(2, Time("09:00"), Time("19:00"), 10);
    
    testing::internal::CaptureStdout();
    
    // Только 1 стол занят
    club.processEvent("09:00", "client1", CLIENT_ARRIVED);
    club.processEvent("09:05", "client1", CLIENT_SAT_TABLE, 1);
    
    // Клиент2 пытается встать в очередь при свободных столах
    club.processEvent("09:10", "client2", CLIENT_ARRIVED);
    club.processEvent("09:15", "client2", CLIENT_WAITING);
    
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("ICanWaitNoLonger!"), std::string::npos);
}

TEST(ComputerClubTest, ClientLeavesJustInClub) {
    ComputerClub club(1, Time("09:00"), Time("19:00"), 10);
    
    testing::internal::CaptureStdout();
    
    // Клиент1 приходит, но не садится
    club.processEvent("09:00", "client1", CLIENT_ARRIVED);
    
    // Клиент1 уходит
    club.processEvent("09:30", "client1", CLIENT_LEFT);
    
    std::string output = testing::internal::GetCapturedStdout();
    
    // Не должно быть ошибок
    EXPECT_EQ(output.find("13"), std::string::npos);
}

TEST(ComputerClubTest, MultipleClientsLeaveQueue) {
    // 1 стол, клиенты не помещаются в очередь
    ComputerClub club(1, Time("09:00"), Time("19:00"), 10);
    
    // Перехватываем вывод
    testing::internal::CaptureStdout();
    
    // Клиент1 занимает стол
    club.processEvent("09:00", "client1", CLIENT_ARRIVED);
    club.processEvent("09:05", "client1", CLIENT_SAT_TABLE, 1);
    
    // Клиент2 встаёт в очередь
    club.processEvent("09:10", "client2", CLIENT_ARRIVED);
    club.processEvent("09:15", "client2", CLIENT_WAITING);
    
    // Клиент3 пытается встать в очередь -> должен уйти сразу (очередь переполнена: 2 > 1)
    club.processEvent("09:20", "client3", CLIENT_ARRIVED);
    club.processEvent("09:25", "client3", CLIENT_WAITING);
    
    // Клиент4 пытается встать в очередь -> тоже уходит сразу
    club.processEvent("09:30", "client4", CLIENT_ARRIVED);
    club.processEvent("09:35", "client4", CLIENT_WAITING);
    
    // Клиент2 уходит из очереди (добровольно)
    club.processEvent("09:40", "client2", CLIENT_LEFT);
    
    // Клиент3 пытается уйти, но его уже нет -> ошибка
    club.processEvent("09:45", "client3", CLIENT_LEFT);
    
    // Клиент1 уходит со стола, очередь пуста
    club.processEvent("10:00", "client1", CLIENT_LEFT);
    
    club.closeClub();
    club.printCloseTime();
    club.printSummary();
    
    std::string output = testing::internal::GetCapturedStdout();

    
    // 1. client3 должен был уйти сразу при попытке встать в очередь
    EXPECT_NE(output.find("09:25 11 client3"), std::string::npos) 
        << "client3 should leave immediately when queue is full";
    
    // 2. client4 тоже должен уйти сразу
    EXPECT_NE(output.find("09:35 11 client4"), std::string::npos) 
        << "client4 should leave immediately when queue is full";
    
    // 3. client2 успешно уходит из очереди (без события 11)
    EXPECT_NE(output.find("09:40 4 client2"), std::string::npos) 
        << "client2 should leave queue normally";
    
    // 4. client3 пытается уйти, но его нет -> ошибка
    EXPECT_NE(output.find("09:45 13 ClientUnknown"), std::string::npos) 
        << "Should get ClientUnknown when non-existent client leaves";
    
    // 5. Никто не садится за стол после client1 (очередь пуста)
    EXPECT_EQ(output.find("12 client"), std::string::npos) 
        << "No one should sit at table after client1 leaves (queue is empty)";
    
    // 6. Правильная выручка: только client1 сидел 55 мин -> 1 час = 10 руб
    EXPECT_NE(output.find("1 10 00:55"), std::string::npos) 
        << "Revenue should be 10 for 55 minutes (rounded up to 1 hour)";
}

TEST(ComputerClubTest, ClientSitsThenWaitsShouldBeError) {
    ComputerClub club(2, Time("09:00"), Time("19:00"), 10);
    
    testing::internal::CaptureStdout();
    
    club.processEvent("09:00", "client1", CLIENT_ARRIVED);
    club.processEvent("09:05", "client1", CLIENT_SAT_TABLE, 1);
    club.processEvent("09:25", "client1", CLIENT_WAITING);  // ← ДОЛЖНА БЫТЬ ОШИБКА
    
    std::string output = testing::internal::GetCapturedStdout();
    
    // Проверяем, что есть ошибка ClientIsAtTable
    EXPECT_NE(output.find("13 IAlreadySittingTable!"), std::string::npos);
}