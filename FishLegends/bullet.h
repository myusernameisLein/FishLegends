#ifndef BULLET
#define BULLET

#include "entity.h"
class Bullet :public Entity{//класс пули
public:
int direction;//направление пули

Bullet(Image &image, float X, float Y, int W, int H, std::string Name, int dir);

void update(float time);
};

#endif // BULLET

