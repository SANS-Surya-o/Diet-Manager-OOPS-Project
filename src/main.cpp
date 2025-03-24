#include "food/Food.h"
#include "food/BasicFood.h"
#include "food/CompositeFood.h"
#include "database/FoodDatabase.h"
#include "utils/FileHandler.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <algorithm>
#include <limits>

// Function prototypes
void displayMenu();
void addBasicFood(FoodDatabase& db);
void createCompositeFood(FoodDatabase& db);
void searchFoods(FoodDatabase& db);
void displayAllFoods(FoodDatabase& db);
std::vector<std::string> splitString(const std::string& str, char delimiter);

int main() {
    // Create data directory if not exists
    FileHandler::createDirectoryIfNotExists("data");
    
    // Initialize the food database
    FoodDatabase foodDB("data/basic_foods.txt", "data/composite_foods.txt");
    
    // Load existing foods from database files
    if (!foodDB.loadFoods()) {
        std::cout << "Failed to load food database. Starting with an empty database." << std::endl;
    }
    
    bool running = true;
    while (running) {
        displayMenu();
        
        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        
        switch (choice) {
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
                if (foodDB.saveFoods()) {
                    std::cout << "Database saved successfully." << std::endl;
                } else {
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
    if (foodDB.saveFoods()) {
        std::cout << "Database saved successfully." << std::endl;
    } else {
        std::cout << "Failed to save database on exit." << std::endl;
    }
    
    return 0;
}

void displayMenu() {
    std::cout << "=======================================" << std::endl;
    std::cout << "           YADA Food Database          " << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "1. Add Basic Food" << std::endl;
    std::cout << "2. Create Composite Food" << std::endl;
    std::cout << "3. Search Foods" << std::endl;
    std::cout << "4. Display All Foods" << std::endl;
    std::cout << "5. Save Database" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "=======================================" << std::endl;
}

void addBasicFood(FoodDatabase& db) {
    std::string id, keywordsStr;
    double calories;
    
    std::cout << "=== Add Basic Food ===" << std::endl;
    
    // Get food ID
    std::cout << "Enter food ID: ";
    std::getline(std::cin, id);
    
    // Check if ID already exists
    if (db.getFoodById(id) != nullptr) {
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
    if (db.addBasicFood(basicFood)) {
        std::cout << "Basic food added successfully." << std::endl;
    } else {
        std::cout << "Failed to add basic food." << std::endl;
    }
}

void createCompositeFood(FoodDatabase& db) {
    std::string id, keywordsStr;
    
    std::cout << "=== Create Composite Food ===" << std::endl;
    
    // Get food ID
    std::cout << "Enter food ID: ";
    std::getline(std::cin, id);
    
    // Check if ID already exists
    if (db.getFoodById(id) != nullptr) {
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
    while (addingComponents) {
        // Display all available foods
        std::cout << "Available foods:" << std::endl;
        auto allFoods = db.getAllFoods();
        for (size_t i = 0; i < allFoods.size(); ++i) {
            std::cout << i + 1 << ". " << allFoods[i]->getId() << std::endl;
        }
        
        int foodIndex;
        double servings;
        
        std::cout << "Enter food number to add (0 to finish): ";
        std::cin >> foodIndex;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        
        if (foodIndex == 0) {
            addingComponents = false;
            continue;
        }
        
        if (foodIndex < 1 || foodIndex > static_cast<int>(allFoods.size())) {
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
    if (db.addCompositeFood(compositeFood)) {
        std::cout << "Composite food created successfully." << std::endl;
        std::cout << "Total calories per serving: " << compositeFood->getCaloriesPerServing() << std::endl;
    } else {
        std::cout << "Failed to create composite food." << std::endl;
    }
}

void searchFoods(FoodDatabase& db) {
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
    if (matchType == 'A') {
        results = db.findFoodsMatchingAllKeywords(keywords);
    } else {
        results = db.findFoodsMatchingAnyKeyword(keywords);
    }
    
    // Display results
    std::cout << "Search results (" << results.size() << " foods found):" << std::endl;
    for (const auto& food : results) {
        food->display();
        std::cout << std::endl;
    }
}

void displayAllFoods(FoodDatabase& db) {
    db.displayAllFoods();
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        // Trim leading and trailing whitespace
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}