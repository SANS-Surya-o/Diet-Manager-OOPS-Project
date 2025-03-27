#include "DailyLog.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

// DailyLogEntry Implementation
DailyLogEntry::DailyLogEntry(std::shared_ptr<Food> food, double servings)
    : m_food(food), m_servings(servings) {}

std::shared_ptr<Food> DailyLogEntry::getFood() const { return m_food; }
double DailyLogEntry::getServings() const { return m_servings; }
double DailyLogEntry::getTotalCalories() const { return m_food->getCaloriesPerServing() * m_servings; }

// DailyLog Implementation
void DailyLog::addFoodEntry(std::shared_ptr<Food> food, double servings) {
    m_foodEntries.emplace_back(food, servings);
}

bool DailyLog::removeFoodEntry(size_t index) {
    if (index >= m_foodEntries.size()) {
        return false;
    }
    m_foodEntries.erase(m_foodEntries.begin() + index);
    return true;
}

const std::vector<DailyLogEntry>& DailyLog::getFoodEntries() const {
    return m_foodEntries;
}

double DailyLog::getTotalCalories() const {
    double total = 0.0;
    for (const auto& entry : m_foodEntries) {
        total += entry.getTotalCalories();
    }
    return total;
}

void DailyLog::clearEntries() {
    m_foodEntries.clear();
}

void DailyLog::insertFoodEntry(std::shared_ptr<Food> food, double servings, size_t index) {
    // Create the log entry
    DailyLogEntry entry(food, servings);
    
    // If index is valid, insert at position, otherwise append
    if (index < m_foodEntries.size()) {
        m_foodEntries.insert(m_foodEntries.begin() + index, entry);
    } else {
        m_foodEntries.push_back(entry);
    }
}

// LogManager Implementation
LogManager::LogManager(const std::string& logFilePath) : m_logFilePath(logFilePath) {
    std::ofstream file(m_logFilePath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not create or access log file at " << m_logFilePath << std::endl;
    }
    file.close();

    
}


bool LogManager::isValidDateFormat(const std::string& date) {
    // Check length (DD-MM-YYYY = 10 chars)
    if (date.length() != 10) return false;
    
    // Check for hyphens at positions 2 and 5
    if (date[2] != '-' || date[5] != '-') return false;
    
    try {
        // Check that all other positions are digits
        for (int i = 0; i < 10; i++) {
            if (i != 2 && i != 5 && !std::isdigit(date[i]))
                return false;
        }
        
        // Extract day, month, and year
        int day = std::stoi(date.substr(0, 2));
        int month = std::stoi(date.substr(3, 2));
        int year = std::stoi(date.substr(6, 4));
        
        // Basic validation for year, month, and day
        if (year < 2000 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        // Check days in month (simplified, doesn't account for leap years)
        if (month == 2 && day > 29) return false;
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
        
        return true;
    }
    catch (...) {
        return false;
    }
}

bool LogManager::loadLogs(FoodDatabase& foodDatabase) {
    std::ifstream file(m_logFilePath);
    if (!file.is_open()) {
        return false;
    }

    m_logs.clear();
    std::string line;
    std::string currentDate;

    while (std::getline(file, line)) {
        if (line.substr(0, 6) == "DATE: ") {
            currentDate = line.substr(6);
            
            // Validate date format before adding to logs
            if (isValidDateFormat(currentDate)) {
                m_logs[currentDate] = DailyLog();
            } else {
                std::cerr << "Warning: Invalid date format found in log file: " << currentDate << std::endl;
                currentDate = ""; // Reset current date to avoid adding entries to an invalid date
            }
        } else if (!currentDate.empty() && !line.empty()) {
            std::istringstream iss(line);
            std::string foodId;
            double servings;

            if (std::getline(iss, foodId, ',') && iss >> servings) {
                auto food = foodDatabase.getFoodById(foodId);
                if (food) {
                    m_logs[currentDate].addFoodEntry(food, servings);
                } else {
                    std::cerr << "Warning: Food ID '" << foodId << "' not found in database." << std::endl;
                }
            }
        }
    }

    return true;
}

bool LogManager::saveLogs() {
    std::ofstream file(m_logFilePath);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& [date, log] : m_logs) {
        file << "DATE: " << date << std::endl;
        for (const auto& entry : log.getFoodEntries()) {
            file << entry.getFood()->getId() << "," 
                 << entry.getServings() << std::endl;
        }
        file << std::endl;  // Separate different dates
    }

    return true;
}

DailyLog& LogManager::getLog(const std::string& date) {
    if (!isValidDateFormat(date)) {
        std::cerr << "Invalid date format. Please use DD-MM-YYYY." << std::endl;
        // what to return when there is an error - fix later....
    }
    else
    return m_logs[date];

}



void LogManager::addUndoAction(LogAction action, const std::string& date, 
                             std::shared_ptr<Food> food, double servings, size_t index) {
    UndoItem item = {action, date, food, servings, index};
    m_undoStack.push_back(item);
}


void LogManager::undo() {
    if (m_undoStack.empty()) {
        std::cout << "Nothing to undo." << std::endl;
        return;
    }
    
    UndoItem item = m_undoStack.back();
    m_undoStack.pop_back();
    
    try {
        DailyLog& log = m_logs[item.date];
        
        switch (item.action) {
            case LogAction::ADD:
                // If the last action was ADD, we need to remove the food
                if (!log.getFoodEntries().empty()) {
                    log.removeFoodEntry(log.getFoodEntries().size() - 1);
                    std::cout << "Undid addition of " << item.food->getName() << std::endl;
                }
                break;
                
            case LogAction::REMOVE:
                // If the last action was REMOVE, we need to add the food back at its original position
                if (item.index < log.getFoodEntries().size()) {
                    // We need to implement an insertFoodEntry method in DailyLog
                    log.insertFoodEntry(item.food, item.servings, item.index);
                } else {
                    // If the original position is now beyond the end of the list, just append
                    log.addFoodEntry(item.food, item.servings);
                }
                std::cout << "Restored " << item.food->getName() << " to log" << std::endl;
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error during undo: " << e.what() << std::endl;
    }
}


std::string LogManager::getCurrentDateString() {
    std::time_t now = std::time(nullptr);
    std::tm* now_tm = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d");
    return oss.str();
}