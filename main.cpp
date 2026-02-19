#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cout << "Usage: image-viewer.exe <path-to-image>" << std::endl;
        return 0;
    }

    std::string imagePath = argv[1];

    sf::Texture texture;
    if (!texture.loadFromFile(imagePath )){
        std::cout<<"Image failed to load\n";
        return 1;
    }

    sf::Sprite sprite(texture);

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Title");

    float imgW = static_cast<float>(texture.getSize().x);
    float imgH = static_cast<float>(texture.getSize().y);
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float scale = std::min(winW / imgW, winH / imgH);
    
    sprite.setOrigin({imgW/2.f,imgH/2.f});
    sprite.setScale({scale,scale});
    sprite.setPosition({winW/2.f,winH/2.f});

    while (window.isOpen()){
        while (auto event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }

            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                sf::FloatRect visibleArea({0, 0},
                {static_cast<float>(resized->size.x),
                static_cast<float>(resized->size.y)});
                window.setView(sf::View(visibleArea));

                float imgW = static_cast<float>(texture.getSize().x);
                float imgH = static_cast<float>(texture.getSize().y);
                float winW = static_cast<float>(window.getSize().x);
                float winH = static_cast<float>(window.getSize().y);
                float scale = std::min(winW / imgW, winH / imgH);
                
                sprite.setOrigin({imgW/2.f,imgH/2.f});
                sprite.setScale({scale,scale});
                sprite.setPosition({winW/2.f,winH/2.f});
                
            }
        }

        window.clear(sf::Color(0,0,0));

        window.draw(sprite);

        window.display();
    }

    return 0;
}