#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>
#include <memory>

/**
 * @brief Abstract base class for all food items in the system
 */
class Food {
public:
    /**
     * @brief Constructor for Food
     * @param id The unique identifier for the food
     * @param keywords List of search keywords for this food
     */
    Food(const std::string& id, const std::vector<std::string>& keywords);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Food() = default;
    
    /**
     * @brief Get the identifier of the food
     * @return The food identifier
     */
    std::string getId() const;
    
    /**
     * @brief Get the search keywords for this food
     * @return Vector of keywords
     */
    std::vector<std::string> getKeywords() const;
    
    /**
     * @brief Add a keyword to this food
     * @param keyword The keyword to add
     */
    void addKeyword(const std::string& keyword);
    
    /**
     * @brief Check if the food matches all the given keywords
     * @param keywords List of keywords to match
     * @return true if all keywords match, false otherwise
     */
    bool matchesAllKeywords(const std::vector<std::string>& keywords) const;
    
    /**
     * @brief Check if the food matches any of the given keywords
     * @param keywords List of keywords to match
     * @return true if any keyword matches, false otherwise
     */
    bool matchesAnyKeyword(const std::vector<std::string>& keywords) const;
    
    /**
     * @brief Get calories per serving
     * @return Calories per serving
     */
    virtual double getCaloriesPerServing() const = 0;
    
    /**
     * @brief Convert the food to a string representation for saving to file
     * @return String representation of the food
     */
    virtual std::string toString() const = 0;
    
    /**
     * @brief Display food information
     */
    virtual void display() const = 0;

    /**
     * @brief Get the name (ID) of the food
     * @return Food name
     */
    std::string getName() const { return id; }

protected:
    std::string id;                    // Unique identifier
    std::vector<std::string> keywords; // Search keywords
};

#endif // FOOD_H