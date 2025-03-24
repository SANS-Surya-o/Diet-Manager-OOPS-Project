#ifndef FOOD_DATABASE_H
#define FOOD_DATABASE_H

#include "../food/Food.h"
#include "../food/BasicFood.h"
#include "../food/CompositeFood.h"
#include <map>
#include <string>
#include <memory>
#include <vector>

/**
 * @brief Class for managing the food database
 */
class FoodDatabase {
public:
    /**
     * @brief Constructor for FoodDatabase
     * @param basicFoodFilePath Path to the basic foods database file
     * @param compositeFoodFilePath Path to the composite foods database file
     */
    FoodDatabase(const std::string& basicFoodFilePath, const std::string& compositeFoodFilePath);
    
    /**
     * @brief Destructor for FoodDatabase
     */
    ~FoodDatabase();
    
    /**
     * @brief Load foods from database files
     * @return true if loading was successful, false otherwise
     */
    bool loadFoods();
    
    /**
     * @brief Save foods to database files
     * @return true if saving was successful, false otherwise
     */
    bool saveFoods();
    
    /**
     * @brief Add a basic food to the database
     * @param food Shared pointer to the basic food
     * @return true if food was added successfully, false if a food with the same ID already exists
     */
    bool addBasicFood(std::shared_ptr<BasicFood> food);
    
    /**
     * @brief Add a composite food to the database
     * @param food Shared pointer to the composite food
     * @return true if food was added successfully, false if a food with the same ID already exists
     */
    bool addCompositeFood(std::shared_ptr<CompositeFood> food);
    
    /**
     * @brief Get a food by ID
     * @param id The ID of the food to get
     * @return Shared pointer to the food, or nullptr if not found
     */
    std::shared_ptr<Food> getFoodById(const std::string& id) const;
    
    /**
     * @brief Find foods matching all the given keywords
     * @param keywords List of keywords to match
     * @return Vector of foods matching all keywords
     */
    std::vector<std::shared_ptr<Food>> findFoodsMatchingAllKeywords(const std::vector<std::string>& keywords) const;
    
    /**
     * @brief Find foods matching any of the given keywords
     * @param keywords List of keywords to match
     * @return Vector of foods matching any keyword
     */
    std::vector<std::shared_ptr<Food>> findFoodsMatchingAnyKeyword(const std::vector<std::string>& keywords) const;
    
    /**
     * @brief Get all foods in the database
     * @return Vector of all foods
     */
    std::vector<std::shared_ptr<Food>> getAllFoods() const;
    
    /**
     * @brief Display all foods in the database
     */
    void displayAllFoods() const;

private:
    std::string basicFoodFilePath;    // Path to basic foods database file
    std::string compositeFoodFilePath; // Path to composite foods database file
    
    std::map<std::string, std::shared_ptr<Food>> foods; // Map of food IDs to Food objects
    
    /**
     * @brief Load basic foods from database file
     * @return true if loading was successful, false otherwise
     */
    bool loadBasicFoods();
    
    /**
     * @brief Load composite foods from database file
     * @return true if loading was successful, false otherwise
     */
    bool loadCompositeFoods();
    
    /**
     * @brief Save basic foods to database file
     * @return true if saving was successful, false otherwise
     */
    bool saveBasicFoods();
    
    /**
     * @brief Save composite foods to database file
     * @return true if saving was successful, false otherwise
     */
    bool saveCompositeFoods();
};

#endif // FOOD_DATABASE_H