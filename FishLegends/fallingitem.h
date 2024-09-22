#ifndef FALLINGITEM_H
#define FALLINGITEM_H
#include "entity.h"

class FallingItem : public Entity {
public:
    FallingItem(Image &image, float x, float y, float w, float h, const std::string &name);

    void update(float time);
};

#endif // FALLINGITEM_H
