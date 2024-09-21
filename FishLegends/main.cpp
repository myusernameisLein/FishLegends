#include "map.h"
#include "entity.h" // Подключаем код с классом сущностей
#include "player.h"
#include "bullet.h"
#include "enemy.h"

int main()
{
    sf::SoundBuffer music;
    music.loadFromFile("sound/music.wav"); // тут загружаем в буфер что-то
    sf::Sound sound;
    sound.setBuffer(music);
    sound.setLoop(true);
    sound.play();

    sf::SoundBuffer bullet;
    bullet.loadFromFile("sound/bubble.wav"); // тут загружаем в буфер что-то
    sf::Sound sound2;
    sound2.setBuffer(bullet);

    sf::SoundBuffer death;
    death.loadFromFile("sound/death.wav"); // тут загружаем в буфер что-то
    sf::Sound sound3;
    sound3.setBuffer(death);

    sf::SoundBuffer kill;
    kill.loadFromFile("sound/kill.wav"); // тут загружаем в буфер что-то
    sf::Sound sound4;
    sound4.setBuffer(kill);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1248, 960, desktop.bitsPerPixel), "FISH LEGENDS");

    Font font; // шрифт
    font.loadFromFile("Pixel Cyr.ttf"); // передаем нашему шрифту файл шрифта

    auto image = sf::Image{};
    if (!image.loadFromFile("fish.png"))
    {
    }

    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
    // ...

    Text text("", font, 20); // создаем объект текст
    text.setColor(Color::Black); // покрасили текст в черный
    text.setStyle(Text::Regular); // обычный текст

    // Добавляем текст "GAME OVER"
    Text gameOverText("", font, 150);
    gameOverText.setColor(Color::Black);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(250, 350);

    Image map_image; // объект изображения для карты
    map_image.loadFromFile("images/map_new.png"); // загружаем файл для карты
    Texture map; // текстура карты
    map.loadFromImage(map_image); // заряжаем текстуру картинкой
    Sprite s_map; // создаём спрайт для карты
    s_map.setTexture(map); // заливаем текстуру спрайтом

    Clock clock;
    Clock gameTimeClock; // переменная игрового времени
    int gameTime = 0; // игровое время

    Image heroImage;
    heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага
    Image BulletImage; // изображение для пули
    BulletImage.loadFromFile("images/bullet.png"); // загрузили картинку в объект изображения

    Player p(heroImage, 100, 100, 96, 96, "Player1"); // объект класса игрока
    std::list<Entity*> enemies; // список врагов
    std::list<Entity*> Bullets; // список пуль
    std::list<Entity*>::iterator it; // итератор для прохода по элементам списка
    std::list<Entity*>::iterator deathenemy;

    const int ENEMY_COUNT = 3; // максимальное количество врагов
    int enemiesCount = 0; // текущее количество врагов

    // Заполняем список объектами врагов
    for (int i = 0; i < ENEMY_COUNT; i++) {
        float xr = 300 + rand() % 500; // случайная координата врага
        float yr = 300 + rand() % 350;
        enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
        enemiesCount += 1; // увеличили счетчик врагов
    }

    bool gameOver = false; // флаг состояния "игра окончена"

    int createObjectForMapTimerEnemy = 0;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        if (!gameOver && p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); // игровое время
        clock.restart();
        time = time / 600;
        createObjectForMapTimerEnemy += time;

        if (!gameOver) {
            if (createObjectForMapTimerEnemy > 4000) {
                        float xr = 300 + rand() % 500;
                        float yr = 300 + rand() % 350;
                        enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
                        enemiesCount += 1;
                        createObjectForMapTimerEnemy = 0;
                    }
           }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Стреляем по нажатию клавиши "Space"
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !gameOver)
            {
                Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state));
                sound2.play();
            }
        }

        if (!gameOver) {
            // Оживляем объекты только если игра не окончена
            p.update(time);

            // Оживляем врагов
            for (it = enemies.begin(); it != enemies.end(); it++) {
                (*it)->update(time);
            }

            // Оживляем пули
            for (it = Bullets.begin(); it != Bullets.end(); it++) {
                (*it)->update(time);
            }

            // Проверка пересечения игрока с врагами
            if (p.life) {
                for (it = enemies.begin(); it != enemies.end(); it++) {
                    if ((p.getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy")) {
                        p.health = 0;
                        p.life = false;
                        gameOver = true; // устанавливаем флаг "игра окончена"
                        sound3.play();

                    }
                }
            }

            // Пересечение пули с врагом
            for (deathenemy = enemies.begin(); deathenemy != enemies.end(); deathenemy++) {
                for (it = Bullets.begin(); it != Bullets.end(); it++) {
                    if (((*it)->getRect().intersects((*deathenemy)->getRect())) &&
                        ((*deathenemy)->name == "EasyEnemy") && ((*it)->name == "Bullet")) {

                        (*deathenemy)->health = 0;
                        (*deathenemy)->life = false;
                        (*it)->life = false;
                        sound4.play();
                    }
                }
            }

            // Удаление мёртвых врагов и пуль
            for (deathenemy = enemies.begin(); deathenemy != enemies.end(); ) {
                if (!(*deathenemy)->life) {
                    deathenemy = enemies.erase(deathenemy);
                    enemiesCount -= 1;
                } else {
                    ++deathenemy;
                }
            }

            for (it = Bullets.begin(); it != Bullets.end(); ) {
                if (!(*it)->life) {
                    it = Bullets.erase(it);
                } else {
                    ++it;
                }
            }
        }

        window.clear();

        // Рисуем карту
        for (int i = 0; i < HEIGHT_MAP; i++) {
            for (int j = 0; j < WIDTH_MAP; j++) {
                if (p.TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 48, 48));
                if ((p.TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(96, 0, 48, 48));
                s_map.setPosition(j * 48, i * 48);
                window.draw(s_map);
            }
        }

        // Объявляем переменные здоровья и времени
        std::ostringstream playerHealthString, gameTimeString;
        playerHealthString << p.health;
        gameTimeString << gameTime;

        text.setString("Health: " + playerHealthString.str() + "\nTime: " + gameTimeString.str());
        text.setPosition(50, 50);
        window.draw(text);

        // Рисуем игрока
        if (p.life) {
            window.draw(p.sprite);
        }
        if (p.life || gameOver) {
            window.draw(p.sprite);
        }


        // Рисуем врагов
        for (it = enemies.begin(); it != enemies.end(); it++) {
            if ((*it)->life)
                window.draw((*it)->sprite);
        }

        // Рисуем пули
        for (it = Bullets.begin(); it != Bullets.end(); it++) {
            if ((*it)->life)
                window.draw((*it)->sprite);
        }

        // Отображаем текст "GAME OVER", если игра окончена
        if (p.health <= 0){ p.life = false; gameOver = true;}//если жизней меньше 0, либо равно 0, то умираем
        if (gameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
