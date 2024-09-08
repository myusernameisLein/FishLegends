#include "map.h"
#include "entity.h" //Подключаем код с классом сщуностей
#include "player.h"
#include "bullet.h"
#include "enemy.h"
////////////////////////////КЛАСС СУЩНОСТЬ////////////////////////
////////////////////////////КЛАСС ИГРОКА////////////////////////
////////////////////////////КЛАСС ВРАГА////////////////////////
////////////////////////////КЛАСС ПУЛИ////////////////////////
int main()
{

    sf::SoundBuffer music;
    music.loadFromFile("sound/music.wav");// тут загружаем в буфер что то
    sf::Sound sound;
    sound.setBuffer(music);
    sound.setLoop(true);
    sound.play();

    sf::SoundBuffer bullet;
    bullet.loadFromFile("sound/bubble.wav");// тут загружаем в буфер что то
    sf::Sound sound2;
    sound2.setBuffer(bullet);
    //sound2.play();
sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
sf::RenderWindow window(sf::VideoMode(1248, 960, desktop.bitsPerPixel), "Lesson 12");
Font font;//шрифт
font.loadFromFile("Pixel Cyr.ttf");//передаем нашему шрифту файл шрифта
Text text("", font, 20);//создаем объект текст
text.setColor(Color::Black);//покрасили текст в красный
text.setStyle(Text::Regular);//жирный текст.
Image map_image;//объект изображения для карты
map_image.loadFromFile("images/map_new.png");//загружаем файл для карты
Texture map;//текстура карты
map.loadFromImage(map_image);//заряжаем текстуру картинкой
Sprite s_map;//создаём спрайт для карты
s_map.setTexture(map);//заливаем текстуру спрайтом
Clock clock;
Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
int gameTime = 0;//объявили игровое время, инициализировали.
Image heroImage;
heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
Image easyEnemyImage;
easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага
Image BulletImage;//изображение для пули
BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
Player p(heroImage, 100, 100, 96, 96, "Player1");//объект класса игрока
std::list<Entity*> enemies; //список врагов
std::list<Entity*> Bullets; //список пуль
std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка
std::list<Entity*>::iterator deathenemy;
const int ENEMY_COUNT = 3; //максимальное количество врагов в игре
int enemiesCount = 0; //текущее количество врагов в игре
//Заполняем список объектами врагами
for (int i = 0; i < ENEMY_COUNT; i++)
{
float xr = 300 + rand() % 500; // случайная координата врага на поле игры по оси “x”
float yr = 300 + rand() % 350; // случайная координата врага на поле игры по оси “y”
//создаем врагов и помещаем в список
enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
enemiesCount += 1; //увеличили счётчик врагов
}
//Создание переменных под время для генерирования водослей, змей и сердец
int createObjectForMapTimerAlga = 0;
int createObjectForMapTimerSnake = 0;
int createObjectForMapTimerHeart = 0;
int createObjectForMapTimerEnemy = 0;

while (window.isOpen())
{
float time = clock.getElapsedTime().asMicroseconds();
if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в
//секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо.
//оно не обновляет логику игры
clock.restart();
time = time / 600;
createObjectForMapTimerAlga += time;//наращиваем таймер
createObjectForMapTimerSnake += time;
createObjectForMapTimerHeart += time;
createObjectForMapTimerEnemy += time;

if (createObjectForMapTimerAlga>3000){
    p.randomMapGenerateAlga();//генерация камней
    createObjectForMapTimerAlga = 0;//обнуляем таймер
}
if (createObjectForMapTimerSnake>4800){
    p.randomMapGenerateSnake();//генерация змей
    createObjectForMapTimerSnake = 0;//обнуляем таймер
}
if (createObjectForMapTimerHeart>9000){
    p.randomMapGenerateHeart();//генерация сердец
    createObjectForMapTimerHeart = 0;//обнуляем таймер
}
sf::Event event;
while (window.pollEvent(event))
{
if (event.type == sf::Event::Closed)
window.close();
//стреляем по нажатию клавиши "Space"
if (event.type == sf::Event::KeyPressed)
{
if (event.key.code == sf::Keyboard::Space)
{
Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state));
sound2.play();
}
}
}
p.update(time); //оживляем объект “p” класса “Player”
//оживляем врагов
for (it = enemies.begin(); it != enemies.end(); it++)
{
(*it)->update(time); //запускаем метод update()
}
//оживляем пули
for (it = Bullets.begin(); it != Bullets.end(); it++)
{
(*it)->update(time); //запускаем метод update()
}
//Проверяем список на наличие "мертвых" пуль и удаляем их
for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
{// если этот объект мертв, то удаляем его
if ((*it)-> life == false) { it = Bullets.erase(it); }
else it++;//и идем курсором (итератором) к след объекту.
}
//Проверка пересечения игрока с врагами
//Если пересечение произошло, то "health = 0", игрок обездвижевается и
//выводится сообщение "you are lose"
if (p.life == true){//если игрок жив
for (it = enemies.begin(); it != enemies.end(); it++){//бежим по списку врагов
if ((p.getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy"))
{
p.health = 0;
std::cout << "you are lose";
}
}
}

if (p.life == false){
    for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
    {// если этот объект мертв, то удаляем его
     it = Bullets.erase(it);
    }
}

//пересечение пули с врагом
for (deathenemy = enemies.begin(); deathenemy != enemies.end(); deathenemy++){//бежим по списку врагов
     for (it = Bullets.begin(); it != Bullets.end(); it++){//по списку пуль
          if (((*it)->getRect().intersects((*deathenemy)->getRect())) &&
                ((*deathenemy)->name == "EasyEnemy") && ((*it)->name == "Bullet"))
            {
                cout << "Nice shot!\n";

                //при попадании пули у врага отнимается здоровье
                (*deathenemy)-> health = 0;
                if ((*deathenemy)-> health <= 0) { 
                    (*deathenemy)-> life = false;
                }
                (*it)-> life = false;

            }

        }
    }
for (deathenemy = enemies.begin(); deathenemy != enemies.end(); deathenemy++){
    if ((*deathenemy)-> life == false) {
        deathenemy = enemies.erase(deathenemy);
        enemiesCount -= 1;
    }
}

if (createObjectForMapTimerEnemy > 4000) {
    float xr = 300 + rand() % 500; // случайная координата врага на поле игры по оси “x”
    float yr = 300 + rand() % 350; // случайная координата врага на поле игры по оси “y”
    //создаем врагов и помещаем в список
    enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
    enemiesCount += 1; //увеличили счётчик врагов
    createObjectForMapTimerEnemy = 0;
}

window.clear();
/////////////////////////////Рисуем карту/////////////////////
for (int i = 0; i < HEIGHT_MAP; i++)
for (int j = 0; j < WIDTH_MAP; j++)
{
if (p.TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 48, 48));
if (p.TileMap[i][j] == 's') s_map.setTextureRect(IntRect(48, 0, 48, 48));
if ((p.TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(96, 0, 48, 48));
if ((p.TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(144, 0, 48, 48));//цветок
if ((p.TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(192, 0, 48, 48));//сердце
s_map.setPosition(j * 48, i * 48);
window.draw(s_map);
}
//объявили переменную здоровья и времени
std::ostringstream playerHealthString, gameTimeString;
playerHealthString << p.health; gameTimeString << gameTime;//формируем строку
text.setString("Health: " + playerHealthString.str() + "\nTime: " +
gameTimeString.str());//задаем строку тексту
text.setPosition(50, 50);//задаем позицию текста
window.draw(text);//рисуем этот текст
window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
//рисуем врагов
for (it = enemies.begin(); it != enemies.end(); it++)
{
if ((*it)->life) //если враги живы
window.draw((*it)->sprite); //рисуем
}
//рисуем пули
for (it = Bullets.begin(); it != Bullets.end(); it++)
{
if ((*it)->life) //если пули живы
window.draw((*it)->sprite); //рисуем объекты
}
window.display();
}
return 0;
}
