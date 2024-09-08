#include "player.h"

Player::Player(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X,
Y, W, H, Name){
    playerScore = 0;
    state = stay;
    if (name == "Player1"){
        //Задаем спрайту один прямоугольник для
        //вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
}


void Player::control(){
    dx = 0;
    dy = 0;

    if (Keyboard::isKeyPressed(Keyboard::Left) or Keyboard::isKeyPressed(Keyboard::A)) {
    state = left;
    dx = -0.1;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right) or Keyboard::isKeyPressed(Keyboard::D)) {
    state = right;
    dx = -0.1;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Up) or Keyboard::isKeyPressed(Keyboard::W)) {
    state = up;
    dy = -0.1;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Down) or Keyboard::isKeyPressed(Keyboard::S)) {
    state = down;
    dy = -0.1;
    }
    else {
        state = stay;
        dx = 0;
        dy = 0;
    }
}

void Player::checkCollisionWithMap(float Dx, float Dy) {
    for (int i = y / 48; i < (y + h) / 48; i++)//проходимся по элементам карты
    for (int j = x / 48; j<(x + w) / 48; j++)
    {
        if (TileMap[i][j] == '0')//если элемент тайлик земли
        {
        if (Dy > 0) { y = i * 48 - h; dy = 0; }//по Y
        if (Dy < 0) { y = i * 48 + 48; dy = 0; }//столкновение с верхними краями
        if (Dx > 0) { x = j * 48 - w; dx = 0; }//с правым краем карты
        if (Dx < 0) { x = j * 48 + 48; dx = 0; }// с левым краем карты
        }

        if (TileMap[i][j] == 's') {
        playerScore++; //если взяли камень
        TileMap[i][j] = ' ';
        }

        if (TileMap[i][j] == 'f') {
        health -= 40;//если взяли ядовитый цветок
        TileMap[i][j] = ' ';//убрали цветок
        }

        if (TileMap[i][j] == 'h') {
        health += 20;//если взяли сердечко
        TileMap[i][j] = ' ';//убрали сердечко
        }
    }
}

void Player::update(float time) //метод "оживления/обновления" объекта класса.
{
    if (life) {//проверяем, жив ли герой
    control();//функция управления персонажем
    switch (state)//делаются различные действия в зависимости от состояния
    {
        case right:{//состояние идти вправо
        dx = 0.1;
        dy = 0;
        CurrentFrame += 0.005*time;
        if (CurrentFrame > 3) CurrentFrame -= 3;
        sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
        break;
        }

        case left:{//состояние идти влево
        dx = -0.1;
        dy = 0;
        CurrentFrame += 0.005*time;
        if (CurrentFrame > 3) CurrentFrame -= 3;
        sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
        break;
        }

        case up:{//идти вверх
        dx = 0;
        dy = -0.1;
        CurrentFrame += 0.005*time;
        if (CurrentFrame > 3) CurrentFrame -= 3;
        sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
        break;
        }

        case down:{//идти вниз
        dx = 0;
        dy = 0.1;
        CurrentFrame += 0.005*time;
        if (CurrentFrame > 3) CurrentFrame -= 3;
        sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
        break;
        }

        case stay:{//стоим
        dx = 0;
        dy = 0;
        break;
        }
    }
    x += dx*time; //движение по “X”
    checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
    y += dy*time; //движение по “Y”
    checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
    speed = 0; //обнуляем скорость, чтобы персонаж остановился.
    //state = stay;
    sprite.setPosition(x, y); //спрайт в позиции (x, y).
    if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
    }
}
