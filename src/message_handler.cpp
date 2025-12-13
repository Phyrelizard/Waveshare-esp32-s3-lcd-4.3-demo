#include "message_handler.h"
#include "ui/ui_message.h"

MessageHandler messageHandler;

void MessageHandler::begin() {
    // Initialize message display
    initMessageUI();
}

void MessageHandler::update() {
    // Check if current message should be dismissed
    if (currentMessage != nullptr) {
        if (currentMessage->duration > 0) {
            unsigned long elapsed = (millis() - messageDisplayStartTime) / 1000;
            if (elapsed >= (unsigned long)currentMessage->duration) {
                hideCurrentMessage();
            }
        }
    }
    
    // Process queue if no active message
    if (currentMessage == nullptr && !messageQueue.empty()) {
        processQueue();
    }
}

uint32_t MessageHandler::addMessage(const String& text, const String& title,
                                     int duration, MessagePriority priority) {
    Message msg;
    msg.text = text;
    msg.title = title;
    msg.duration = duration;
    msg.priority = priority;
    msg.timestamp = millis();
    msg.id = nextMessageId++;
    
    // Urgent messages interrupt current display
    if (priority == PRIORITY_URGENT) {
        if (currentMessage != nullptr) {
            hideCurrentMessage();
        }
        displayMessage(msg);
    } else {
        messageQueue.push(msg);
    }
    
    // Add to history
    messageHistory.push_back(msg);
    if (messageHistory.size() > 50) {  // Keep last 50 messages
        messageHistory.erase(messageHistory.begin());
    }
    
    Serial.printf("Message added: ID=%d, Priority=%d, Text=%s\n", 
                  msg.id, priority, text.c_str());
    
    return msg.id;
}

void MessageHandler::dismissMessage(uint32_t id) {
    if (currentMessage != nullptr && currentMessage->id == id) {
        hideCurrentMessage();
    }
}

void MessageHandler::dismissCurrentMessage() {
    if (currentMessage != nullptr) {
        hideCurrentMessage();
    }
}

void MessageHandler::processQueue() {
    if (messageQueue.empty()) return;
    
    Message msg = messageQueue.front();
    messageQueue.pop();
    displayMessage(msg);
}

void MessageHandler::displayMessage(const Message& msg) {
    if (currentMessage != nullptr) {
        delete currentMessage;
    }
    
    currentMessage = new Message(msg);
    messageDisplayStartTime = millis();
    
    // Display via UI
    showMessage(msg.text, msg.title, msg.priority);
    
    Serial.printf("Displaying message: %s\n", msg.text.c_str());
}

void MessageHandler::hideCurrentMessage() {
    if (currentMessage != nullptr) {
        hideMessage();
        delete currentMessage;
        currentMessage = nullptr;
    }
}

std::vector<Message> MessageHandler::getMessageHistory() {
    return messageHistory;
}
