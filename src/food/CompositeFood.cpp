#include "CompositeFood.h"
#include <iostream>
#include <sstream>

CompositeFood::CompositeFood(const std::string& id, const std::vector<std::string>& keywords)
    : Food(id, keywords) {}

void CompositeFood::addComponent(std::shared_ptr<Food> food, double servings) {
    // If the food is already a component, add the servings
    if (components.find(food) != components.end()) {
        components[food] += servings;
    } else {
        components[food] = servings;
    }
}

std::map<std::shared_ptr<Food>, double> CompositeFood::getComponents() const {
    return components;
}

double CompositeFood::getCaloriesPerServing() const {
    double totalCalories = 0.0;
    
    // Sum up calories from all components
    for (const auto& component : components) {
        totalCalories += component.first->getCaloriesPerServing() * component.second;
    }
    
    return totalCalories;
}

std::string CompositeFood::toString() const {
    std::stringstream ss;
    ss << "COMPOSITE:" << id << ":";
    
    // Add keywords separated by commas
    for (size_t i = 0; i < keywords.size(); ++i) {
        ss << keywords[i];
        if (i < keywords.size() - 1) {
            ss << ",";
        }
    }
    
    ss << ":";
    
    // Add components in format: foodId=servings;foodId=servings;...
    bool first = true;
    for (const auto& component : components) {
        if (!first) {
            ss << ";";
        }
        ss << component.first->getId() << "=" << component.second;
        first = false;
    }
    
    return ss.str();
}

std::shared_ptr<CompositeFood> CompositeFood::fromString(const std::string& str, 
                                                        const std::map<std::string, std::shared_ptr<Food>>& foodMap) {
    std::stringstream ss(str);
    std::string type, id, keywordStr, componentsStr;
    
    // Parse the string: "COMPOSITE:id:keyword1,keyword2,...:foodId=servings;foodId=servings;..."
    std::getline(ss, type, ':');
    if (type != "COMPOSITE") {
        throw std::invalid_argument("Not a composite food entry");
    }
    
    std::getline(ss, id, ':');
    std::getline(ss, keywordStr, ':');
    std::getline(ss, componentsStr);
    
    // Parse keywords
    std::vector<std::string> keywords;
    std::stringstream keywordStream(keywordStr);
    std::string keyword;
    while (std::getline(keywordStream, keyword, ',')) {
        keywords.push_back(keyword);
    }
    
    // Create the composite food
    auto compositeFood = std::make_shared<CompositeFood>(id, keywords);
    
    // Parse components
    std::stringstream componentStream(componentsStr);
    std::string componentStr;
    while (std::getline(componentStream, componentStr, ';')) {
        std::stringstream componentParts(componentStr);
        std::string foodId, servingsStr;
        
        std::getline(componentParts, foodId, '=');
        std::getline(componentParts, servingsStr);
        
        double servings = std::stod(servingsStr);
        
        // Find the food in the food map
        auto foodIter = foodMap.find(foodId);
        if (foodIter != foodMap.end()) {
            compositeFood->addComponent(foodIter->second, servings);
        } else {
            throw std::runtime_error("Component food not found: " + foodId);
        }
    }
    
    return compositeFood;
}

void CompositeFood::display() const {
    std::cout << "Composite Food: " << id << std::endl;
    std::cout << "  Keywords: ";
    for (size_t i = 0; i < keywords.size(); ++i) {
        std::cout << keywords[i];
        if (i < keywords.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
    std::cout << "  Components:" << std::endl;
    
    for (const auto& component : components) {
        std::cout << "    " << component.first->getId() << ": " << component.second << " serving(s)" << std::endl;
    }
    
    std::cout << "  Total calories per serving: " << getCaloriesPerServing() << std::endl;
}