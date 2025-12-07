#include <loguru.hpp>
#include "fmt/format.h"

#include "config.h"
#include "InputLoop.h"
#include "blocks/block.h"
#include "world/chunk.h"
#include "files/file.h"

InputLoop::InputLoop(Camera* camera, Chunks* chunks) : camera(camera), chunks(chunks) 
{
    blockSelector = new BlockSelector(camera);
}

InputLoop::~InputLoop()
{
    delete blockSelector;
}

void InputLoop::updateTime(){
    float currentTime = glfwGetTime();
    delta = currentTime - lastTime;
    lastTime = currentTime;
}


void InputLoop::inputLoop(){
    updateTime();

    if (Events::jPressed(GLFW_KEY_TAB)){
        Events::toggleCursor();
    }

    if (Events::pressed(GLFW_KEY_W)){
        camera->position += camera->front * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_S)){
        camera->position -= camera->front * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_D)){
        camera->position += camera->right * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_A)){
        camera->position -= camera->right * delta * camera->speed;
    }

    if (Events::pressed(GLFW_KEY_SPACE)){
        camera->position += camera->up * delta * camera->speed;
    }

    if (Events::pressed(GLFW_KEY_LEFT_SHIFT)){
        camera->position -= camera->up * delta * camera->speed;
    }

    if (Events::jPressed(GLFW_KEY_F1))
    {
        unsigned char* buffer = new unsigned char[chunks->volume * CHUNK_BLOCKS];
        chunks->write(buffer);
        FileReader::writeBinary("world.bin", (const char*) buffer, chunks->volume * CHUNK_BLOCKS);
        delete[] buffer;
        LOG_F(INFO, fmt::format("WORLD SAVED IN {} WITH {} BYTES", "world.bin", chunks->volume*CHUNK_BLOCKS).c_str());
    }

    if (Events::jPressed(GLFW_KEY_F2))
    {
        unsigned char* buffer = new unsigned char[chunks->volume * CHUNK_BLOCKS];
        FileReader::readBinary("world.bin", (char*) buffer, chunks->volume * CHUNK_BLOCKS);
        chunks->read(buffer);
        delete[] buffer;
        LOG_F(INFO, fmt::format("WORLD LOADED FROM {}", "world.bin").c_str());
    }

    if (Events::scroll_up){
        // LOG_F(INFO, "Camera speed UP");
        if (camera->speed >= 0)
        camera->speed += 1;
    }
    if (Events::scroll_down){
        // LOG_F(INFO, "Camera speed DOWN");
        if (camera->speed > 1) 
        camera->speed -= 1;
    }

    if (Events::cursor_locked){
        camera->camY += -Events::deltaY / HEIGHT * 2;
        camera->camX += -Events::deltaX / HEIGHT * 2;

        if (camera->camY < -radians(89.0f))
            camera->camY = -radians(89.0f);
        if (camera->camY > radians(89.0f))
            camera->camY = radians(89.0f);

        camera->rotation = mat4(1.0f);
        camera->rotate(camera->camY, camera->camX, 0);
    }


    {
        vec3 end;
        vec3 norm;
        vec3 iend;
        
        block* blk = chunks->rayCast(camera->position, camera->front, 10.0f, end, norm, iend);
        if (blk != nullptr)
        {
            LOG_F(INFO, fmt::format("Found block at: {} {} {}",iend.x, iend.y, iend.z).c_str());
            blockSelector->draw(iend.x, iend.y, iend.z);

            if (Events::jClicked(GLFW_MOUSE_BUTTON_1)){
                chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
            }
            if (Events::jClicked(GLFW_MOUSE_BUTTON_2)){
                chunks->set((int)(iend.x)+(int)(norm.x), (int)(iend.y)+(int)(norm.y), (int)(iend.z)+(int)(norm.z), 2);
            }
        }

        
    }
}