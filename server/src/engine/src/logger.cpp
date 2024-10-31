#include <string>
#include <fstream>
#include <iostream>
#include "logger.h"

const int DEBUG_MODE = 0;

void empty_file(const std::string& name) {
  if (!DEBUG_MODE) return;
  std::ofstream my_file;
  my_file.open(name, std::ofstream::out | std::ofstream::trunc);
  if (!my_file) {
    std::cout << "Failed to open file: " << name << std::endl;
  } else {
    std::cout << "Opened file: " << name << std::endl;
  }
  my_file.close();
}


void file_logger(const std::string& name, const std::string& content) {
  if (!DEBUG_MODE) return;
  std::ofstream my_file;
  my_file.open(name, std::ofstream::app);
  my_file << content << std::endl;
  my_file.close();
}

