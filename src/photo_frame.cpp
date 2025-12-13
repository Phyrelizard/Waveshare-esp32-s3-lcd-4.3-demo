#include "photo_frame.h"
#include "filesystem_manager.h"
#include "ui/ui_photo_frame.h"

PhotoFrame photoFrame;

void PhotoFrame::begin() {
    loadImageList();
}

void PhotoFrame::update() {
    if (state != SLIDESHOW_PLAYING) {
        return;
    }
    
    unsigned long currentTime = millis();
    if (currentTime - lastTransitionTime >= (slideshowDuration * 1000)) {
        next();
    }
}

void PhotoFrame::loadImageList() {
    imageList.clear();
    std::vector<ImageInfo> images = fsManager.listImages();
    
    for (const auto& img : images) {
        imageList.push_back(img.filename);
    }
    
    Serial.printf("Loaded %d images for slideshow\n", imageList.size());
    
    if (shuffleEnabled) {
        shuffleImages();
    }
}

void PhotoFrame::shuffleImages() {
    // Simple shuffle algorithm
    for (size_t i = imageList.size() - 1; i > 0; i--) {
        size_t j = random(0, i + 1);
        String temp = imageList[i];
        imageList[i] = imageList[j];
        imageList[j] = temp;
    }
}

void PhotoFrame::start() {
    loadImageList();
    
    if (imageList.empty()) {
        Serial.println("No images available for slideshow");
        return;
    }
    
    state = SLIDESHOW_PLAYING;
    currentImageIndex = 0;
    lastTransitionTime = millis();
    
    showPhotoFrameScreen();
    displayCurrentImage();
    
    Serial.println("Slideshow started");
}

void PhotoFrame::stop() {
    state = SLIDESHOW_STOPPED;
    hidePhotoFrameScreen();
    Serial.println("Slideshow stopped");
}

void PhotoFrame::pause() {
    if (state == SLIDESHOW_PLAYING) {
        state = SLIDESHOW_PAUSED;
        Serial.println("Slideshow paused");
    }
}

void PhotoFrame::resume() {
    if (state == SLIDESHOW_PAUSED) {
        state = SLIDESHOW_PLAYING;
        lastTransitionTime = millis();
        Serial.println("Slideshow resumed");
    }
}

void PhotoFrame::next() {
    if (imageList.empty()) return;
    
    currentImageIndex++;
    if (currentImageIndex >= (int)imageList.size()) {
        currentImageIndex = 0;
        
        // Reshuffle if enabled
        if (shuffleEnabled) {
            shuffleImages();
        }
    }
    
    lastTransitionTime = millis();
    displayCurrentImage();
}

void PhotoFrame::previous() {
    if (imageList.empty()) return;
    
    currentImageIndex--;
    if (currentImageIndex < 0) {
        currentImageIndex = imageList.size() - 1;
    }
    
    lastTransitionTime = millis();
    displayCurrentImage();
}

String PhotoFrame::getCurrentImageFilename() {
    if (imageList.empty() || currentImageIndex < 0 || currentImageIndex >= (int)imageList.size()) {
        return "";
    }
    return imageList[currentImageIndex];
}

void PhotoFrame::displayCurrentImage() {
    String filename = getCurrentImageFilename();
    if (filename.length() > 0) {
        displayImage(filename);
        updatePhotoFrameUI(currentImageIndex + 1, imageList.size());
    }
}

void PhotoFrame::displayImage(const String& filename) {
    // This will be implemented in ui_photo_frame.cpp
    // For now, just log
    Serial.printf("Displaying image: %s (%d/%d)\n", 
                  filename.c_str(), 
                  currentImageIndex + 1, 
                  imageList.size());
    
    // The actual image display logic will use LVGL image decoder
    showPhotoFrameImage(filename);
}
