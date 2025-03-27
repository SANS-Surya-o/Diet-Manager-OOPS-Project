#include "DietGoalProfile.h"
#include <cmath>
#include <stdexcept>

DietGoalProfile::DietGoalProfile() 
    : m_gender(""),
      m_height(0),
      m_weight(0),
      m_age(0),
      m_activityLevel(ActivityLevel::SEDENTARY),
      m_calculationMethod(CalorieCalculationMethod::HARRIS_BENEDICT) {}

void DietGoalProfile::setGender(const std::string& gender) {
    if (gender != "male" && gender != "female") {
        throw std::invalid_argument("Gender must be 'male' or 'female'");
    }
    m_gender = gender;
}

void DietGoalProfile::setHeight(double height) {
    if (height <= 0) {
        throw std::invalid_argument("Height must be positive");
    }
    m_height = height;
}

void DietGoalProfile::setWeight(double weight) {
    if (weight <= 0) {
        throw std::invalid_argument("Weight must be positive");
    }
    m_weight = weight;
}

void DietGoalProfile::setAge(int age) {
    if (age <= 0) {
        throw std::invalid_argument("Age must be positive");
    }
    m_age = age;
}

void DietGoalProfile::setActivityLevel(ActivityLevel level) {
    m_activityLevel = level;
}

void DietGoalProfile::setCalorieCalculationMethod(CalorieCalculationMethod method) {
    m_calculationMethod = method;
}

std::string DietGoalProfile::getGender() const { return m_gender; }
double DietGoalProfile::getHeight() const { return m_height; }
double DietGoalProfile::getWeight() const { return m_weight; }
int DietGoalProfile::getAge() const { return m_age; }
ActivityLevel DietGoalProfile::getActivityLevel() const { return m_activityLevel; }
CalorieCalculationMethod DietGoalProfile::getCalorieCalculationMethod() const { return m_calculationMethod; }

double DietGoalProfile::calculateTargetCalories() const {
    switch (m_calculationMethod) {
        case CalorieCalculationMethod::HARRIS_BENEDICT:
            return calculateHarrisBenedictCalories();
        case CalorieCalculationMethod::MIFFLIN_ST_JEOR:
            return calculateMifflinStJeorCalories();
        default:
            throw std::runtime_error("Invalid calorie calculation method");
    }
}

double DietGoalProfile::calculateHarrisBenedictCalories() const {
    double baseCalories;
    if (m_gender == "male") {
        baseCalories = 88.362 + (13.397 * m_weight) + (4.799 * m_height) - (5.677 * m_age);
    } else {
        baseCalories = 447.593 + (9.247 * m_weight) + (3.098 * m_height) - (4.330 * m_age);
    }

    // Apply activity level multiplier
    switch (m_activityLevel) {
        case ActivityLevel::SEDENTARY: return baseCalories * 1.2;
        case ActivityLevel::LIGHTLY_ACTIVE: return baseCalories * 1.375;
        case ActivityLevel::MODERATELY_ACTIVE: return baseCalories * 1.55;
        case ActivityLevel::VERY_ACTIVE: return baseCalories * 1.725;
        case ActivityLevel::EXTRA_ACTIVE: return baseCalories * 1.9;
        default: return baseCalories * 1.2;
    }
}

double DietGoalProfile::calculateMifflinStJeorCalories() const {
    double baseCalories;
    if (m_gender == "male") {
        baseCalories = (10 * m_weight) + (6.25 * m_height) - (5 * m_age) + 5;
    } else {
        baseCalories = (10 * m_weight) + (6.25 * m_height) - (5 * m_age) - 161;
    }

    // Apply activity level multiplier
    switch (m_activityLevel) {
        case ActivityLevel::SEDENTARY: return baseCalories * 1.2;
        case ActivityLevel::LIGHTLY_ACTIVE: return baseCalories * 1.375;
        case ActivityLevel::MODERATELY_ACTIVE: return baseCalories * 1.55;
        case ActivityLevel::VERY_ACTIVE: return baseCalories * 1.725;
        case ActivityLevel::EXTRA_ACTIVE: return baseCalories * 1.9;
        default: return baseCalories * 1.2;
    }
}