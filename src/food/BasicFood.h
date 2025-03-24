#ifndef BASIC_FOOD_H
#define BASIC_FOOD_H

#include "Food.h"

/**
 * @brief Class representing a basic food item with calories
 */
class BasicFood : public Food {
public:
    /**
     * @brief Constructor for BasicFood
     * @param id The unique identifier for the food
     * @param keywords List of search keywords for this food
     * @param caloriesPerServing Calories per serving of this food
     */
    BasicFood(const std::string& id, const std::vector<std::string>& keywords, double caloriesPerServing);
    
    /**
     * @brief Get calories per serving
     * @return Calories per serving
     */
    double getCaloriesPerServing() const override;
    
    /**
     * @brief Set calories per serving
     * @param calories The new calories per serving value
     */
    void setCaloriesPerServing(double calories);
    
    /**
     * @brief Convert the food to a string representation for saving to file
     * @return String representation of the food
     */
    std::string toString() const override;
    
    /**
     * @brief Create a BasicFood object from a string representation
     * @param str String representation of the BasicFood
     * @return Shared pointer to a new BasicFood object
     */
    static std::shared_ptr<BasicFood> fromString(const std::string& str);
    
    /**
     * @brief Display food information
     */
    void display() const override;

private:
    double caloriesPerServing; // Calories per serving of this food
};

#endif // BASIC_FOOD_H