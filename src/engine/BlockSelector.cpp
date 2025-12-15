#include "BlockSelector.h"

BlockSelector::BlockSelector(Camera* camera) : camera(camera)
{
    shader = new Shader("./assets/shaders/lines.vert", "./assets/shaders/lines.frag");
    lineBatch = new LineBatch(4096);
}

BlockSelector::~BlockSelector()
{
    delete shader;
    delete lineBatch;
}

void BlockSelector::draw(int x, int y, int z)
{
    float center_x = x + 0.5f;
    float center_y = y + 0.5f;
    float center_z = z + 0.5f;

    shader->use();
    shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
    lineBatch->box(center_x, center_y, center_z, 1.005f,1.005f,1.005f, 0,0,0,0.5f);
    lineBatch->draw();
}
