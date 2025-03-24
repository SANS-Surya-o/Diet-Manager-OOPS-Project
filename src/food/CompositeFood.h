#ifndef COMPOSITE_FOOD_H
#define COMPOSITE_FOOD_H

#include "Food.h"
#include <map>

/**
 * @brief Class representing a composite food made up of other foods
 */
class CompositeFood : public Food {
public:
    /**
     * @brief Constructor for CompositeFood
     * @param id The unique identifier for the food
     * @param keywords List of search keywords for this food
     */
    CompositeFood(const std::string& id, const std::vector<std::string>& keywords);
    
    /**
     * @brief Add a component food with specified servings
     * @param food Shared pointer to the component food
     * @param servings Number of servings of the component food
     */
    void addComponent(std::shared_ptr<Food> food, double servings);
    
    /**
     * @brief Get the component foods and their servings
     * @return Map of food pointers to servings
     */
    std::map<std::shared_ptr<Food>, double> getComponents() const;
    
    /**
     * @brief Get calories per serving
     * @return Calories per serving
     */
    double getCaloriesPerServing() const override;
    
    /**
     * @brief Convert the food to a string representation for saving to file
     * @return String representation of the food
     */
    std::string toString() const override;
    
    /**
     * @brief Create a CompositeFood object from a string representation and a food database
     * @param str String representation of the CompositeFood
     * @param foodMap Map of food IDs to Food objects
     * @return Shared pointer to a new CompositeFood object
     */
    static std::shared_ptr<CompositeFood> fromString(const std::string& str, 
                                                    const std::map<std::string, std::shared_ptr<Food>>& foodMap);
    
    /**
     * @brief Display food information
     */
    void display() const override;

private:
    std::map<std::shared_ptr<Food>, double> components; // Map of component foods to servings
};

#endif // COMPOSITE_FOOD_H