#pragma once

namespace Agina
{
    class Time
    {
    public:
        static void Update();
        static float GetDeltaTime();

    private:
        static float deltaTime;
        static float lastFrame;
    };
}