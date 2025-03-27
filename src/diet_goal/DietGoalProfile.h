#ifndef DIET_GOAL_PROFILE_H
#define DIET_GOAL_PROFILE_H

#include <string>
#include <memory>
#include <vector>

enum class ActivityLevel {
    SEDENTARY,
    LIGHTLY_ACTIVE,
    MODERATELY_ACTIVE,
    VERY_ACTIVE,
    EXTRA_ACTIVE
};

enum class CalorieCalculationMethod {
    HARRIS_BENEDICT,
    MIFFLIN_ST_JEOR
};

class DietGoalProfile {
public:
    DietGoalProfile();

    // Setters for profile information
    void setGender(const std::string& gender);
    void setHeight(double height);  // in cm
    void setWeight(double weight);  // in kg
    void setAge(int age);
    void setActivityLevel(ActivityLevel level);
    void setCalorieCalculationMethod(CalorieCalculationMethod method);

    // Getters
    std::string getGender() const;
    double getHeight() const;
    double getWeight() const;
    int getAge() const;
    ActivityLevel getActivityLevel() const;
    CalorieCalculationMethod getCalorieCalculationMethod() const;

    // Calculate target calories
    double calculateTargetCalories() const;

private:
    std::string m_gender;
    double m_height;
    double m_weight;
    int m_age;
    ActivityLevel m_activityLevel;
    CalorieCalculationMethod m_calculationMethod;

    // Private calculation methods for different calorie estimation approaches
    double calculateHarrisBenedictCalories() const;
    double calculateMifflinStJeorCalories() const;
};

#endif // DIET_GOAL_PROFILE_H