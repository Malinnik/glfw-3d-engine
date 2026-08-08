#include "player.h"
#include "world/WorldManager.h"

entity::player::Player::Player()
{
    transform = Transform(0, 30, 0);
    camera = new Camera(transform.position, glm::radians(70.0f));
    blockSelector = new BlockSelector(camera);
    crosshair = new Crosshair();
}

entity::player::Player::Player(Transform transform)
{
    this->transform = transform;
    camera = new Camera(transform.position, glm::radians(70.0f));
    blockSelector = new BlockSelector(camera);
    crosshair = new Crosshair();

}

entity::player::Player::~Player()
{
    delete camera;
    delete blockSelector;
    delete crosshair;

}

void entity::player::Player::onUpdate(float delta)
{
    processMovement(delta);
    processMouseMotion();
    processBlockInteraction();
    
    // TODO: make childs position auto depends on parent
    camera->position = transform.position;
}

void entity::player::Player::onRender()
{
    crosshair->draw();
}

void entity::player::Player::processMovement(float delta)
{
    // float speed = moveSpeed * delta;
    glm::vec3 moveDir(0.0f);

    if (Events::jPressed(GLFW_KEY_TAB))
        Events::toggleCursor();

    if (Events::pressed(GLFW_KEY_W))
        transform.position += camera->front * delta * speed;
    if (Events::pressed(GLFW_KEY_S))
        transform.position -= camera->front * delta * speed;
    if (Events::pressed(GLFW_KEY_D))
        transform.position += camera->right * delta * speed;
    if (Events::pressed(GLFW_KEY_A))
        transform.position -= camera->right * delta * speed;
    if (Events::pressed(GLFW_KEY_SPACE))
        transform.position += camera->up * delta * speed;
    if (Events::pressed(GLFW_KEY_LEFT_SHIFT))
        transform.position -= camera->up * delta * speed;

    if (glm::length(moveDir) > 0.0f) {
        moveDir = glm::normalize(moveDir);
        transform.position += moveDir * speed;
    }

    if (Events::scroll_up){
        // LOG_F(INFO, "Camera speed UP");
        if (speed >= 0)
            speed += 1;
    }
    if (Events::scroll_down){
        // LOG_F(INFO, "Camera speed DOWN");
        if (speed > 1) 
            speed -= 1;
    }
}

void entity::player::Player::processMouseMotion()
{
    // Поворот только если курсор захвачен
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
}

void entity::player::Player::processBlockInteraction()
{
    World* world = WorldManager::instance().getActiveWorld();
    if (!world) return;
    Chunks* chunks = world->getChunks();

    vec3 end;
    vec3 norm;
    vec3 iend;
    
    blocks::Block* blk = chunks->rayCast(camera->position, camera->front, 10.0f, end, norm, iend);
    if (blk != nullptr)
    {
        blockSelector->draw(iend.x, iend.y, iend.z);

        if (Events::jClicked(GLFW_MOUSE_BUTTON_1)){
            chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
        }
        if (Events::jClicked(GLFW_MOUSE_BUTTON_2)){
            chunks->set((int)(iend.x)+(int)(norm.x), (int)(iend.y)+(int)(norm.y), (int)(iend.z)+(int)(norm.z), 2);
        }
    }
}
