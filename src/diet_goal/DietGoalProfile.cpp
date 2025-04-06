#include "DietGoalProfile.h"
#include <cmath>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>

const std::vector<CalorieCalculationMethod> DietGoalProfile::calorieCalculationMethods = {
    CalorieCalculationMethod("Harris-Benedict", [](Gender gender, double weight, double height, int age) {
        double baseCalories = (gender == Gender::MALE)
            ? 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age)
            : 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
        return baseCalories;
    }),
    CalorieCalculationMethod("Mifflin-St Jeor", [](Gender gender, double weight, double height, int age) {
        double baseCalories = (gender == Gender::MALE)
            ? (10 * weight) + (6.25 * height) - (5 * age) + 5
            : (10 * weight) + (6.25 * height) - (5 * age) - 161;
        return baseCalories;
    })
};

bool DietGoalProfile::loaded() {
    return m_loaded;
}

DietGoalProfile::DietGoalProfile(std::string filepath) {
    m_filepath = filepath;
    m_loaded = false;
    m_gender = Gender::UNDEFINED;
    m_height = 0;
    m_weight = 0;
    m_age = 0;
    m_activityLevel = ActivityLevel::UNDEFINED;
    m_calculationMethod = nullptr;
}

void DietGoalProfile::setGender(Gender gender) {
    if (gender != Gender::MALE && gender != Gender::FEMALE) {
        throw std::invalid_argument("Gender must be 'male' or 'female'");
    }
    m_gender = gender;
    if (m_loaded) createLog();
}

void DietGoalProfile::setHeight(double height) {
    if (height <= 0) {
        throw std::invalid_argument("Height must be positive");
    }
    m_height = height;
    if (m_loaded) createLog();
}

void DietGoalProfile::setWeight(double weight) {
    if (weight <= 0) {
        throw std::invalid_argument("Weight must be positive");
    }
    m_weight = weight;
    if (m_loaded) createLog();
}

void DietGoalProfile::setAge(int age) {
    if (age <= 0) {
        throw std::invalid_argument("Age must be positive");
    }
    m_age = age;
    if (m_loaded) createLog();
}

void DietGoalProfile::setActivityLevel(ActivityLevel level) {
    if (level < ActivityLevel::SEDENTARY || level > ActivityLevel::EXTRA_ACTIVE) {
        throw std::invalid_argument("Activity level must be between SEDENTARY and EXTRA ACTIVE");
    }
    m_activityLevel = level;
    if (m_loaded) createLog();
}

void DietGoalProfile::setCalorieCalculationMethod(int idx) {
    m_calculationMethod = &calorieCalculationMethods[idx-1];
    if (m_loaded) createLog();
}

void DietGoalProfile::createLog() {
    if (!m_calculationMethod) {
        throw std::runtime_error("Cannot create log: Calorie calculation method not set");
    }
    
    auto now = std::time(nullptr);
    char dateBuffer[11];
    std::strftime(dateBuffer, sizeof(dateBuffer), "%d-%m-%Y", std::localtime(&now));
    std::string date(dateBuffer);
    
    int methodIdx = m_calculationMethod - &calorieCalculationMethods[0] + 1;
    m_logs.emplace_back(date, m_age, m_weight, m_activityLevel, methodIdx);
}

Gender DietGoalProfile::getGender() const { return m_gender; }
double DietGoalProfile::getHeight() const { return m_height; }
double DietGoalProfile::getWeight() const { return m_weight; }
int DietGoalProfile::getAge() const { return m_age; }
ActivityLevel DietGoalProfile::getActivityLevel() const { return m_activityLevel; }
const CalorieCalculationMethod& DietGoalProfile::getCalorieCalculationMethod() const {
    if (!m_calculationMethod) {
        throw std::runtime_error("Calorie calculation method not set");
    }
    return *m_calculationMethod;
}

double DietGoalProfile::calculateTargetCalories() const {
    if (!m_calculationMethod) {
        throw std::runtime_error("Calorie calculation method not set");
    }
    double baseCalories = m_calculationMethod->calculate(m_gender, m_weight, m_height, m_age);

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

bool DietGoalProfile::saveToFile() {
    if(!m_loaded) return true;
    std::ofstream outFile(m_filepath);
    if (!outFile) {
        throw std::runtime_error("Failed to open file for writing: " + m_filepath);
        return false;
    }

    outFile << "# User info database\n# log format: DD-MM-YYYY:age:weight:activitylevel:method\n"
            << "gender:" << (m_gender == Gender::MALE ? "M" : "F") << '\n'
            << "height:" << m_height << '\n';

    // Save logs
    DietProfileLog* prevLog = nullptr;
    for (const auto& log : m_logs) {
        outFile << log.date << ":";
        if (!prevLog || log.age != prevLog->age) outFile << log.age;
        outFile << ":";
        if (!prevLog || log.weight != prevLog->weight) outFile << log.weight;
        outFile << ":";
        if (!prevLog || log.activityLevel != prevLog->activityLevel) outFile << static_cast<int>(log.activityLevel);
        outFile << ":";
        if (!prevLog || log.calorieCalculationMethodIdx != prevLog->calorieCalculationMethodIdx) outFile << log.calorieCalculationMethodIdx;
        outFile << "\n";

        prevLog = const_cast<DietProfileLog*>(&log);
    }

    outFile.close();
    return true;
}

void DietGoalProfile::initializeProfileFromUser() {
        std::string gender;
        std::cout << "Enter your gender (M/F): ";
        std::cin >> gender;
        setGender(gender == "M" ? Gender::MALE : Gender::FEMALE);

        double height;
        std::cout << "Enter your height (in cm): ";
        while (true) {
            try {
                std::cin >> height;
                setHeight(height);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid height. " << e.what() << " Please enter again: ";
            }
        }

        std::cout << "What's your activity level? (1: Sedentary, 2: Lightly Active, 3: Moderately Active, 4: Very Active, 5: Extra Active): ";
        int activityLevel;
        while (true) {
            try {
                std::cin >> activityLevel;
                setActivityLevel(static_cast<ActivityLevel>(activityLevel - 1));
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid activity level. " << e.what() << " Please enter again: ";
            }
        }

        std::cout << "Enter your weight (in kg): ";
        double weight;
        std::cin >> weight;
        setWeight(weight);

        std::cout << "Enter your age: ";
        int age;
        std::cin >> age;
        setAge(age);

        std::cout << "Enter your preferred calorie calculation method: " << std::endl;
        listCalculationMethods();
        int methodChoice;
        std::cin >> methodChoice;
        while (methodChoice < 1 and methodChoice > calorieCalculationMethods.size()) {
            std::cout << "Invalid choice." << std::endl;
            std::cin >> methodChoice;
        }
        setCalorieCalculationMethod(methodChoice);

        // Create a log entry after all data is collected
        createLog();

        m_loaded = true;
}

void DietGoalProfile::listCalculationMethods() const {
    for (size_t i = 0; i < calorieCalculationMethods.size(); ++i) {
        std::cout << i + 1 << ": " << calorieCalculationMethods[i].title() << std::endl;
    }
}

int DietGoalProfile::numberOfCalculationMethods() const {
    return calorieCalculationMethods.size();
}

void DietGoalProfile::loadFromFile() {
    std::cout << "Loading diet profile..." << std::endl;
    std::ifstream inFile(m_filepath);
    if (!inFile) {
        // Initialize the file if it doesn't exist
        std::ofstream outFile(m_filepath);
        if (!outFile) {
            throw std::runtime_error("Failed to create file: " + m_filepath);
        }
        std::cout << "File not found. Creating a new profile..." << std::endl;

        initializeProfileFromUser();

        outFile << "# User info database\n"
                << "# log format: DD-MM-YYYY:age:weight:activitylevel:method\n"
                << "gender:" << (m_gender == Gender::MALE ? "M" : "F") << "\n"
                << "height:" << m_height << "\n";
        outFile.close();
        return;
    }

    // Validate the first 4 lines
    std::string line;
    std::getline(inFile, line);
    if (line != "# User info database") {
        throw std::runtime_error("Invalid file format: Missing header");
    }

    std::getline(inFile, line);
    if (line != "# log format: DD-MM-YYYY:age:weight:activitylevel:method") {
        throw std::runtime_error("Invalid file format: Missing log format description");
    }

    std::getline(inFile, line);
    if (line.rfind("gender:", 0) != 0) {
        throw std::runtime_error("Invalid file format: Missing gender");
    }
    setGender(line.substr(7, 1) == "M" ? Gender::MALE : Gender::FEMALE); // Extract gender after "gender:"

    std::getline(inFile, line);
    if (line.rfind("height:", 0) != 0) {
        throw std::runtime_error("Invalid file format: Missing height");
    }
    setHeight(std::stod(line.substr(7))); // Extract height after "height:"

    // Load logs
    std::string prevDate, date;
    int prevAge = -1, age;
    double prevWeight = -1, weight;
    ActivityLevel prevActivityLevel = ActivityLevel::UNDEFINED, activityLevel;
    int prevMethodIdx = -1, methodIdx;

    while (std::getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream logStream(line);
        std::string ageStr, weightStr, activityLevelStr, methodIdxStr;
        std::getline(logStream, date, ':');
        std::getline(logStream, ageStr, ':');
        std::getline(logStream, weightStr, ':');
        std::getline(logStream, activityLevelStr, ':');
        std::getline(logStream, methodIdxStr, ':');

        age = ageStr.empty() ? prevAge : std::stoi(ageStr);
        setAge(age);
        weight = weightStr.empty() ? prevWeight : std::stod(weightStr);
        setWeight(weight);
        activityLevel = activityLevelStr.empty() ? prevActivityLevel : static_cast<ActivityLevel>(std::stoi(activityLevelStr));
        setActivityLevel(activityLevel);
        methodIdx = methodIdxStr.empty() ? prevMethodIdx : std::stoi(methodIdxStr);
        setCalorieCalculationMethod(methodIdx);
        if (prevDate.empty() && (age == -1 || weight == -1 || activityLevel == ActivityLevel::UNDEFINED || methodIdx == -1)) {
            throw std::runtime_error("First day's parameters must all be specified");
        }

        // Create and add the log entry
        m_logs.emplace_back(date, age, weight, activityLevel, methodIdx);

        prevDate = date;
        prevAge = age;
        prevWeight = weight;
        prevActivityLevel = activityLevel;
        prevMethodIdx = methodIdx;
    }

    m_loaded = true;

    inFile.close();
}

void DietGoalProfile::addLog(const DietProfileLog& log) {
    m_logs.push_back(log);
}

double DietGoalProfile::calculateRemainingCalories(DailyLog &log) const {
    double targetCalories = calculateTargetCalories();
    double consumedCalories = log.getTotalCalories();
    return targetCalories - consumedCalories;
}

double CalorieCalculationMethod::calculate(Gender gender, double weight, double height, int age) const {
    // Calculates base calories.
    return m_calculationFunction(gender, weight, height, age);
}

CalorieCalculationMethod::CalorieCalculationMethod(std::string title, std::function<double(Gender, double, double, int)> calculationFunction)
    : m_title(title), m_calculationFunction(calculationFunction) {}
