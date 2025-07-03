# YADA - Yet Another Diet Assistant

This is a prototype implementation of the YADA diet management system for DASS Assignment 3.
Taught me about the use of good design principles, and some OOPS concepts in C++

## Project Structure

The project is organized as follows:

```
YADA/
├── src/                  # Source files
│   ├── main.cpp          # Main program entry point
│   ├── food/             # Food-related classes
│   │   ├── Food.h        # Base Food class
│   │   ├── Food.cpp
│   │   ├── BasicFood.h   # Basic food implementation
│   │   ├── BasicFood.cpp
│   │   ├── CompositeFood.h # Composite food implementation
│   │   └── CompositeFood.cpp
│   ├── database/         # Database management
│   │   ├── FoodDatabase.h
│   │   └── FoodDatabase.cpp
│   ├── utils/            # Utility functions
│   │   ├── FileHandler.h
│   │   └── FileHandler.cpp
│   ├── DietGoalProfile.h # User profile and calorie calculation
│   ├── DietGoalProfile.cpp
│   ├── DailyLog.h        # Daily food log management
│   └── DailyLog.cpp
├── data/                 # Data files
│   ├── basic_foods.txt   # Database of basic foods
│   ├── composite_foods.txt # Database of composite foods
│   └── daily_logs.txt    # Daily food consumption logs
└── CMakeLists.txt        # Build configuration
```

## Building the Project

### Prerequisites

- CMake 3.10 or higher
- C++14 compatible compiler (GCC, Clang, MSVC)

### Build Instructions

1. Create a build directory:
   ```
   mkdir build
   cd build
   ```

2. Configure and build:
   ```
   cmake ..
   cmake --build .
   ```

3. Run the application:
   ```
   ./yada
   ```

## Features Implemented

### Food Database
- Basic Foods with ID, keywords, and calories
- Composite Foods made from basic foods and other composite foods
- Text file storage in human-readable format
- Save/load database functionality
- Ability to add new basic and composite foods
- Keyword-based food search with ANY/ALL matching options

### Daily Logs
- Track food consumption by date
- Add and remove food entries
- Calculate total daily calories
- Persistent log storage
- Undo functionality for log modifications

### Diet Goal Profile
- User profile management (gender, height, weight, age)
- Multiple activity level settings
- Two calorie calculation methods:
  1. Harris-Benedict Equation
  2. Mifflin-St Jeor Equation
- Dynamic target calorie calculation based on user profile

## Design Patterns Used

1. **Composite Pattern**: Used for representing basic and composite foods with a common interface.
2. **Factory Method**: Used for creating food objects from text strings.
3. **Observer Pattern**: Can be implemented for notifying parts of the system when foods are added or modified.
4. **Strategy Pattern**: Implemented in calorie calculation methods, allowing easy addition of new calculation approaches

## Extension Points

The design allows for easy extension in several areas:

1. **Web Data Sources**: New sources for basic food information can be added by implementing adapters that convert from external data formats to the BasicFood format.
2. **Nutrition Information**: The Food class can be easily extended to store additional nutritional information beyond calories.
3. **Persistence Formats**: The storage format can be changed by modifying the toString/fromString methods or implementing new serialization strategies.
4. **Calorie Calculation**: New calorie calculation methods can be easily added to the DietGoalProfile class.

## Usage Examples

### Adding a Basic Food
1. Select option 1 from the main menu
2. Enter a unique ID for the food
3. Enter a comma-separated list of keywords
4. Enter the calories per serving

### Creating a Composite Food
1. Select option 2 from the main menu
2. Enter a unique ID for the composite food
3. Enter a comma-separated list of keywords
4. Select component foods and specify the number of servings for each

### Managing Daily Logs
1. Select option 2 from the main menu
2. Choose to add, view, or remove food entries
3. Entries are tracked by date
4. Undo functionality allows reverting recent changes

### Managing Diet Goal Profile
1. Select option 3 from the main menu
2. Set personal information (gender, height, weight, age)
3. Choose activity level
4. Select calorie calculation method
5. Calculate target daily calories

### Answers to the questions in the document 
1. Expand the food database to include nutri
