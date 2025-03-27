#include "BasicFood.h"
#include <iostream>
#include <sstream>

BasicFood::BasicFood(const std::string& id, const std::vector<std::string>& keywords, double caloriesPerServing)
    : Food(id, keywords), caloriesPerServing(caloriesPerServing) {}

double BasicFood::getCaloriesPerServing() const {
    return caloriesPerServing;
}

void BasicFood::setCaloriesPerServing(double calories) {
    caloriesPerServing = calories;
}

std::string BasicFood::toString() const {
    std::stringstream ss;
    ss << "BASIC:" << id << ":";
    
    // Add keywords separated by commas
    for (size_t i = 0; i < keywords.size(); ++i) {
        ss << keywords[i];
        if (i < keywords.size() - 1) {
            ss << ",";
        }
    }
    
    ss << ":" << caloriesPerServing;
    return ss.str();
}

std::shared_ptr<BasicFood> BasicFood::fromString(const std::string& str) {
    std::stringstream ss(str);
    std::string type, id, keywordStr, caloriesStr;
    
    // Parse the string: "BASIC:id:keyword1,keyword2,...:calories"
    std::getline(ss, type, ':');
    if (type != "BASIC") {
        throw std::invalid_argument("Not a basic food entry");
    }
    
    std::getline(ss, id, ':');
    std::getline(ss, keywordStr, ':');
    std::getline(ss, caloriesStr);
    
    // Parse keywords
    std::vector<std::string> keywords;
    std::stringstream keywordStream(keywordStr);
    std::string keyword;
    while (std::getline(keywordStream, keyword, ',')) {
        keywords.push_back(keyword);
    }
    
    // Parse calories
    double calories = std::stod(caloriesStr);
    
    return std::make_shared<BasicFood>(id, keywords, calories);
}

void BasicFood::display() const {
    std::cout << "Basic Food: " << id << std::endl;
    std::cout << "  Keywords: ";
    for (size_t i = 0; i < keywords.size(); ++i) {
        std::cout << keywords[i];
        if (i < keywords.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
    std::cout << "  Calories per serving: " << caloriesPerServing << std::endl;
}
