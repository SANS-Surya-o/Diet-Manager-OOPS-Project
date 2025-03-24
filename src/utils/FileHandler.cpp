#include "FileHandler.h"
#include <fstream>
#include <sys/stat.h>
#include <iostream>

bool FileHandler::fileExists(const std::string& filePath) {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

bool FileHandler::createDirectoryIfNotExists(const std::string& dirPath) {
    struct stat info;
    
    // Check if the directory already exists
    if (stat(dirPath.c_str(), &info) == 0 && (info.st_mode & S_IFDIR)) {
        return true; // Directory already exists
    }
    
    // Create the directory
#ifdef _WIN32
    int result = _mkdir(dirPath.c_str()); // Windows
#else
    int result = mkdir(dirPath.c_str(), 0755); // Linux/Mac
#endif
    
    if (result == 0) {
        return true; // Directory created successfully
    } else {
        std::cerr << "Error creating directory: " << dirPath << std::endl;
        return false;
    }
}

std::vector<std::string> FileHandler::readAllLines(const std::string& filePath) {
    std::vector<std::string> lines;
    std::ifstream file(filePath);
    
    if (!file) {
        std::cerr << "Could not open file for reading: " << filePath << std::endl;
        return lines;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

bool FileHandler::writeAllLines(const std::string& filePath, const std::vector<std::string>& lines) {
    std::ofstream file(filePath);
    
    if (!file) {
        std::cerr << "Could not open file for writing: " << filePath << std::endl;
        return false;
    }
    
    for (const auto& line : lines) {
        file << line << std::endl;
    }
    
    return true;
}