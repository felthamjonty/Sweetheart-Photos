#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <filesystem>

int main(int argc, char* argv[]){
    sf::RenderWindow window(sf::VideoMode({171, 258}), "Photograph", sf::Style::None);
    window.setFramerateLimit(60);
    
    std::string parentFolder = std::filesystem::path(argv[0]).parent_path().string();
    
    sf::Image icon;
    if (icon.loadFromFile(parentFolder+"/image/icon.png")){
        window.setIcon({icon.getSize().x, icon.getSize().y}, icon.getPixelsPtr());
    }
    
    sf::Texture bgImage;
    if (!bgImage.loadFromFile(parentFolder+"/image/photo.png")){
        std::cout<<"UI failed to load\n";
        return 1;
    }

    sf::Font font;
    if (!font.openFromFile(parentFolder+"/font/Marker-Regular.otf")){
        std::cout << "Could not load font!" << std::endl;
        return 1;
    }

    sf::Sprite bgSprite(bgImage);

    if (argc >= 2){
        std::string imagePath = argv[1];

        sf::Texture texture;
        if (!texture.loadFromFile(imagePath )){
            std::cout<<"Image failed to load\n";
            return 1;
        }
        sf::Sprite sprite(texture);

        sprite.setScale({151.f/texture.getSize().x,198.f/texture.getSize().y});
        sprite.setPosition({10.f,15.f});

        std::string textS = argv[1];
        bool stringStart=true;
        std::string correctedText ="";
        for (int i = textS.length()-1; i >= 0; i--){
            if (textS[i]=='\\'||textS[i]=='/'){
                stringStart=i;
                break;
            }
            if(stringStart&&textS[i]=='.'){
                stringStart=false;
            }else if(!stringStart){
                correctedText=textS[i]+correctedText;
            }
        }

        transform(correctedText.begin(), correctedText.end(), correctedText.begin(), ::toupper);

        sf::Text text(font);
        text.setString(correctedText);
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::Black);
        text.setOrigin({text.getLocalBounds().position.x+text.getLocalBounds().size.x/2,text.getLocalBounds().position.y+text.getLocalBounds().size.y/2});
        text.setPosition({85.f, 235.f});

        while (window.isOpen()){
            while (auto event = window.pollEvent()){
                if (event->is<sf::Event::Closed>())window.close();
            }

            window.clear(sf::Color::Black);

            window.draw(bgSprite);
            window.draw(text);
            window.draw(sprite);

            window.display();
        }

        return 0;
    }else{
        while (window.isOpen()){
            while (auto event = window.pollEvent()){
                if (event->is<sf::Event::Closed>())window.close();
            }

            window.clear(sf::Color::Black);
            window.draw(bgSprite);
            window.display();
        }

        return 0;
    }
}