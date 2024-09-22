#include "map.h"
#include "entity.h" // Подключаем код с классом сущностей
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "fallingitem.h"

void resetGame(Player &p, std::list<Entity*> &enemies, std::list<Entity*> &fallingItems, std::list<Entity*> &Bullets, Image &easyEnemyImage, sf::Clock &gameTimeClock) {
    // Сброс состояния игрока
    p.health = 100;
    p.currentsize = 1.0f;
    p.isSizeMax = false;
    p.w = 72;
    p.h = 72;
    p.x = 100;  // Начальная позиция
    p.y = 100;
    p.life = true;  // Игрок снова жив

    // Очистка списков врагов, предметов и пуль
    enemies.clear();
    fallingItems.clear();
    Bullets.clear();

    // Сброс врагов
    const int ENEMY_COUNT = 3; // максимальное количество врагов
    for (int i = 0; i < ENEMY_COUNT; i++) {
        float xr = 300 + rand() % 500; // случайная координата врага
        float yr = 300 + rand() % 350;
        enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
    }
    // Сброс игрового времени
       gameTimeClock.restart();  // Сбрасываем игровые часы для отсчета времени с нуля
}

int main()
{
    SoundBuffer music;
    music.loadFromFile("sound/music.wav"); // тут загружаем в буфер что-то
    Sound sound;
    sound.setBuffer(music);
    sound.setLoop(true);
    sound.play();

    SoundBuffer bullet;
    bullet.loadFromFile("sound/bubble.wav"); // тут загружаем в буфер что-то
    Sound sound2;
    sound2.setBuffer(bullet);

    SoundBuffer death;
    death.loadFromFile("sound/death.wav"); // тут загружаем в буфер что-то
    Sound sound3;
    sound3.setBuffer(death);

    SoundBuffer kill;
    kill.loadFromFile("sound/kill.wav"); // тут загружаем в буфер что-то
    Sound sound4;
    sound4.setBuffer(kill);

    SoundBuffer hum;
    hum.loadFromFile("sound/hum.wav"); // тут загружаем в буфер что-то
    Sound sound5;
    sound5.setBuffer(hum);

    SoundBuffer eatfish;
    eatfish.loadFromFile("sound/eatfish.wav"); // тут загружаем в буфер что-то
    Sound sound6;
    sound6.setBuffer(eatfish);

    SoundBuffer rush;
    rush.loadFromFile("sound/rush.wav"); // тут загружаем в буфер что-то
    Sound sound7;
    sound7.setBuffer(rush);

    float countdownTime = 0.0f;          // Время таймера (начинаем с 0)
        sf::Clock countdownClock;            // Часы для отсчета времени
        bool isCountdownActive = false;      // Флаг, активен ли таймер

    VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow window(VideoMode(1248, 960, desktop.bitsPerPixel), "FISH LEGENDS");

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

    int createFallingItemTimer = 0;
    std::list<Entity*> fallingItems;
    Image itemImage;
    itemImage.loadFromFile("images/item.png");  // Загружаем картинку для предмета
        // Загрузка фонов (три разных картинки)

     Image backgroundImage1, backgroundImage2, backgroundImage3;
     backgroundImage1.loadFromFile("images/bg.png");
     backgroundImage2.loadFromFile("images/bg2.png");
     backgroundImage3.loadFromFile("images/bg3.png");

     Texture backgroundTexture1, backgroundTexture2, backgroundTexture3;
     backgroundTexture1.loadFromImage(backgroundImage1);
     backgroundTexture2.loadFromImage(backgroundImage2);
     backgroundTexture3.loadFromImage(backgroundImage3);

     Sprite backgroundSprite1, backgroundSprite2, backgroundSprite3;
     backgroundSprite1.setTexture(backgroundTexture1);
     backgroundSprite2.setTexture(backgroundTexture2);
     backgroundSprite3.setTexture(backgroundTexture3);

     // Масштабируем фоны под размер окна
     backgroundSprite1.setScale(window.getSize().x / 1248.0f, window.getSize().y / 960.0f);
     backgroundSprite2.setScale(window.getSize().x / 1248.0f, window.getSize().y / 960.0f);
     backgroundSprite3.setScale(window.getSize().x / 1248.0f, window.getSize().y / 960.0f);

    Clock clock;

    Clock backgroundClock;
    float switchTime = 0.5f; // интервал смены фона в секундах
    int currentBackground = 0; // текущий фон (0, 1, 2)

    Clock gameTimeClock; // переменная игрового времени
    int gameTime = 0; // игровое время

    Image heroImage;
    heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага
    Image BulletImage; // изображение для пули
    BulletImage.loadFromFile("images/bullet.png"); // загрузили картинку в объект изображения

    Player p(heroImage, 100, 100, 72, 72, "Player1"); // объект класса игрока
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
        if (p.currentsize >= 1.5f && !isCountdownActive) {
                       isCountdownActive = true;    // Активируем таймер
                       countdownTime = 8.0f;        // Устанавливаем время на 5 секунд
                       countdownClock.restart();    // Сбрасываем часы
                       sound7.play();
                   }

                   // Если таймер активен, обновляем его
                   if (isCountdownActive) {
                       float elapsed = countdownClock.getElapsedTime().asSeconds();  // Получаем время с момента последнего кадра

                       countdownTime -= elapsed;    // Уменьшаем таймер
                       countdownClock.restart();    // Сбрасываем часы для следующего кадра

                       // Когда время истекает
                       if (countdownTime <= 1.0f) {
                           countdownTime = 1.0f;    // Ограничиваем минимальное значение
                           isCountdownActive = false; // Деактивируем таймер
                       }
                   }

                   // Форматируем вывод оставшегося времени
                   std::ostringstream countdownString;
                   countdownString << static_cast<int>(countdownTime);  // Приводим к целому для отображения секунд

                   // Отображаем таймер на экране
                   Text countdownText("RAGE EFFECT:" + countdownString.str(), font, 30);  // Создаем текст для таймера
                   countdownText.setColor(Color::Red);                 // Устанавливаем цвет текста
                   countdownText.setPosition(50, 150);                   // Устанавливаем позицию текста

                   // Рисуем текст таймера на экране
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

        if (backgroundClock.getElapsedTime().asSeconds() > switchTime) {
                            currentBackground = (currentBackground + 1) % 3; // переключаем между 0, 1 и 2
                            backgroundClock.restart(); // сбрасываем таймер
                        }

                        // Очищаем окно
                        window.clear();

                        // Рисуем фон в зависимости от текущего индекса фона
                        if (currentBackground == 0) {
                            window.draw(backgroundSprite1);
                        } else if (currentBackground == 1) {
                            window.draw(backgroundSprite2);
                        } else if (currentBackground == 2) {
                            window.draw(backgroundSprite3);
                        }


                        if (!gameOver) {  // Спавн новых предметов только если игра не окончена
                                            createFallingItemTimer += time;
                                            if (createFallingItemTimer > 2000) {
                                                float xr = 48 + rand() % (window.getSize().x - 48 - 32);  // Случайная координата X с учетом границ
                                                fallingItems.push_back(new FallingItem(itemImage, xr, -50, 32, 32, "FallingItem"));
                                                createFallingItemTimer = 0;
                                            }
                                        }

                                                for (auto it = fallingItems.begin(); it != fallingItems.end(); ) {
                                                    if (!gameOver) {
                                                            (*it)->update(time);  // Обновляем предметы только если игра не окончена
                                                        }

                                                    if ((*it)->getRect().intersects(p.getRect())) {
                                                        p.increaseSize();
                                                        (*it)->life = false;
                                                        sound5.play();
                                                    }

                                                    if ((*it)->getRect().left < 48 || (*it)->getRect().left + (*it)->getRect().width > window.getSize().x - 48) {
                                                        (*it)->life = false;
                                                    }

                                                    if (!(*it)->life) {
                                                        it = fallingItems.erase(it);
                                                    } else {
                                                        window.draw((*it)->sprite);
                                                        ++it;
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
                                if ((p.getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy") && (p.currentsize < 1.5)) {
                                    p.health = 0;
                                    p.life = false;
                                    gameOver = true; // устанавливаем флаг "игра окончена"
                                    sound3.play();

                                }
                                else if ((p.getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy") && (p.currentsize >= 1.5)) {
                                       (*it)->life = false;  // Враг умирает
                                    sound6.play();

                                }
                            }
                        }
                        if (p.currentsize >= 1.5f && !p.isSizeMax) {
                            p.isSizeMax = true;  // Фиксируем достижение размера
                            p.sizeReachedTime = gameTimeClock.getElapsedTime().asSeconds();  // Запоминаем время
                        }

                        if (p.isSizeMax && gameTimeClock.getElapsedTime().asSeconds() - p.sizeReachedTime >= 7.0f) {
                            p.currentsize = 1.0f;  // Возвращаем исходный размер
                            p.isSizeMax = false;     // Сбрасываем флаг
                            p.w = 72;
                            p.h = 72;
                            p.sprite.setScale(p.w / p.originalWidth, p.h / p.originalHeight);
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

        // Объявляем переменные здоровья и времени
        std::ostringstream playerHealthString, gameTimeString, playerSize;
        playerHealthString << p.health;
        gameTimeString << gameTime;
        playerSize << p.currentsize;

        text.setString("Health: " + playerHealthString.str() + "\nTime: " + gameTimeString.str() + "\nSize: " + playerSize.str());
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

        if (p.currentsize >= 1.5f){
                window.draw(countdownText);}

                window.display();

        if (gameOver) {
            window.draw(gameOverText);
        }
        if (gameOver) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                        gameOver = false;  // Сбрасываем флаг Game Over
                        resetGame(p, enemies, fallingItems, Bullets, easyEnemyImage, gameTimeClock);  // Сбрасываем игру
                    }
                }

    }

    return 0;
}
