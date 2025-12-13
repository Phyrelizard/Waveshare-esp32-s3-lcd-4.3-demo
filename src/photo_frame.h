#ifndef PHOTO_FRAME_H
#define PHOTO_FRAME_H

#include <Arduino.h>
#include <vector>

enum SlideshowState {
    SLIDESHOW_STOPPED,
    SLIDESHOW_PLAYING,
    SLIDESHOW_PAUSED
};

class PhotoFrame {
public:
    void begin();
    void update();  // Call periodically
    
    void start();
    void stop();
    void pause();
    void resume();
    void next();
    void previous();
    
    bool isActive() { return state != SLIDESHOW_STOPPED; }
    SlideshowState getState() { return state; }
    
    void setDuration(int seconds) { slideshowDuration = seconds; }
    int getDuration() { return slideshowDuration; }
    
    void setShuffle(bool enabled) { shuffleEnabled = enabled; }
    bool isShuffle() { return shuffleEnabled; }
    
    void setClockOverlay(bool enabled) { clockOverlayEnabled = enabled; }
    bool hasClockOverlay() { return clockOverlayEnabled; }
    
    int getCurrentImageIndex() { return currentImageIndex; }
    int getTotalImages() { return imageList.size(); }
    String getCurrentImageFilename();
    
private:
    SlideshowState state = SLIDESHOW_STOPPED;
    std::vector<String> imageList;
    int currentImageIndex = 0;
    unsigned long lastTransitionTime = 0;
    int slideshowDuration = 30;  // seconds
    bool shuffleEnabled = false;
    bool clockOverlayEnabled = true;
    
    void loadImageList();
    void shuffleImages();
    void displayCurrentImage();
    void displayImage(const String& filename);
};

extern PhotoFrame photoFrame;

#endif
