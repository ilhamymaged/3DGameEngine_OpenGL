#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

enum class EventType
{
    KeyPressedEvent,
    KeyReleasedEvent,
    MouseMovedEvent,
    MouseScrolledEvent,
    WindowResizedEvent
};

class Event
{
public:
    virtual ~Event() = default;
    virtual EventType getType() const = 0;
};

class EventDispatcher
{
public:
    EventDispatcher(Event& e) : m_Event{ e }, m_Handled{ false } {}

    template<typename T, typename F>
    void Dispatch(const F& func)
    {
        if (m_Event.getType() == T::GetStaticType())
        {
            func(static_cast<T&>(m_Event));
            m_Handled = true;
        }
    }

private:
    Event& m_Event;
    bool m_Handled;
};


class WindowResized : public Event
{
public:
    WindowResized(int width, int height) : width(width), height(height){}
    static EventType GetStaticType() { return EventType::WindowResizedEvent; }
    EventType getType() const override { return GetStaticType(); }

    int GetNewWidth() { return width; }
    int GetNewHeight() { return height; }

private:
    int width, height;
};

class MouseMoved : public Event
{
public:
    MouseMoved(double x, double y) : x(x), y(y) {}
    static EventType GetStaticType() { return EventType::MouseMovedEvent; }
    EventType getType() const override { return GetStaticType(); }

    double getX() { return x; }
    double getY() { return y; }

private:
    double x;
    double y;
};

class MouseScrolled : public Event
{
public:
    MouseScrolled(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
    static EventType GetStaticType() { return EventType::MouseScrolledEvent; }
    EventType getType() const override { return GetStaticType(); }

    double getXoffset() { return xoffset; }
    double getYoffset() { return yoffset; }
 
private:
    double xoffset;
    double yoffset;
};

class KeyPressed : public Event
{
public:
    KeyPressed(int key, int scancode, int mods)
        : key(key), scancode(scancode), action(GLFW_PRESS), mods(mods) {}

    static EventType GetStaticType() { return EventType::KeyPressedEvent; }
    EventType getType() const override { return GetStaticType(); }
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

    static EventType GetStaticType() { return EventType::KeyReleasedEvent; }
    EventType getType() const override { return GetStaticType(); }
    int getKey() const { return key; }

private:
    int key;
    int scancode;
    int action;
    int mods;
};