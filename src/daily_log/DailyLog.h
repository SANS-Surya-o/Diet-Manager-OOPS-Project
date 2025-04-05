#ifndef DAILY_LOG_H
#define DAILY_LOG_H

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <ctime>
#include <functional>
#include "food/Food.h"
#include "database/FoodDatabase.h"

class DailyLogEntry
{
public:
    DailyLogEntry(std::shared_ptr<Food> food, double servings);

    std::shared_ptr<Food> getFood() const;
    double getServings() const;
    double getTotalCalories() const;

private:
    std::shared_ptr<Food> m_food;
    double m_servings;
};

class DailyLog
{
public:
    // Add food to the log
    void addFoodEntry(std::shared_ptr<Food> food, double servings);

    // Remove a specific food entry
    bool removeFoodEntry(size_t index);

    // Get all food entries for the day
    const std::vector<DailyLogEntry> &getFoodEntries() const;

    // Calculate total calories for the day
    double getTotalCalories() const;

    // Clear all entries for the day
    void clearEntries();

    // Insert a food entry at a specific position
    void insertFoodEntry(std::shared_ptr<Food> food, double servings, size_t index);

    

private:
    std::vector<DailyLogEntry> m_foodEntries;
};

class LogManager
{
public:
    enum class LogAction
    {
        ADD,
        REMOVE
    };

    struct UndoItem
    {
        LogAction action;
        std::string date;
        std::shared_ptr<Food> food;
        double servings;
        size_t index; // Only used for REMOVE actions
    };
    LogManager(const std::string &logFilePath = "data/daily_logs.txt");

    // Load logs from file
    bool loadLogs(FoodDatabase &db);

    // Save logs to file
    bool saveLogs();

    // Get log for a specific date
    DailyLog &getLog(const std::string &date);

    // Check if a date string is in the correct format (DD-MM-YYYY)
    bool isValidDateFormat(const std::string &date);
    
    void addUndoAction(LogAction action, const std::string& date, 
                  std::shared_ptr<Food> food, double servings, size_t index = 0);
    void undo();

private:
    std::map<std::string, DailyLog> m_logs;
    std::string m_logFilePath;
    std::vector<UndoItem> m_undoStack;

    // Helper to get current date string
    static std::string getCurrentDateString();
};

#endif // DAILY_LOG_H