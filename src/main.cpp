#include "food/Food.h"
#include "food/BasicFood.h"
#include "food/CompositeFood.h"
#include "database/FoodDatabase.h"
#include "diet_goal/DietGoalProfile.h"
#include "daily_log/DailyLog.h"
#include "utils/FileHandler.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <algorithm>
#include <limits>

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

int main()
{
    // Create data directory if not exists
    FileHandler::createDirectoryIfNotExists("data");

    // Initialize the food database
    FoodDatabase foodDB("data/basic_foods.txt", "data/composite_foods.txt");

    // Load existing foods from database files
    if (!foodDB.loadFoods())
    {
        std::cout << "Failed to load food database. Starting with an empty database." << std::endl;
    }

    // Initialize log manager
    LogManager logManager("data/daily_logs.txt");
    if (!logManager.loadLogs(foodDB))
    {
        std::cout << "Failed to load daily logs. Starting with an empty log." << std::endl;
    }

    // Initialize diet goal profile
    DietGoalProfile dietProfile("data/diet_profile.txt");

    bool running = true;
    while (running)
    {
        displayMainMenu();

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

        switch (choice)
        {
        case 1:
            manageFoodDatabase(foodDB);
            break;
        case 2:
            manageDailyLogs(foodDB, logManager);
            break;
        case 3:
            manageDietGoalProfile(dietProfile);
            break;
        case 4:
            // Save database, logs, and exit
            if (foodDB.saveFoods() && logManager.saveLogs() && dietProfile.saveToFile())
            {
                std::cout << "Database and logs saved successfully." << std::endl;
                running = false;
            }
            else
            {
                std::cout << "Failed to save database or logs." << std::endl;
            }
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}

void displayMainMenu()
{
    std::cout << "=======================================" << std::endl;
    std::cout << "      YADA - Diet Management System    " << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "1. Manage Food Database" << std::endl;
    std::cout << "2. Manage Daily Logs" << std::endl;
    std::cout << "3. Manage Diet Goal Profile" << std::endl;
    std::cout << "4. Save and Exit" << std::endl;
    std::cout << "0. Exit Without Saving" << std::endl;
    std::cout << "=======================================" << std::endl;
}

// Food Database Micro Operations

void displayMenu()
{
    std::cout << "=======================================" << std::endl;
    std::cout << "           YADA Food Database          " << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "1. Add Basic Food" << std::endl;
    std::cout << "2. Create Composite Food" << std::endl;
    std::cout << "3. Search Foods" << std::endl;
    std::cout << "4. Display All Foods" << std::endl;
    std::cout << "5. Save Database" << std::endl;
    std::cout << "0. Return to Main Menu" << std::endl;
    std::cout << "=======================================" << std::endl;
}

void addBasicFood(FoodDatabase &db)
{
    std::string id, keywordsStr;
    double calories;

    std::cout << "=== Add Basic Food ===" << std::endl;

    // Get food ID
    std::cout << "Enter food ID: ";
    std::getline(std::cin, id);

    // Check if ID already exists
    if (db.getFoodById(id) != nullptr)
    {
        std::cout << "A food with this ID already exists." << std::endl;
        return;
    }

    // Get keywords
    std::cout << "Enter keywords (comma-separated): ";
    std::getline(std::cin, keywordsStr);

    // Get calories
    std::cout << "Enter calories per serving: ";
    std::cin >> calories;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    // Split keywords string into vector
    std::vector<std::string> keywords = splitString(keywordsStr, ',');

    // Create and add the basic food
    auto basicFood = std::make_shared<BasicFood>(id, keywords, calories);
    if (db.addBasicFood(basicFood))
    {
        std::cout << "Basic food added successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to add basic food." << std::endl;
    }
}

void createCompositeFood(FoodDatabase &db)
{
    std::string id, keywordsStr;

    std::cout << "=== Create Composite Food ===" << std::endl;

    // Get food ID
    std::cout << "Enter food ID: ";
    std::getline(std::cin, id);

    // Check if ID already exists
    if (db.getFoodById(id) != nullptr)
    {
        std::cout << "A food with this ID already exists." << std::endl;
        return;
    }

    // Get keywords
    std::cout << "Enter keywords (comma-separated): ";
    std::getline(std::cin, keywordsStr);

    // Split keywords string into vector
    std::vector<std::string> keywords = splitString(keywordsStr, ',');

    // Create the composite food
    auto compositeFood = std::make_shared<CompositeFood>(id, keywords);

    // Add components to the composite food
    bool addingComponents = true;
    while (addingComponents)
    {
        // Display all available foods
        std::cout << "Available foods:" << std::endl;
        auto allFoods = db.getAllFoods();
        for (size_t i = 0; i < allFoods.size(); ++i)
        {
            std::cout << i + 1 << ". " << allFoods[i]->getId() << std::endl;
        }

        int foodIndex;
        double servings;

        std::cout << "Enter food number to add (0 to finish): ";
        std::cin >> foodIndex;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

        if (foodIndex == 0)
        {
            addingComponents = false;
            continue;
        }

        if (foodIndex < 1 || foodIndex > static_cast<int>(allFoods.size()))
        {
            std::cout << "Invalid food number." << std::endl;
            continue;
        }

        std::cout << "Enter number of servings: ";
        std::cin >> servings;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

        // Add the component
        compositeFood->addComponent(allFoods[foodIndex - 1], servings);
        std::cout << "Component added." << std::endl;
    }

    // Add the composite food to the database
    if (db.addCompositeFood(compositeFood))
    {
        std::cout << "Composite food created successfully." << std::endl;
        std::cout << "Total calories per serving: " << compositeFood->getCaloriesPerServing() << std::endl;
    }
    else
    {
        std::cout << "Failed to create composite food." << std::endl;
    }
}

void searchFoods(FoodDatabase &db)
{
    std::string keywordsStr;
    char matchType;

    std::cout << "=== Search Foods ===" << std::endl;

    // Get search keywords
    std::cout << "Enter search keywords (comma-separated): ";
    std::getline(std::cin, keywordsStr);

    // Get match type
    std::cout << "Match (a)ny keyword or (A)ll keywords? ";
    std::cin >> matchType;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    // Split keywords string into vector
    std::vector<std::string> keywords = splitString(keywordsStr, ',');

    // Search for foods
    std::vector<std::shared_ptr<Food>> results;
    if (matchType == 'A')
    {
        results = db.findFoodsMatchingAllKeywords(keywords);
    }
    else
    {
        results = db.findFoodsMatchingAnyKeyword(keywords);
    }

    // Display results
    std::cout << "Search results (" << results.size() << " foods found):" << std::endl;
    for (const auto &food : results)
    {
        food->display();
        std::cout << std::endl;
    }
}

void displayAllFoods(FoodDatabase &db)
{
    db.displayAllFoods();
}

std::vector<std::string> splitString(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter))
    {
        // Trim leading and trailing whitespace
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);

        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

void manageFoodDatabase(FoodDatabase &foodDB)
{

    bool running = true;
    while (running)
    {
        displayMenu();

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

        switch (choice)
        {
        case 1:
            addBasicFood(foodDB);
            break;
        case 2:
            createCompositeFood(foodDB);
            break;
        case 3:
            searchFoods(foodDB);
            break;
        case 4:
            displayAllFoods(foodDB);
            break;
        case 5:
            if (foodDB.saveFoods())
            {
                std::cout << "Database saved successfully." << std::endl;
            }
            else
            {
                std::cout << "Failed to save database." << std::endl;
            }
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

        std::cout << std::endl;
    }

    // Save database on exit
    if (foodDB.saveFoods())
    {
        std::cout << "Database saved successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to save database on exit." << std::endl;
    }
}

void manageDailyLogs(FoodDatabase &db, LogManager &logManager)
{
    bool managingLogs = true;
    while (managingLogs)
    {
        std::cout << "\n=== Daily Log Management ===" << std::endl;
        std::cout << "1. Add Food to Log" << std::endl;
        std::cout << "2. View Log" << std::endl;
        std::cout << "3. Remove Food from Log" << std::endl;
        std::cout << "4. Undo Last Action" << std::endl;
        std::cout << "5. Save Logs" << std::endl; // Add this line
        std::cout << "0. Return to Main Menu" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string date;
        switch (choice)
        {
        case 1:
        {
            std::cout << "Enter date (DD-MM-YYYY): ";
            std::getline(std::cin, date);
            if (!logManager.isValidDateFormat(date))
            {
                std::cout << "Invalid date format. Please use DD-MM-YYYY." << std::endl;
                break;
            }

            int sel = 0;
            while (!sel)
            {
                std::cout << "\nHow would you like to view food (When you know what to add, enter 0 to proceed)?" << std::endl;
                std::cout << "1. View all foods" << std::endl;
                std::cout << "2. Search for specific foods" << std::endl;
                std::cout << "0. Proceed" << std::endl;

                int foodSelectionChoice;
                std::cout << "Enter your choice: ";
                std::cin >> foodSelectionChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::shared_ptr<Food> selectedFood = nullptr;

                switch (foodSelectionChoice)
                {
                case 1:
                {
                    displayAllFoods(db);
                    break;
                }
                case 2:
                {
                    searchFoods(db);
                    break;
                }
                case 0:
                {
                    std::cout << "Returning to main menu" << std::endl;
                    sel = 1;
                    break;
                }
                default:
                {
                    std::cout << "Invalid choice." << std::endl;
                    break;
                }
                }
            }

            std::string foodId;
            double servings;
            std::cout << "Enter food ID: ";
            std::getline(std::cin, foodId);
            std::cout << "Enter number of servings: ";
            std::cin >> servings;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            auto food = db.getFoodById(foodId);
            if (food)
            {
                DailyLog &log = logManager.getLog(date);
                log.addFoodEntry(food, servings);
                logManager.addUndoAction(LogManager::LogAction::ADD, date, food, servings);
                std::cout << "Food added to log." << std::endl;
            }
            else
            {
                std::cout << "Food not found." << std::endl;
            }
            break;
        }
        case 2:
        {
            std::cout << "Enter date (DD-MM-YYYY): ";
            std::getline(std::cin, date);
            if (!logManager.isValidDateFormat(date))
            {
                std::cout << "Invalid date format. Please use DD-MM-YYYY." << std::endl;
                break;
            }
            auto &log = logManager.getLog(date);
            std::cout << "Log for " << date << ":" << std::endl;
            for (size_t i = 0; i < log.getFoodEntries().size(); ++i)
            {
                const auto &entry = log.getFoodEntries()[i];
                std::cout << i + 1 << ". " << entry.getFood()->getId()
                          << " - " << entry.getServings() << " servings, "
                          << entry.getTotalCalories() << " calories" << std::endl;
            }
            std::cout << "Total Calories: " << log.getTotalCalories() << std::endl;
            break;
        }
        case 3:
        {
            std::cout << "Enter date (DD-MM-YYYY): ";
            std::getline(std::cin, date);
            if (!logManager.isValidDateFormat(date))
            {
                std::cout << "Invalid date format. Please use DD-MM-YYYY." << std::endl;
                break;
            }
            std::cout << "Enter index of food to remove: ";
            size_t index;
            std::cin >> index;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            auto &log = logManager.getLog(date);
            size_t actualIndex = index - 1;
            auto foodToRemove = log.getFoodEntries()[actualIndex].getFood();
            double servingsToRemove = log.getFoodEntries()[actualIndex].getServings();

            logManager.addUndoAction(LogManager::LogAction::REMOVE, date, foodToRemove, servingsToRemove, actualIndex);

            if (log.removeFoodEntry(actualIndex))
            {
                std::cout << "Food removed from log." << std::endl;
            }
            else
            {
                std::cout << "Failed to remove food from log." << std::endl;
            }
            break;
        }
        case 4:
            logManager.undo();
            std::cout << "Last action undone." << std::endl;
            break;
        case 5:
            if (logManager.saveLogs())
            {
                std::cout << "Logs saved successfully." << std::endl;
            }
            else
            {
                std::cout << "Failed to save logs." << std::endl;
            }
            break;
        case 0:
            managingLogs = false;
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

void manageDietGoalProfile(DietGoalProfile &profile)
{
    if (!profile.loaded())
    {
        try
        {
            profile.loadFromFile();
        }
        catch (const std::exception &e)
        {
            profile.initializeProfileFromUser();
        }
    }

    bool managing = true;
    while (managing)
    {
        std::cout << "\n=== Diet Goal Profile ===" << std::endl;
        std::cout << "1. Set Gender" << std::endl;
        std::cout << "2. Set Height" << std::endl;
        std::cout << "3. Set Weight" << std::endl;
        std::cout << "4. Set Age" << std::endl;
        std::cout << "5. Set Activity Level" << std::endl;
        std::cout << "6. Set Calorie Calculation Method" << std::endl;
        std::cout << "7. Calculate Target Calories" << std::endl;
        std::cout << "0. Return to Main Menu" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            std::string gender;
            std::cout << "Enter gender (M/F): ";
            std::getline(std::cin, gender);
            try
            {
                profile.setGender(gender == "M" ? Gender::MALE : Gender::FEMALE);
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 2:
        {
            double height;
            std::cout << "Enter height in cm: ";
            std::cin >> height;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            try
            {
                profile.setHeight(height);
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 3:
        {
            double weight;
            std::cout << "Enter weight in kg: ";
            std::cin >> weight;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            try
            {
                profile.setWeight(weight);
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 4:
        {
            int age;
            std::cout << "Enter age: ";
            std::cin >> age;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            try
            {
                profile.setAge(age);
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 5:
        {
            std::cout << "Select Activity Level:" << std::endl;
            std::cout << "1. Sedentary" << std::endl;
            std::cout << "2. Lightly Active" << std::endl;
            std::cout << "3. Moderately Active" << std::endl;
            std::cout << "4. Very Active" << std::endl;
            std::cout << "5. Extra Active" << std::endl;

            int levelChoice;
            std::cin >> levelChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            ActivityLevel levels[] = {
                ActivityLevel::SEDENTARY,
                ActivityLevel::LIGHTLY_ACTIVE,
                ActivityLevel::MODERATELY_ACTIVE,
                ActivityLevel::VERY_ACTIVE,
                ActivityLevel::EXTRA_ACTIVE};

            if (levelChoice >= 1 && levelChoice <= 5)
            {
                profile.setActivityLevel(levels[levelChoice - 1]);
            }
            else
            {
                std::cout << "Invalid choice." << std::endl;
            }
            break;
        }
        case 6:
        {
            std::cout << "Select Calorie Calculation Method:" << std::endl;
            profile.listCalculationMethods();

            int methodChoice;
            std::cin >> methodChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (methodChoice >= 1 && methodChoice <= 2)
            {
                profile.setCalorieCalculationMethod(methodChoice);
                std::cout << "Setting method successful." << std::endl;
            }
            else
            {
                std::cout << "Invalid choice." << std::endl;
            }
            break;
        }
        case 7:
        {
            try
            {
                double targetCalories = profile.calculateTargetCalories();
                std::cout << "Target Daily Calories: " << targetCalories << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "Error calculating target calories: " << e.what() << std::endl;
            }
            break;
        }
        case 0:
            managing = false;
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }
}