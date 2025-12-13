#ifndef FILESYSTEM_MANAGER_H
#define FILESYSTEM_MANAGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include <vector>

#define MAX_IMAGE_SIZE (512 * 1024)  // 512KB max per image
#define MAX_IMAGES 50

struct ImageInfo {
    String filename;
    size_t size;
    time_t uploaded;
    bool temporary;
};

class FilesystemManager {
public:
    bool begin();
    bool saveImage(const uint8_t* data, size_t len, const String& filename, bool temporary = false);
    bool deleteImage(const String& filename);
    std::vector<ImageInfo> listImages();
    bool imageExists(const String& filename);
    size_t getImageSize(const String& filename);
    size_t getFreeSpace();
    size_t getTotalSpace();
    bool isValidImageType(const uint8_t* data, size_t len);
    
private:
    bool initialized = false;
    String getImagePath(const String& filename);
};

extern FilesystemManager fsManager;

#endif
