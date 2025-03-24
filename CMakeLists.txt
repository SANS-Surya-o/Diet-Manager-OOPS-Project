cmake_minimum_required(VERSION 3.10)
project(YADA)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add source files
set(SOURCES
    src/main.cpp
    src/food/Food.cpp
    src/food/BasicFood.cpp
    src/food/CompositeFood.cpp
    src/database/FoodDatabase.cpp
    src/utils/FileHandler.cpp
)

# Create executable
add_executable(yada ${SOURCES})

# Create data directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

# Copy sample data files if they don't exist
if(NOT EXISTS ${CMAKE_BINARY_DIR}/data/basic_foods.txt)
    file(WRITE ${CMAKE_BINARY_DIR}/data/basic_foods.txt
        "# Basic Foods Database\n"
        "# Format: BASIC:id:keyword1,keyword2,...:calories\n"
        "BASIC:apple:fruit,fresh,sweet:52\n"
        "BASIC:banana:fruit,fresh,sweet,tropical:89\n"
        "BASIC:chicken_breast:meat,protein,lean:165\n"
        "BASIC:bread_slice:grain,carb,wheat:80\n"
        "BASIC:milk:dairy,liquid,calcium:103\n"
        "BASIC:rice:grain,carb,white:200\n"
        "BASIC:egg:protein,breakfast:78\n"
        "BASIC:tomato:vegetable,red,fresh:22\n"
        "BASIC:lettuce:vegetable,green,fresh,salad:5\n"
        "BASIC:cheese_cheddar:dairy,protein,fat:113\n"
        "BASIC:peanut_butter:spread,protein,fat:188\n"
        "BASIC:jelly:spread,sweet,fruit:56\n"
    )
endif()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/data/composite_foods.txt)
    file(WRITE ${CMAKE_BINARY_DIR}/data/composite_foods.txt
        "# Composite Foods Database\n"
        "# Format: COMPOSITE:id:keyword1,keyword2,...:foodId=servings;foodId=servings;...\n"
        "COMPOSITE:pbj_sandwich:sandwich,lunch,protein:bread_slice=2;peanut_butter=1;jelly=1\n"
        "COMPOSITE:cereal_with_milk:breakfast,dairy,grain:cereal=1;milk=1\n"
    )
endif()

# Install target
install(TARGETS yada DESTINATION bin)