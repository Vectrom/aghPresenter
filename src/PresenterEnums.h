#pragma once

enum class TimerPosition : int {
    LeftTop = 0,
    RightTop,
    LeftBottom,
    RightBottom
};

enum class ShowOnPresentationTimerType : int
{
    Nothing = 0,
    DurationClock,
    Timer
};
