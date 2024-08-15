#ifndef ENTITY
#define ENTITY
#include "map.h"

#include <windows.h>

////////////////////////////КЛАСС СУЩНОСТЬ////////////////////////
class Entity {

public:

enum { left, right, up, down, stay} state;// тип перечисления - состояние объекта
float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей

int w, h, health; //переменная “health”, хранящая жизни игрока
bool life; //переменная “life” жизнь, логическая

Texture texture;//сфмл текстура

Sprite sprite;//сфмл спрайт

float CurrentFrame;//хранит текущий кадр

std::string name;//враги могут быть разные, врагов можно различать по именам
//каждому можно дать свое действие в update() в зависимости от имени

Entity(Image &image, float X, float Y, int W, int H, std::string Name);

FloatRect getRect();

virtual void update(float time) = 0;

std::string TileMap[HEIGHT_MAP] = {
    "00000000000000000000000000",
    "0                        0",
    "0      s                 0",
    "0                 f      0",
    "0                        0",
    "0                        0",
    "0                        0",
    "0          f             0",
    "0                        0",
    "0                        0",
    "0                   h    0",
    "0                        0",
    "0    f                   0",
    "0                        0",
    "0                        0",
    "0                        0",
    "0                        0",
    "0                        0",
    "0                        0",
    "00000000000000000000000000",

};

void randomMapGenerate();//рандомно расставляем камни

};
#endif // ENTITY

