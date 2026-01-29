#include "input_validator.hpp"
#include <gtest/gtest.h>

TEST(InputValidatorTest, ValidClientNames) {
    EXPECT_TRUE(isCorrectClientName("client1"));
    EXPECT_TRUE(isCorrectClientName("client_1"));
    EXPECT_TRUE(isCorrectClientName("client-1"));
    EXPECT_TRUE(isCorrectClientName("abc"));
    EXPECT_TRUE(isCorrectClientName("a1b2c3"));
    EXPECT_TRUE(isCorrectClientName("_test_"));
    EXPECT_TRUE(isCorrectClientName("test-123_abc"));
}

TEST(InputValidatorTest, InvalidClientNames) {
    EXPECT_FALSE(isCorrectClientName(""));             
    EXPECT_FALSE(isCorrectClientName("Client")); // имена клиентов могут содержать только строчные латинские буквы нижнего регистра       
    EXPECT_FALSE(isCorrectClientName("CLIENT"));       
    EXPECT_FALSE(isCorrectClientName("client 1"));      
    EXPECT_FALSE(isCorrectClientName("client.1"));      
    EXPECT_FALSE(isCorrectClientName("cli;ent555"));
    EXPECT_FALSE(isCorrectClientName("k@te1"));     
}

TEST(InputValidatorTest, ValidEventType) {
    EXPECT_TRUE(isCorrectEventType(1));
    EXPECT_TRUE(isCorrectEventType(2));
    EXPECT_TRUE(isCorrectEventType(3));
    EXPECT_TRUE(isCorrectEventType(4));
    EXPECT_TRUE(isCorrectEventType(11));
    EXPECT_TRUE(isCorrectEventType(12));
    EXPECT_TRUE(isCorrectEventType(13));
}

TEST(InputValidatorTest, InvalidEventType) {
    EXPECT_FALSE(isCorrectEventType(0));
    EXPECT_FALSE(isCorrectEventType(5));
    EXPECT_FALSE(isCorrectEventType(-1));
    EXPECT_FALSE(isCorrectEventType(111));
    EXPECT_FALSE(isCorrectEventType(10));
    EXPECT_FALSE(isCorrectEventType(14));
    EXPECT_FALSE(isCorrectEventType(-25));
}

TEST(InputValidatorTest, ValidTableNumber) {
    EXPECT_TRUE(isCorrectTableNumber(1, 1));
    EXPECT_TRUE(isCorrectTableNumber(2, 3));
    EXPECT_TRUE(isCorrectTableNumber(8, 10));
    EXPECT_TRUE(isCorrectTableNumber(5, 5));
}

TEST(InputValidatorTest, InvalidTableNumber) {
    EXPECT_FALSE(isCorrectTableNumber(-1, 1));
    EXPECT_FALSE(isCorrectTableNumber(0, 7));
    EXPECT_FALSE(isCorrectTableNumber(9, 3));
    EXPECT_FALSE(isCorrectTableNumber(125, 110));
}