#ifndef SDCARD_H
#define SDCARD_H
#include "esp_err.h"


class SDCard {

public:
  static esp_err_t mountCard(const char* rootPath);
  static esp_err_t close();

  static size_t getFileSize(const char *filePath);

  static size_t readFile(const char* path, void* ptr);
  static char* writeFile(const char* path, const char* fileName);
};


#endif