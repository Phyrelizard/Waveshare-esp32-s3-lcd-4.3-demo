#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <Arduino.h>
#include <queue>
#include <vector>

enum MessagePriority {
    PRIORITY_NORMAL,
    PRIORITY_IMPORTANT,
    PRIORITY_URGENT
};

struct Message {
    String text;
    String title;
    int duration;  // -1 for permanent
    MessagePriority priority;
    unsigned long timestamp;
    uint32_t id;
};

class MessageHandler {
public:
    void begin();
    void update();
    
    uint32_t addMessage(const String& text, const String& title = "", 
                        int duration = 10, MessagePriority priority = PRIORITY_NORMAL);
    void dismissMessage(uint32_t id);
    void dismissCurrentMessage();
    
    bool hasActiveMessage() { return currentMessage != nullptr; }
    Message* getCurrentMessage() { return currentMessage; }
    
    std::vector<Message> getMessageHistory();
    
private:
    std::queue<Message> messageQueue;
    Message* currentMessage = nullptr;
    std::vector<Message> messageHistory;
    uint32_t nextMessageId = 1;
    unsigned long messageDisplayStartTime = 0;
    
    void processQueue();
    void displayMessage(const Message& msg);
    void hideCurrentMessage();
};

extern MessageHandler messageHandler;

#endif
