#include "fallingitem.h"

FallingItem::FallingItem(Image &image, float x, float y, float w, float h, const std::string &name)
        : Entity(image, x, y, w, h, name) {
        this->dx = 0;
        this->dy = 0.05;
    }

void FallingItem::update(float time) {
        y += dy * time;
        sprite.setPosition(x, y);

        // Если предмет выходит за пределы экрана, помечаем его как "мертвый"
        if (y > 960) {
            life = false;
        }
    }
