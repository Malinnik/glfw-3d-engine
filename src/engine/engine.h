#pragma once

#include "window/events.h"
#include "window/window.h"
#include "ui/gui.h"
#include "world/world.h"

class Engine{
    Window window;
    Events events;
    imgui gui;

    World* world;
};