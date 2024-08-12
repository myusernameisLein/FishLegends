#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h" //подключили код с картой
using namespace sf;
////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player { // класс Игрока
private: float x, y;
public:
float w, h, dx, dy, speed; //координаты игрока (х,у), высота и ширина,
//ускорение (по “х” и по “у”), сама скорость
int playerScore, health;//новая переменная “health”, хранящая жизни игрока
float CurrentFrame = 0;//хранит текущий кадр
bool life;//переменная “life” жизнь, логическая
enum {left, right, up, down, stay } state;//перечисления - состояние объекта
std::string File; //файл с расширением
Image image;//сфмл изображение
Texture texture;//сфмл текстура
Sprite sprite;//сфмл спрайт
//Конструктор с параметрами для класса Player. При создании объекта класса мы будем задавать
//имя файла, координату Х и У, ширину и высоту
Player(std::string F, float X, float Y, float W, float H){
playerScore = 0;
dx = 0; dy = 0; speed = 0; state = stay;
health = 100;
//инициализировали переменную жизни в конструкторе
life = true;//инициализировали логическую переменную жизни, герой жив
File = F; //имя файла+расширение
w = W; h = H; //высота и ширина
image.loadFromFile("images/" + File);//загружаем в image изображение,
image.createMaskFromColor(Color(41, 33, 59)); //убираем ненужный темно-синий цвет
texture.loadFromImage(image); //заносим наше изображение в текстуру
sprite.setTexture(texture); //заливаем спрайт текстурой
x = X; y = Y; //координата появления спрайта
sprite.setTextureRect(IntRect(0, 0, w, h));
//Задаем спрайту один прямоугольник для
//вывода одного льва. IntRect – для приведения типов
}
void control(){
if (Keyboard::isKeyPressed(Keyboard::Left)) {
state = left;
speed = 0.1;
}
if (Keyboard::isKeyPressed(Keyboard::Right)) {
state = right;
speed = 0.1;
}
if (Keyboard::isKeyPressed(Keyboard::Up)) {
state = up;
speed = 0.1;
}
if (Keyboard::isKeyPressed(Keyboard::Down)) {
state = down;
speed = 0.1;
}
}
void update(float time) //метод "оживления/обновления" объекта класса.
{
if (life) {//проверяем, жив ли герой
control();//функция управления персонажем
switch (state)//делаются различные действия в зависимости от состояния
{
case right:{//состояние идти вправо
dx = speed;
CurrentFrame += 0.005*time;
if (CurrentFrame > 3) CurrentFrame -= 3;
sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
break;
}
case left:{//состояние идти влево
dx = -speed;
CurrentFrame += 0.005*time;
if (CurrentFrame > 3) CurrentFrame -= 3;
sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
break;
}
case up:{//идти вверх
dy = -speed;
CurrentFrame += 0.005*time;
if (CurrentFrame > 3) CurrentFrame -= 3;
sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
break;
}
case down:{//идти вниз
dy = speed;
CurrentFrame += 0.005*time;
if (CurrentFrame > 3) CurrentFrame -= 3;
sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
break;
}
case stay:{//стоим
dy = speed;
dx = speed;
break;
}
}
x += dx*time; //движение по “X”
checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
y += dy*time; //движение по “Y”
checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
speed = 0; //обнуляем скорость, чтобы персонаж остановился.
//state = stay; //состояние - стоим
sprite.setPosition(x, y); //спрайт в позиции (x, y).
if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
}
}
//Метод проверки столкновений с элементами карты
void checkCollisionWithMap(float Dx, float Dy) {
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
FloatRect getRect(){//метод получения прямоугольника. его коорд, размеры (шир,высот).
FloatRect FR(x, y, w, h);
return FR;
//Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
//в нашей игре это координаты текущего расположения тайла на карте
//далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте
//эта ф-ция нужна для проверки пересечений
}
};
int main()
{
sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
sf::RenderWindow window(sf::VideoMode(1248, 960, desktop.bitsPerPixel), "Lesson 12");
Font font;//шрифт
font.loadFromFile("Pixel Cyr.ttf");//передаем нашему шрифту файл шрифта
Text text("", font, 20);//создаем объект текст
text.setColor(Color::Black);//покрасили текст в красный
text.setStyle(Text::Bold);//жирный текст.
Image map_image;//объект изображения для карты
map_image.loadFromFile("images/map_new.png");//загружаем файл для карты
Texture map;//текстура карты
map.loadFromImage(map_image);//заряжаем текстуру картинкой
Sprite s_map;//создаём спрайт для карты
s_map.setTexture(map);//заливаем текстуру спрайтом
Clock clock;
Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
int gameTime = 0;//объявили игровое время, инициализировали.
Player p("hero.png", 250, 250, 96.0, 96.0);//создаем объект p класса player
int createObjectForMapTimer = 0;//Переменная под время для генерирования камней
while (window.isOpen())
{
 float time = clock.getElapsedTime().asMicroseconds();
 if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в
//секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо.
//оно не обновляет логику игры
clock.restart();
time = time / 800;
createObjectForMapTimer += time;//наращиваем таймер
if (createObjectForMapTimer>2000){
randomMapGenerate();//генерация камней
createObjectForMapTimer = 0;//обнуляем таймер
}
sf::Event event;
while (window.pollEvent(event))
{
if (event.type == sf::Event::Closed)
window.close();
}
p.update(time); //оживляем объект “p” класса “Player” с помощью времени sfml,
// передавая время в качестве параметра функции update.
window.clear();
/////////////////////////////Рисуем карту/////////////////////
for (int i = 0; i < HEIGHT_MAP; i++)
for (int j = 0; j < WIDTH_MAP; j++)
{
if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 48, 48));
if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(48, 0, 48, 48));
if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(96, 0, 48, 48));
if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(144, 0, 48, 48));//цветок
if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(192, 0, 48, 48));//сердце
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
window.display();
}
return 0;
}
