#include "player.h"
#include "world/WorldManager.h"
#include "blocks/block.h"

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

    physics.update(delta, *this);

    // TODO: make childs position auto depends on parent
    camera->position = transform.position + glm::vec3(0, eyeHeight, 0);
}

void entity::player::Player::onRender()
{
    crosshair->draw();
    if (hasTarget)
        blockSelector->draw(targetBlock.x, targetBlock.y, targetBlock.z);
}

void entity::player::Player::processMovement(float delta)
{
    // TODO: Move acceleration to other class staff
    float acceleration = (physics.onGround ? 30 : 10);
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
        physics.velocity += moveDir * acceleration * delta;
    }
    else 
    {
        physics.velocity.x *= (1.0f - 10.0f * delta);
        physics.velocity.z *= (1.0f - 10.0f * delta);
    }

    float maxSpeed = 8.0f;
    glm::vec2 horiz(physics.velocity.x, physics.velocity.z);
    if (glm::length(horiz) > maxSpeed)
     {
        horiz = glm::normalize(horiz) * maxSpeed;
        physics.velocity.x = horiz.x;
        physics.velocity.z = horiz.y;
    }

    if (physics.onGround && Events::pressed(GLFW_KEY_SPACE))
    {
        physics.velocity.y = physics.jumpStrength;
        physics.onGround = false;
        // transform.position += camera->up;

    }
    // if (Events::pressed(GLFW_KEY_LEFT_SHIFT))
    //     transform.position -= camera->up;


    // if (Events::scroll_up){
    //     // LOG_F(INFO, "Camera speed UP");
    //     if (speed >= 0)
    //         speed += 1;
    // }
    // if (Events::scroll_down){
    //     // LOG_F(INFO, "Camera speed DOWN");
    //     if (speed > 1) 
    //         speed -= 1;
    // }
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
        targetBlock = glm::ivec3(iend.x, iend.y, iend.z);
        hasTarget = true;
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
        hasTarget = false;
    }
}

entity::PhysicsComponent::PhysicsComponent() {}

void entity::PhysicsComponent::update(float delta, Entity &owner)
{
    if (!m_blockQuery) return;

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
