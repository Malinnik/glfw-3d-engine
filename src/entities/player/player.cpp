#include "player.h"

entity::player::Player::Player()
{
    camera = new Camera(transform.position, 70);
}
