#include "Food.h"
#include <algorithm>
#include <iostream>

Food::Food(const std::string& id, const std::vector<std::string>& keywords)
    : id(id), keywords(keywords) {}

std::string Food::getId() const {
    return id;
}

std::vector<std::string> Food::getKeywords() const {
    return keywords;
}

void Food::addKeyword(const std::string& keyword) {
    // Check if keyword already exists
    if (std::find(keywords.begin(), keywords.end(), keyword) == keywords.end()) {
        keywords.push_back(keyword);
    }
}

bool Food::matchesAllKeywords(const std::vector<std::string>& searchKeywords) const {
    for (const auto& keyword : searchKeywords) {
        // Convert keyword to lowercase for case-insensitive matching
        std::string lowerKeyword = keyword;
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
        
        bool found = false;
        for (const auto& foodKeyword : keywords) {
            // Convert food keyword to lowercase for comparison
            std::string lowerFoodKeyword = foodKeyword;
            std::transform(lowerFoodKeyword.begin(), lowerFoodKeyword.end(), lowerFoodKeyword.begin(), ::tolower);
            
            if (lowerFoodKeyword.find(lowerKeyword) != std::string::npos) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            return false; // If any keyword doesn't match, return false
        }
    }
    return true; // All keywords matched
}

bool Food::matchesAnyKeyword(const std::vector<std::string>& searchKeywords) const {
    if (searchKeywords.empty()) {
        return true; // If no search keywords provided, match everything
    }
    
    for (const auto& keyword : searchKeywords) {
        // Convert keyword to lowercase for case-insensitive matching
        std::string lowerKeyword = keyword;
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
        
        for (const auto& foodKeyword : keywords) {
            // Convert food keyword to lowercase for comparison
            std::string lowerFoodKeyword = foodKeyword;
            std::transform(lowerFoodKeyword.begin(), lowerFoodKeyword.end(), lowerFoodKeyword.begin(), ::tolower);
            
            if (lowerFoodKeyword.find(lowerKeyword) != std::string::npos) {
                return true; // Found a match
            }
        }
    }
    return false; // No matches found
}