#include "filesystem_manager.h"

FilesystemManager fsManager;

bool FilesystemManager::begin() {
    if (!initialized) {
        initialized = LittleFS.begin(true);
        if (initialized) {
            Serial.println("LittleFS initialized successfully");
        } else {
            Serial.println("LittleFS initialization failed");
        }
    }
    return initialized;
}

String FilesystemManager::getImagePath(const String& filename) {
    return "/images/" + filename;
}

bool FilesystemManager::saveImage(const uint8_t* data, size_t len, const String& filename, bool temporary) {
    if (!initialized || len > MAX_IMAGE_SIZE) {
        Serial.printf("Save failed: initialized=%d, size=%d\n", initialized, len);
        return false;
    }
    
    // Validate image type
    if (!isValidImageType(data, len)) {
        Serial.println("Invalid image type");
        return false;
    }
    
    // Create images directory if it doesn't exist
    if (!LittleFS.exists("/images")) {
        LittleFS.mkdir("/images");
    }
    
    String path = getImagePath(filename);
    File file = LittleFS.open(path, "w");
    if (!file) {
        Serial.printf("Failed to open file for writing: %s\n", path.c_str());
        return false;
    }
    
    size_t written = file.write(data, len);
    file.close();
    
    if (written != len) {
        Serial.printf("Write size mismatch: %d != %d\n", written, len);
        return false;
    }
    
    Serial.printf("Image saved: %s (%d bytes)\n", filename.c_str(), len);
    return true;
}

bool FilesystemManager::deleteImage(const String& filename) {
    if (!initialized) return false;
    
    String path = getImagePath(filename);
    if (LittleFS.exists(path)) {
        return LittleFS.remove(path);
    }
    return false;
}

std::vector<ImageInfo> FilesystemManager::listImages() {
    std::vector<ImageInfo> images;
    if (!initialized) return images;
    
    if (!LittleFS.exists("/images")) {
        return images;
    }
    
    File root = LittleFS.open("/images");
    if (!root || !root.isDirectory()) {
        return images;
    }
    
    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            ImageInfo info;
            info.filename = String(file.name());
            info.size = file.size();
            info.uploaded = file.getLastWrite();
            info.temporary = false;
            images.push_back(info);
        }
        file = root.openNextFile();
    }
    
    return images;
}

bool FilesystemManager::imageExists(const String& filename) {
    if (!initialized) return false;
    String path = getImagePath(filename);
    return LittleFS.exists(path);
}

size_t FilesystemManager::getImageSize(const String& filename) {
    if (!initialized) return 0;
    
    String path = getImagePath(filename);
    File file = LittleFS.open(path, "r");
    if (!file) return 0;
    
    size_t size = file.size();
    file.close();
    return size;
}

size_t FilesystemManager::getFreeSpace() {
    if (!initialized) return 0;
    return LittleFS.totalBytes() - LittleFS.usedBytes();
}

size_t FilesystemManager::getTotalSpace() {
    if (!initialized) return 0;
    return LittleFS.totalBytes();
}

bool FilesystemManager::isValidImageType(const uint8_t* data, size_t len) {
    if (len < 4) return false;
    
    // Check for JPEG magic bytes (FF D8 FF)
    if (data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF) {
        return true;
    }
    
    // Check for PNG magic bytes (89 50 4E 47)
    if (data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47) {
        return true;
    }
    
    // Check for BMP magic bytes (42 4D)
    if (data[0] == 0x42 && data[1] == 0x4D) {
        return true;
    }
    
    return false;
}
