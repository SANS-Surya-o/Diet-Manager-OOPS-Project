#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>

/**
 * @brief Utility class for file operations
 */
class FileHandler {
public:
    /**
     * @brief Check if a file exists
     * @param filePath Path to the file
     * @return true if the file exists, false otherwise
     */
    static bool fileExists(const std::string& filePath);
    
    /**
     * @brief Create directory if it doesn't exist
     * @param dirPath Path to the directory
     * @return true if directory exists or was created successfully, false otherwise
     */
    static bool createDirectoryIfNotExists(const std::string& dirPath);
    
    /**
     * @brief Read all lines from a file
     * @param filePath Path to the file
     * @return Vector of strings, each representing a line from the file
     */
    static std::vector<std::string> readAllLines(const std::string& filePath);
    
    /**
     * @brief Write all lines to a file
     * @param filePath Path to the file
     * @param lines Vector of strings to write
     * @return true if writing was successful, false otherwise
     */
    static bool writeAllLines(const std::string& filePath, const std::vector<std::string>& lines);
};

#endif // FILE_HANDLER_H