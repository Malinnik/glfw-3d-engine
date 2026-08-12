#include "player.h"
#include "world/WorldManager.h"
#include "blocks/block.h"
#include "loguru.hpp"

entity::player::Player::Player()
{
    transform = Transform(0, 30, 0);
    camera = new Camera(transform.position, glm::radians(70.0f));
    blockSelector = new BlockSelector();
    crosshair = new Crosshair();

    World* world = WorldManager::instance().getActiveWorld();
    if (world)
        physics.gravity = world->Gravity;
    physics.setSize(0.6f, 1.8f);
    physics.setBlockQuery([this](const AABB& box) -> std::vector<glm::ivec3> {
        World* world = WorldManager::instance().getActiveWorld();
        if (!world) return {};
        Chunks* chunks = world->getChunks();
        if (!chunks) return {};
        std::vector<glm::ivec3> result;
        int minX = (int)floor(box.min.x);
        int maxX = (int)floor(box.max.x - 0.001f);
        int minY = (int)floor(box.min.y);
        int maxY = (int)floor(box.max.y - 0.001f);
        int minZ = (int)floor(box.min.z);
        int maxZ = (int)floor(box.max.z - 0.001f);
        for (int y = minY; y <= maxY; ++y)
            for (int z = minZ; z <= maxZ; ++z)
                for (int x = minX; x <= maxX; ++x)
                {
                    blocks::Block* blk = chunks->get(x, y, z);
                    if (!blk) continue;
                    if (blk->id != 0)
                        result.push_back({x, y, z});
                }
        return result;
    });

}

entity::player::Player::Player(Transform transform)
{
    this->transform = transform;
    camera = new Camera(transform.position, glm::radians(70.0f));
    blockSelector = new BlockSelector();
    crosshair = new Crosshair();

    setController(std::make_unique<SurvivalController>(&physics, camera, &targetBlock, &hasTarget));


    // World* world = WorldManager::instance().getActiveWorld();
    // if (world)
    //     physics.gravity = world->Gravity;
    physics.gravity = 32.0f;
    physics.setSize(0.6f, 1.8f);
    physics.setBlockQuery([this](const AABB& box) -> std::vector<glm::ivec3> {
        World* world = WorldManager::instance().getActiveWorld();
        if (!world) return {};
        Chunks* chunks = world->getChunks();
        if (!chunks) return {};
        std::vector<glm::ivec3> result;
        int minX = (int)floor(box.min.x);
        int maxX = (int)floor(box.max.x - 0.001f);
        int minY = (int)floor(box.min.y);
        int maxY = (int)floor(box.max.y - 0.001f);
        int minZ = (int)floor(box.min.z);
        int maxZ = (int)floor(box.max.z - 0.001f);
        for (int y = minY; y <= maxY; ++y)
            for (int z = minZ; z <= maxZ; ++z)
                for (int x = minX; x <= maxX; ++x)
                {
                    blocks::Block* blk = chunks->get(x, y, z);
                    if (!blk) continue;
                    if (blk->id != 0)
                        result.push_back({x, y, z});
                }
        return result;
    });
}

entity::player::Player::~Player()
{
    delete camera;
    delete blockSelector;
    delete crosshair;

}

void entity::player::Player::onUpdate(float delta)
{
    handleControl(delta);
    physics.update(delta, *this);
    // TODO: make childs position auto depends on parent
    camera->position = transform.position + glm::vec3(0, eyeHeight, 0);

    if (Events::jPressed(GLFW_KEY_F1))
        setController(std::make_unique<SurvivalController>(&physics, camera, &targetBlock, &hasTarget));
    if (Events::jPressed(GLFW_KEY_F2))
        setController(std::make_unique<CreativeController>(&physics, camera, &targetBlock, &hasTarget, &speed));
}

void entity::player::Player::onRender()
{
    crosshair->draw();
    if (hasTarget)
        blockSelector->draw(targetBlock.x, targetBlock.y, targetBlock.z);
}


entity::PhysicsComponent::PhysicsComponent() {}

void entity::PhysicsComponent::update(float delta, Entity &owner)
{
    if (!m_blockQuery) return;

    if (gravityEnabled)
        velocity.y -= gravity * delta;

    moveAxis(velocity.x * delta, 0, owner);
    moveAxis(velocity.y * delta, 1, owner);
    moveAxis(velocity.z * delta, 2, owner);
}

void entity::PhysicsComponent::setSize(float width, float height)
{
    m_width = width;
    m_height = height;
}

AABB entity::PhysicsComponent::getAABB(const glm::vec3 &position) const
{
    return AABB(position - glm::vec3(m_width / 2.0f, 0.0f, m_width / 2.0f),
                glm::vec3(m_width, m_height, m_width));
}

void entity::PhysicsComponent::moveAxis(float amount, int axis, Entity &owner)
{
    if (amount == 0.0f) return;

    glm::vec3& pos = owner.transform.position;
    AABB currentBox = getAABB(pos);
    AABB testBox = currentBox;
    if (axis == 0) testBox.translate(glm::vec3(amount, 0, 0));
    else if (axis == 1) testBox.translate(glm::vec3(0, amount, 0));
    else if (axis == 2) testBox.translate(glm::vec3(0, 0, amount));

    auto blocks = m_blockQuery(testBox);
    if (blocks.empty()) {
        if (axis == 0) pos.x += amount;
        else if (axis == 1) pos.y += amount;
        else if (axis == 2) pos.z += amount;
        return;
    }

    float sign = (amount > 0) ? 1.0f : -1.0f;
    float correctedAmount = amount;

    for (const auto& block : blocks) {
        AABB blockBox(glm::vec3(block.x, block.y, block.z), glm::vec3(1.0f));
        if (axis == 0) {
            if (sign > 0) correctedAmount = std::min(correctedAmount, amount - (testBox.max.x - blockBox.min.x));
            else          correctedAmount = std::max(correctedAmount, amount + (blockBox.max.x - testBox.min.x));
        } else if (axis == 1) {
            if (sign > 0) correctedAmount = std::min(correctedAmount, amount - (testBox.max.y - blockBox.min.y));
            else          correctedAmount = std::max(correctedAmount, amount + (blockBox.max.y - testBox.min.y));
        } else {
            if (sign > 0) correctedAmount = std::min(correctedAmount, amount - (testBox.max.z - blockBox.min.z));
            else          correctedAmount = std::max(correctedAmount, amount + (blockBox.max.z - testBox.min.z));
        }
    }

    if (axis == 0) pos.x += correctedAmount;
    else if (axis == 1) {
        pos.y += correctedAmount;
        if (amount < 0 && correctedAmount > amount) {
            velocity.y = 0.0f;
            onGround = true;
        } else if (amount > 0 && correctedAmount < amount) {
            velocity.y = 0.0f;
        }
    } else pos.z += correctedAmount;

    if (axis == 0) velocity.x = 0.0f;
    if (axis == 2) velocity.z = 0.0f;
}

entity::SurvivalController::SurvivalController(class PhysicsComponent* physicsComponent, class Camera* camera, glm::ivec3* targetBlock, bool* hasTarget) : physics(physicsComponent), camera(camera), targetBlock(targetBlock), hasTarget(hasTarget)
{
    if (physics)
        physics->gravityEnabled = true;
}

void entity::SurvivalController::processMovement(float delta)
{
    if (!camera) return;
    if (!physics) return;

    // TODO: Move acceleration to other class staff
    float acceleration = (physics->onGround ? 50 : 5);
    glm::vec3 moveDir(0.0f);

    if (Events::jPressed(GLFW_KEY_TAB))
        Events::toggleCursor();

    if (Events::pressed(GLFW_KEY_W))
        moveDir += camera->front;
    if (Events::pressed(GLFW_KEY_S))
        moveDir -= camera->front;
    if (Events::pressed(GLFW_KEY_D))
        moveDir += camera->right;
    if (Events::pressed(GLFW_KEY_A))
        moveDir -= camera->right;

    if (glm::length(moveDir) > 0.0f)
    {
        moveDir = glm::normalize(moveDir);
        moveDir.y = 0.0f;
        physics->velocity += moveDir * acceleration * delta;
    }
    else 
    {
        physics->velocity.x *= (1.0f - 10.0f * delta);
        physics->velocity.z *= (1.0f - 10.0f * delta);
    }

    float maxSpeed = 4.0f;
    glm::vec2 horiz(physics->velocity.x, physics->velocity.z);
    if (glm::length(horiz) > maxSpeed)
     {
        horiz = glm::normalize(horiz) * maxSpeed;
        physics->velocity.x = horiz.x;
        physics->velocity.z = horiz.y;
    }

    if (physics->onGround && Events::pressed(GLFW_KEY_SPACE))
    {
        physics->velocity.y = physics->jumpStrength;
        physics->onGround = false;

    }
}

void entity::SurvivalController::processMouseMotion()
{
    if (!camera)
        return;

    if (Events::cursor_locked) {
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

void entity::SurvivalController::processBlockInteraction()
{
    World* world = WorldManager::instance().getActiveWorld();
    if (!world) return;
    Chunks* chunks = world->getChunks();
    if (!chunks) return;

    vec3 end;
    vec3 norm;
    vec3 iend;
    
    blocks::Block* blk = chunks->rayCast(camera->position, camera->front, 10.0f, end, norm, iend);
    if (blk != nullptr)
    {
        *targetBlock = glm::ivec3(iend.x, iend.y, iend.z);
        *hasTarget = true;
        // blockSelector->draw(iend.x, iend.y, iend.z);

        if (Events::jClicked(GLFW_MOUSE_BUTTON_1)){
            chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
        }
        if (Events::jClicked(GLFW_MOUSE_BUTTON_2)){
            chunks->set((int)(iend.x)+(int)(norm.x), (int)(iend.y)+(int)(norm.y), (int)(iend.z)+(int)(norm.z), 2);
        }
    }
    else 
    {
        *hasTarget = false;
    }
}

entity::CreativeController::CreativeController(class PhysicsComponent* physicsComponent, class Camera* camera, glm::ivec3* targetBlock, bool* hasTarget, float* speed) : physics(physicsComponent), camera(camera), targetBlock(targetBlock), hasTarget(hasTarget), speed(speed)
{
    physics->gravityEnabled = false;
}

void entity::CreativeController::processMovement(float delta)
{
    if (Events::jPressed(GLFW_KEY_TAB))
        Events::toggleCursor();

    if (Events::doublePressed(GLFW_KEY_SPACE)) {
        physics->gravityEnabled = !physics->gravityEnabled;
        physics->velocity.y = 0.0f;
    }

    glm::vec3 moveDir(0.0f);

    // Горизонтальное движение
    if (Events::pressed(GLFW_KEY_W)) moveDir += camera->front;
    if (Events::pressed(GLFW_KEY_S)) moveDir -= camera->front;
    if (Events::pressed(GLFW_KEY_D)) moveDir += camera->right;
    if (Events::pressed(GLFW_KEY_A)) moveDir -= camera->right;

    // Вертикальное движение только при включённом полёте
    if (!physics->gravityEnabled) {
        if (Events::pressed(GLFW_KEY_SPACE)) moveDir.y += 1.0f;
        if (Events::pressed(GLFW_KEY_LEFT_SHIFT)) moveDir.y -= 1.0f;
    } else {
        // При гравитации пробел = прыжок (как в выживании)
        if (physics->onGround && Events::pressed(GLFW_KEY_SPACE)) {
            physics->velocity.y = physics->jumpStrength;
            physics->onGround = false;
        }
    }

    // Регулировка скорости колёсиком
    if (Events::scroll_up) {
        *speed += 1.0f;
        Events::scroll_up = false;
    }
    if (Events::scroll_down) {
        *speed = std::max(1.0f, *speed - 1.0f);
        Events::scroll_down = false;
    }

    glm::vec3 desiredVelocity(0.0f);
    if (glm::length(moveDir) > 0.0f) {
        moveDir = glm::normalize(moveDir);
        desiredVelocity = moveDir * (*speed);
    }

    if (physics->gravityEnabled) {
        // Если гравитация включена, управляем только горизонтальными компонентами,
        // вертикальная скорость пусть меняется физикой (гравитацией и прыжками).
        physics->velocity.x = desiredVelocity.x;
        physics->velocity.z = desiredVelocity.z;
    } else {
        // Полёт: полный контроль скорости, включая вертикальную.
        physics->velocity = desiredVelocity;
    }

}

void entity::CreativeController::processMouseMotion()
{
    if (Events::cursor_locked) {
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

void entity::CreativeController::processBlockInteraction()
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
        *targetBlock = glm::ivec3(iend.x, iend.y, iend.z);
        *hasTarget = true;
        // blockSelector->draw(iend.x, iend.y, iend.z);

        if (Events::jClicked(GLFW_MOUSE_BUTTON_1)){
            chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
        }
        if (Events::jClicked(GLFW_MOUSE_BUTTON_2)){
            chunks->set((int)(iend.x)+(int)(norm.x), (int)(iend.y)+(int)(norm.y), (int)(iend.z)+(int)(norm.z), 2);
        }
    }
    else 
    {
        *hasTarget = false;
    }
}
