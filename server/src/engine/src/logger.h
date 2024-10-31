#include <string>

/**
*
*
* @brief Clear the file with the following name
*
*/
void empty_file(const std::string& name);

/**
*
*
* @brief Appends content to file with name (with endline included)
*
*/
void file_logger(const std::string& name, const std::string& content);
void file_logger(const std::string& name, int16_t content);

