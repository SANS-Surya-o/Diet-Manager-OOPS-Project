# YADA - Yet Another Diet Assistant

This is a prototype implementation of the YADA diet management system for DASS Assignment 3.

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
│   └── utils/            # Utility functions
│       ├── FileHandler.h
│       └── FileHandler.cpp
├── data/                 # Data files
│   ├── basic_foods.txt   # Database of basic foods
│   └── composite_foods.txt # Database of composite foods
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

## Design Patterns Used

1. **Composite Pattern**: Used for representing basic and composite foods with a common interface.
2. **Factory Method**: Used for creating food objects from text strings.
3. **Observer Pattern**: Can be implemented for notifying parts of the system when foods are added or modified.

## Extension Points

The design allows for easy extension in several areas:

1. **Web Data Sources**: New sources for basic food information can be added by implementing adapters that convert from external data formats to the BasicFood format.
2. **Nutrition Information**: The Food class can be easily extended to store additional nutritional information beyond calories.
3. **Persistence Formats**: The storage format can be changed by modifying the toString/fromString methods or implementing new serialization strategies.

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

### Searching for Foods
1. Select option 3 from the main menu
2. Enter search keywords
3. Specify whether to match ANY or ALL keywords