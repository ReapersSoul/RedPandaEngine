#pragma once
#include <string>

namespace Events
{
    struct Event
    {
        std::string EventType;
    };
    struct MouseEvent : public Event
    {
        int x, y;
        int normal_x, normal_y;
        int button;
        int action;
        int mods;
        double scrollxoffset;
        double scrollyoffset;
    };
    struct KeyboardEvent : public Event
    {
        int key;
        int scancode;
        int action;
        int mods;
    };
    struct JoystickEvent : public Event
    {
        int jid;
        int evnt;
    };
    struct ErrorEvent : public Event
    {
        int error;
        const char *description;
    };

    class EventProcessor
    {
    public:
        virtual bool HandleEvent(Event *e)
        {
            return true;
        };
    };
}