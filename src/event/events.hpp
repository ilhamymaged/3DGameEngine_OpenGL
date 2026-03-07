#pragma once
#include <GLFW/glfw3.h>

enum class EventType
{
    KeyPressed,
    KeyReleased,
    MouseMoved,
    MouseScrolled,

    WindowResized
};

class Event
{
public:
    virtual ~Event() = default;
    virtual EventType getType() const = 0;
};

class WindowResized : public Event
{
public:
    WindowResized(int width, int height) : width(width), height(height){}
    EventType getType() const override { return EventType::WindowResized; }

    int GetNewWidth() { return width; }
    int GetNewHeight() { return height; }

private:
    int width, height;
};

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(double x, double y) : x(x), y(y) {}
    EventType getType() const override { return EventType::MouseMoved; }

    double getX() { return x; }
    double getY() { return y; }

private:
    double x;
    double y;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
    EventType getType() const override { return EventType::MouseScrolled; }

    double getXoffset() { return xoffset; }
    double getYoffset() { return yoffset; }
 
private:
    double xoffset;
    double yoffset;
};

class KeyPressedEvent : public Event
{
public:
    KeyPressedEvent(int key, int scancode, int mods)
        : key(key), scancode(scancode), action(GLFW_PRESS), mods(mods) {}

    EventType getType() const override { return EventType::KeyPressed; }
    int getKey() const { return key; }

private:
    int key;
    int scancode;
    int mods;
    int action;
};

class KeyReleased : public Event
{
public:
    KeyReleased(int key, int scancode, int mods)
        : key(key), scancode(scancode), action(GLFW_RELEASE), mods(mods) {}

    EventType getType() const override { return EventType::KeyReleased; }
    int getKey() const { return key; }

private:
    int key;
    int scancode;
    int action;
    int mods;
};