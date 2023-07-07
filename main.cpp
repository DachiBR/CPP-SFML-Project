#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#define SCREENX 800
#define SCREENY 600


class Transformable {
protected:
    sf::RectangleShape shape;
public:
    virtual void move() {};
    virtual void draw() {};
    virtual void update() {};
    void setPosition(sf::Vector2f& position) { shape.setPosition(position); }

    const sf::Vector2f& getPosition() { return shape.getPosition(); }

    sf::RectangleShape& getShape() { return shape; }

    const sf::RectangleShape& getShape() const { return shape; }


};

class Figure : public Transformable {
public:
    Figure(float sizeX, float sizeY, sf::Color color) {
        shape.setSize(sf::Vector2f(sizeX, sizeY));
        shape.setFillColor(color);
    }
};

class Bullet : public Figure {
private:
    sf::Vector2f velocity;
public:
    Bullet(float sizeX, float sizeY, sf::Color color, float speed) : Figure(sizeX, sizeY, color), velocity(speed, 0.0f) {}

    void move() { shape.move(velocity); }

    bool operator==(const Bullet& other) const { return shape.getPosition() == other.shape.getPosition() && velocity == other.velocity; }


};

class Player : public Figure {
public:
    Player() : Figure(0, 0, sf::Color::Transparent) {};
    Player(float sizeX, float sizeY, sf::Color color, const sf::Vector2f& position) : Figure(sizeX, sizeY, color) { shape.setPosition(position); }
};

class Target : public Figure {
public:
    Target() : Figure(0, 0, sf::Color::Transparent) {};

    Target(float sizeX, float sizeY, sf::Color color, const sf::Vector2f& position) : Figure(sizeX, sizeY, color) {
        shape.setPosition(position);
    }
};


class Game {
private:
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    shootBullet();
                }
                else if (event.key.code == sf::Keyboard::Q) {
                    quitGame();
                }
            }
        }
    }

    void shootBullet() {
        Bullet bullet(10, 10, sf::Color::White, bulletSpeed);
        sf::Vector2f bulletPosition = player.getPosition();
        bulletPosition.x += player.getShape().getSize().x;
        bulletPosition.y += player.getShape().getSize().y / 2 - bullet.getShape().getSize().y / 2;
        bullet.getShape().setPosition(bulletPosition);
        bullets.push_back(bullet);
    }

    void quitGame() {
        window.close();
    }

    void update() {
        sf::Vector2f position = player.getPosition();
        float speed = 3.0f;

        position.y += speed * direction;

        if (position.y <= 0 || position.y + player.getShape().getSize().y >= window.getSize().y)
            direction *= -1;

        player.setPosition(position);

        std::vector<Bullet> collidedBullets;

        for (auto& bullet : bullets) {
            bullet.move();

            if (bullet.getPosition().x > window.getSize().x)
                collidedBullets.push_back(bullet);


            for (auto& target : targets) {
                sf::FloatRect bulletBounds = bullet.getShape().getGlobalBounds();
                sf::FloatRect targetBounds = target.getShape().getGlobalBounds();

                if (bulletBounds.intersects(targetBounds) && !gameOver) {
                    sf::Vector2f bulletPosition = bullet.getPosition();
                    if (bulletPosition.x <= targetBounds.left && bulletPosition.y >= targetBounds.top &&
                        bulletPosition.y <= targetBounds.top + targetBounds.height) {
                        gameOver = true;
                    }

                }
            }


            sf::FloatRect bulletBounds = bullet.getShape().getGlobalBounds();
            sf::FloatRect scoreLineBounds = scoreLine.getGlobalBounds();

            if (bulletBounds.intersects(scoreLineBounds) && !gameOver) {
                collidedBullets.push_back(bullet);
                score += 5;
            }
        }


        for (const auto& collidedBullet : collidedBullets) {
            auto it = std::find(bullets.begin(), bullets.end(), collidedBullet);
            if (it != bullets.end())
                bullets.erase(it);
        }

    }

    void render() {
        window.clear();

        window.draw(player.getShape());

        for (const auto& target : targets) {
            window.draw(target.getShape());
        }

        for (const auto& bullet : bullets) {
            window.draw(bullet.getShape());
        }

        window.draw(scoreLine);

        if (gameOver) {
            window.close();
        }

        window.display();
    }

    sf::RenderWindow window;
    Player player;
    Target target;
    std::vector<Target> targets;
    std::vector<Bullet> bullets;
    sf::RectangleShape scoreLine;
    int direction;
    bool gameOver;
    int score;
    float bulletSpeed;
public:
    Game() : window(sf::VideoMode(SCREENX, SCREENY), "Go lets play") {
        window.setFramerateLimit(60);

        player = Player(50, 50, sf::Color::Blue, sf::Vector2f(100, 0));

        target = Target(30, 70, sf::Color::Red, sf::Vector2f(600, 0));

        for (float y = 0; y <= SCREENY; y += 100) {
            targets.push_back(Target(30, 70, sf::Color::Red, sf::Vector2f(600, y)));
        }

        const float bulletSpeed = 5.0f;
        this->bulletSpeed = bulletSpeed;

        direction = 1;

        gameOver = false;
        score = 0;

        scoreLine.setSize(sf::Vector2f(2, SCREENY));
        scoreLine.setFillColor(sf::Color::Green);
        scoreLine.setPosition(630, 0);
    }

    void run(){
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
        std::cout << "Your Score is: " << score << std::endl;
    }


};

int main() {
    Game game;
    game.run();
    return 0;
}
