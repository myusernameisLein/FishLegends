#include "entity.h"

Entity::Entity(Image &image, float X, float Y, int W, int H, std::string Name){
    x = X; y = Y; //координата появления спрайта
    w = W; h = H;
    name = Name;
    moveTimer = 0;
    dx = 0; dy = 0;
    speed = 0;
    CurrentFrame = 0;
    health = 100;
    life = true; //инициализировали логическую переменную жизни, герой жив
    texture.loadFromImage(image); //заносим наше изображение в текстуру
    sprite.setTexture(texture); //заливаем спрайт текстурой
}

FloatRect Entity::getRect(){//метод получения прямоугольника. его коорд, размеры (шир,высот).
    FloatRect FR(x, y, w, h); // переменная FR типа FloatRect
    return FR;
    //Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
    //в нашей игре это координаты текущего расположения тайла на карте
    //далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте
    //эта ф-ция нужна для проверки пересечений
}

void Entity::randomMapGenerate(){//рандомно расставляем камни
    int randomElementX = 0;//переменная для хранения случайного элемента по горизонтали
    int randomElementY = 0;//переменная для хранения случайного элемента по вертикали
    srand(time(0));//Инициализация генератора случайных чисел
    int countStone = 5;//количество камней
        while (countStone>0){
            randomElementX = 1 + rand() % (WIDTH_MAP - 1);//псевдослучайное значение по “x” от 1 до
            //ширина карты-1. Ограничение введено чтобы не получать числа бордюра карты
            randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//по “y”
                if (TileMap[randomElementY][randomElementX] == ' ') {//если встретили символ пробел,
                    TileMap[randomElementY][randomElementX] = 's'; //то ставим туда камень.
            countStone--;
            }
        }
}

