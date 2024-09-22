#include "bullet.h"
using namespace sf;
Bullet::Bullet(Image &image, float X, float Y, int W, int H, std::string Name, int dir)
:Entity(image, X, Y, W, H, Name){
    x = X;
    y = Y;
    direction = dir;
    speed = 0.8;
    w = h = 16;
    life = true;
//выше инициализация в конструкторе
}
void Bullet::update(float time)
{
    switch (direction)
{
    case 0: dx = -speed; dy = 0; break;// state = left
    case 1: dx = speed; dy = 0; break;// state = right
    case 2: dx = speed; dy = 0; break;// state = left
    case 3: dx = -speed; dy = 0; break;// state = right
}
    if (life){
        x += dx*time;//само движение пули по х
        y += dy*time;//по у
        if (x <= 0) x = 20;// задержка пули в левой стене, чтобы при проседании кадров
         if (y <= 0) y = 20;
             if (x >= 1248) x = 1218;// задержка пули в правой стене, чтобы при проседании
                 if (y >= 960) y = 930;
                    for (int i = y / 48; i < (y + h) / 48; i++)//проходимся по элементам карты
                    for (int j = x / 48; j < (x + w) / 48; j++)
                            {
                              if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
                                 life = false;// то пуля умирает
                            }
        sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пули
}
}
