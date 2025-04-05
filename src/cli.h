#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <algorithm>
#include <limits>

#include "food/Food.h"
#include "food/BasicFood.h"
#include "food/CompositeFood.h"
#include "database/FoodDatabase.h"
#include "diet_goal/DietGoalProfile.h"
#include "daily_log/DailyLog.h"
#include "utils/FileHandler.h"

#ifndef CLI_H
#define CLI_H

// Function prototypes
void displayMainMenu();
void manageFoodDatabase(FoodDatabase &db);
void manageDailyLogs(FoodDatabase &db, LogManager &logManager);
void manageDietGoalProfile(DietGoalProfile &profile);

// Food Database Operations
void displayMenu();
void addBasicFood(FoodDatabase &db);
void createCompositeFood(FoodDatabase &db);
void searchFoods(FoodDatabase &db);
void displayAllFoods(FoodDatabase &db);
std::vector<std::string> splitString(const std::string &str, char delimiter);

class CLI {

    private:

    FoodDatabase foodDB;
    LogManager logManager;
    DietGoalProfile dietProfile;

    void displayMainMenu();
    void displayMenu();
    void createCompositeFood(FoodDatabase &db);
    void addBasicFood(FoodDatabase &db);
    void searchFoods(FoodDatabase &db);
    void displayAllFoods(FoodDatabase &db);

    std::vector<std::string> splitString(const std::string &str, char delimiter);

    void manageFoodDatabase(FoodDatabase &foodDB);
    void manageDailyLogs(FoodDatabase &db, LogManager &logManager);
    void manageDietGoalProfile(DietGoalProfile &profile);

    public:
    CLI();
    int run();
};

#endif