#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Game
{
public:
    Game() : window(sf::VideoMode(3000, 2000), "S-G-S")
    {
        if (!backgroundTexture.loadFromFile("./additional_files/background.jpg"))
            // Handle error if the image file cannot be loaded

        background.setTexture(backgroundTexture);

        if (!backgroundMusic.openFromFile("./additional_files/music.wav"))
            // Handle error if the music file cannot be loaded

        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }

    void run()
    {
        while (window.isOpen())
        {
            processEvents();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Music backgroundMusic;

    void processEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void render()
    {
        window.clear();
        window.draw(background);
        window.display();
    }
};

int main()
{
    Game game;
    game.run();

    return 0;
}
