#include "FoodDatabase.h"
#include <fstream>
#include <iostream>
#include <algorithm>

FoodDatabase::FoodDatabase(const std::string& basicFoodFilePath, const std::string& compositeFoodFilePath)
    : basicFoodFilePath(basicFoodFilePath), compositeFoodFilePath(compositeFoodFilePath) {}

FoodDatabase::~FoodDatabase() {
    // Automatically save foods on destruction
    saveFoods();
}

bool FoodDatabase::loadFoods() {
    // Clear existing foods
    foods.clear();
    
    // Load basic foods first
    if (!loadBasicFoods()) {
        std::cerr << "Error loading basic foods" << std::endl;
        return false;
    }
    
    // Then load composite foods (which may reference basic foods)
    if (!loadCompositeFoods()) {
        std::cerr << "Error loading composite foods" << std::endl;
        return false;
    }
    
    return true;
}

bool FoodDatabase::loadBasicFoods() {
    std::ifstream file(basicFoodFilePath);
    if (!file) {
        std::cerr << "Could not open basic foods file: " << basicFoodFilePath << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        try {
            auto basicFood = BasicFood::fromString(line);
            foods[basicFood->getId()] = basicFood;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing basic food: " << e.what() << ", line: " << line << std::endl;
        }
    }
    
    return true;
}

bool FoodDatabase::loadCompositeFoods() {
    std::ifstream file(compositeFoodFilePath);
    if (!file) {
        std::cerr << "Could not open composite foods file: " << compositeFoodFilePath << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        try {
            auto compositeFood = CompositeFood::fromString(line, foods);
            foods[compositeFood->getId()] = compositeFood;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing composite food: " << e.what() << ", line: " << line << std::endl;
        }
    }
    
    return true;
}

bool FoodDatabase::saveFoods() {
    return saveBasicFoods() && saveCompositeFoods();
}

bool FoodDatabase::saveBasicFoods() {
    std::ofstream file(basicFoodFilePath);
    if (!file) {
        std::cerr << "Could not open basic foods file for writing: " << basicFoodFilePath << std::endl;
        return false;
    }
    
    file << "# Basic Foods Database" << std::endl;
    file << "# Format: BASIC:id:keyword1,keyword2,...:calories" << std::endl;
    
    for (const auto& pair : foods) {
        auto basicFood = std::dynamic_pointer_cast<BasicFood>(pair.second);
        if (basicFood) {
            file << basicFood->toString() << std::endl;
        }
    }
    
    return true;
}

bool FoodDatabase::saveCompositeFoods() {
    std::ofstream file(compositeFoodFilePath);
    if (!file) {
        std::cerr << "Could not open composite foods file for writing: " << compositeFoodFilePath << std::endl;
        return false;
    }
    
    file << "# Composite Foods Database" << std::endl;
    file << "# Format: COMPOSITE:id:keyword1,keyword2,...:foodId=servings;foodId=servings;..." << std::endl;
    
    for (const auto& pair : foods) {
        auto compositeFood = std::dynamic_pointer_cast<CompositeFood>(pair.second);
        if (compositeFood) {
            file << compositeFood->toString() << std::endl;
        }
    }
    
    return true;
}

bool FoodDatabase::addBasicFood(std::shared_ptr<BasicFood> food) {
    // Check if a food with this ID already exists
    if (foods.find(food->getId()) != foods.end()) {
        return false;
    }
    
    foods[food->getId()] = food;
    return true;
}

bool FoodDatabase::addCompositeFood(std::shared_ptr<CompositeFood> food) {
    // Check if a food with this ID already exists
    if (foods.find(food->getId()) != foods.end()) {
        return false;
    }
    
    foods[food->getId()] = food;
    return true;
}

std::shared_ptr<Food> FoodDatabase::getFoodById(const std::string& id) const {
    auto it = foods.find(id);
    if (it != foods.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Food>> FoodDatabase::findFoodsMatchingAllKeywords(
    const std::vector<std::string>& keywords) const {
    std::vector<std::shared_ptr<Food>> result;
    
    for (const auto& pair : foods) {
        if (pair.second->matchesAllKeywords(keywords)) {
            result.push_back(pair.second);
        }
    }
    
    return result;
}

std::vector<std::shared_ptr<Food>> FoodDatabase::findFoodsMatchingAnyKeyword(
    const std::vector<std::string>& keywords) const {
    std::vector<std::shared_ptr<Food>> result;
    
    for (const auto& pair : foods) {
        if (pair.second->matchesAnyKeyword(keywords)) {
            result.push_back(pair.second);
        }
    }
    
    return result;
}

std::vector<std::shared_ptr<Food>> FoodDatabase::getAllFoods() const {
    std::vector<std::shared_ptr<Food>> result;
    
    for (const auto& pair : foods) {
        result.push_back(pair.second);
    }
    
    return result;
}

void FoodDatabase::displayAllFoods() const {
    std::cout << "All Foods in Database:" << std::endl;
    std::cout << "---------------------" << std::endl;
    
    for (const auto& pair : foods) {
        pair.second->display();
        std::cout << std::endl;
    }
}