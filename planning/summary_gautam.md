# Preferred languages
C++ then Java

# Misc
- DB file must be editable in plain text
- DB is loaded at program entry and saved at program exit.
- User should be able to save DB during program execution.
- on program start, read daily log. (log of all days)
- provide for undoing commands, upto an infinite depth. undo information need not persist among different instances of the app.
- new target intake algs must be addable seamlessly.
- The log file may grow to be quite large; for this reason, using approaches that reduce or eliminate 
duplicate copies of objects is highly desirable (this is a very broad hint).
- Initialize with database of around 12-24 basic foods
- have a readme.txt explaining how to run the program

# Design doc
- Short overview
- UML class diagram (no attributes, only methods)
- sequence diagrams
- explaining how you adhere to competing design principles
- explaining two strongest and weakest aspects of design

# Food
- Identifying string
- List of search keywords
- calories associated with one serving
- List of foods it's made from (multi-set?)
- composite food's identifier and keywords can be decided by user, calories are calculated

# FoodManager
- add basic food (`add <name>`)
- handle downloading data from websites and diet databases.
- scrape through the object to find required params.
- create composite food (`compose`) by selecting each basic component and quantity of each.
- add food to log by selecting food, number of servings.
- filter by keywords: `filter any <keywords>`, `filter all <keywords>`
- able to delete foods from log

# Diet goal profile
- Record user's gender, height, age, weight, acitivty level.
- age, weight and activity level must be changeable every day.
- default is to carry over previous day's information.
- algorithm to calculate target calorie intake: search web. implement at least 2.
- algorithm must be changeable.
- compute difference between target intake and calories consumed that day.