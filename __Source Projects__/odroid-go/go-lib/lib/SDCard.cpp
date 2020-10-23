#include "SDCard.h"



#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_heap_caps.h"
#include "esp_spiffs.h"

#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define SD_PIN_NUM_MISO 19
#define SD_PIN_NUM_MOSI 23
#define SD_PIN_NUM_CLK  18
#define SD_PIN_NUM_CS 22


static bool isMounted = false;

esp_err_t SDCard::mountCard(const char* rootPath) {
  esp_err_t espError;

  if (isMounted) {
    printf("%s: %s Card Already Mounted!\n", __func__, rootPath);
    return ESP_FAIL;
  }

  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = VSPI_HOST; // HSPI_HOST;

  //host.max_freq_khz = SDMMC_FREQ_HIGHSPEED; //10000000;
  host.max_freq_khz = SDMMC_FREQ_DEFAULT;

  sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
  slot_config.gpio_miso = (gpio_num_t)SD_PIN_NUM_MISO;
  slot_config.gpio_mosi = (gpio_num_t)SD_PIN_NUM_MOSI;
  slot_config.gpio_sck  = (gpio_num_t)SD_PIN_NUM_CLK;
  slot_config.gpio_cs = (gpio_num_t)SD_PIN_NUM_CS;
  //slot_config.dma_channel = 2;

  // Options for mounting the filesystem.
  // If format_if_mount_failed is set to true, SD card will be partitioned and
  // formatted in case when mounting fails.
  esp_vfs_fat_sdmmc_mount_config_t mount_config;
  memset(&mount_config, 0, sizeof(mount_config));

  mount_config.format_if_mount_failed = false;
  mount_config.max_files = 5;


  // Use settings defined above to initialize SD card and mount FAT filesystem.
  // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
  // Please check its source code and implement error recovery when developing
  // production applications.
  sdmmc_card_t* card;
  espError = esp_vfs_fat_sdmmc_mount(rootPath, &host, &slot_config, &mount_config, &card);

  if (espError == ESP_OK) {
    isMounted = true;
  }
  else     {
    printf("%s esp_vfs_fat_sdmmc_mount failed (%d)\n", __func__, espError);
  }

  return espError;
}


esp_err_t SDCard::close() {
  esp_err_t result;

  if (!isMounted) {
    printf("%s: not open.\n", __func__);
    return ESP_FAIL;
  }

  result = esp_vfs_fat_sdmmc_unmount();

  if (result != ESP_OK) {
    printf("%s: esp_vfs_fat_sdmmc_unmount failed (%d)\n", __func__, result);
  }

  isMounted = false;

  return result;
}


size_t SDCard::getFileSize(const char* path) {
  size_t size = 0;

  if (! isMounted) {
    printf("%s: %s not open.\n", __func__, path);
    return -1;
  }

  FILE* f = fopen(path, "rb");

  if (f == NULL) {
    printf("%s: %s fopen failed.\n", __func__, path);
  }
  else {
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
  }

  return size;
}

size_t SDCard::readFile(const char* path, void* ptr) {
  size_t size = 0;

  if (!isMounted)   {
    printf("odroid_sdcard_copy_file_to_memory: not open.\n");
    return 0;
  }

  if (!ptr){
    printf("odroid_sdcard_copy_file_to_memory: ptr is null.\n");
  }
  else {
    FILE* f = fopen(path, "rb");

    if (f == NULL) {
      printf("odroid_sdcard_copy_file_to_memory: fopen failed.\n");
    }
    else {
      // copy
      const size_t BLOCK_SIZE = 512;
      
      while(true) {
        __asm__("memw");
        size_t count = fread((uint8_t*)ptr + size, 1, BLOCK_SIZE, f);
        __asm__("memw");

        size += count;

        if (count < BLOCK_SIZE) break;
      }
    }
  }

  return size;
}

char* SDCard::writeFile(const char* filePath, const char* fileName) {
  char* result = NULL;

  // Todo: this should not be so critical!
  if (!filePath) abort();
  if (!fileName) abort();

  //printf("%s: filePath='%s', fileName='%s'\n", __func__, filePath, fileName);

  // Determine folder
  char* extension = const_cast<char *>(fileName) + strlen(fileName); // place at NULL terminator
  while (extension != fileName) {
    if (*extension == '.'){
      ++extension;
      break;
    }

    --extension;
  }

  if (extension == fileName) {
    printf("%s: File extention not found.\n", __func__);
    abort();
  }

  //printf("%s: extension='%s'\n", __func__, extension);

  const char* DATA_PATH = "/odroid/data/";
  const char* SAVE_EXTENSION = ".sav";

  size_t savePathLength = strlen(filePath) + strlen(DATA_PATH) + strlen(extension) + 1 + strlen(fileName) + strlen(SAVE_EXTENSION) + 1;
  char* savePath = (char*)malloc(savePathLength);
  
  if (savePath) {
    strcpy(savePath, filePath);
    strcat(savePath, DATA_PATH);
    strcat(savePath, extension);
    strcat(savePath, "/");
    strcat(savePath, fileName);
    strcat(savePath, SAVE_EXTENSION);

    printf("%s: savefile_path='%s'\n", __func__, savePath);

    result = savePath;
  }

  return result;
}