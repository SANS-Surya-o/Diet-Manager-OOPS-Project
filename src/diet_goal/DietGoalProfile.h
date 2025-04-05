#ifndef DIET_GOAL_PROFILE_H
#define DIET_GOAL_PROFILE_H

#include <string>
#include <vector>
#include <functional>

enum class ActivityLevel {
    SEDENTARY,
    LIGHTLY_ACTIVE,
    MODERATELY_ACTIVE,
    VERY_ACTIVE,
    EXTRA_ACTIVE,
    UNDEFINED
};

enum class Gender {
    UNDEFINED = -1, MALE, FEMALE
};

struct DietProfileLog {
    std::string date;
    int age;
    double weight;
    ActivityLevel activityLevel;
    int calorieCalculationMethodIdx;

    DietProfileLog(std::string date, int age, double weight, ActivityLevel activityLevel, int methodIdx)
        : date(date), age(age), weight(weight), activityLevel(activityLevel), calorieCalculationMethodIdx(methodIdx) {}
};

class CalorieCalculationMethod {
public:
    CalorieCalculationMethod(std::string title, std::function<double(Gender, double, double, int)> calculationFunction);

    double calculate(Gender gender, double weight, double height, int age) const;

    std::string title() const { return m_title; }

private:
    std::string m_title;
    std::function<double(Gender, double, double, int)> m_calculationFunction;
};

class DietGoalProfile {
public:
    DietGoalProfile(std::string filepath = "data/diet_profile.txt");

    // Setters for profile information
    void setGender(Gender gender);
    void setHeight(double height);  // in cm
    void setWeight(double weight);  // in kg
    void setAge(int age);
    void setActivityLevel(ActivityLevel level);
    void setCalorieCalculationMethod(int idx);
    bool loaded();
    void initializeProfileFromUser();

    // Getters
    Gender getGender() const;
    double getHeight() const;
    double getWeight() const;
    int getAge() const;
    ActivityLevel getActivityLevel() const;
    const CalorieCalculationMethod& getCalorieCalculationMethod() const;
    int numberOfCalculationMethods() const;

    bool saveToFile();
    void loadFromFile();
    void listCalculationMethods() const;

    // Calculate target calories
    double calculateTargetCalories() const;

    void addLog(const DietProfileLog& log);
    void saveLogsToFile();
    void loadLogsFromFile();

private:
    void createLog();  // Helper method to create a log entry with current profile data
    Gender m_gender;
    double m_height;
    double m_weight;
    int m_age;
    bool m_loaded;
    ActivityLevel m_activityLevel;
    const CalorieCalculationMethod* m_calculationMethod; // Pointer to selected method
    std::string m_filepath;

    // Array of available calorie calculation methods
    static const std::vector<CalorieCalculationMethod> calorieCalculationMethods;

    std::vector<DietProfileLog> m_logs;
};

#endif // DIET_GOAL_PROFILE_H