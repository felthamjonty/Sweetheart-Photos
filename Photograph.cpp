#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <filesystem>
#include <windows.h>

enum menuSelected{
    Top,
    Bottom,
    Exit
};

int main(int argc, char* argv[]){
    sf::RenderWindow window(sf::VideoMode({171, 258}), "Photograph", sf::Style::None);
    window.setFramerateLimit(60);

    HWND hwnd = window.getNativeHandle();
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    
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
    sf::Font fontA;
    if (!fontA.openFromFile(parentFolder+"/font/Arial.ttf")){
        std::cout << "Could not load font!" << std::endl;
        return 1;
    }

    sf::Sprite bgSprite(bgImage);

    bool dragging = false;
    sf::Vector2i dragOffset;

    sf::RectangleShape rectangle({80.f,62.f});
    rectangle.setFillColor(sf::Color(225.f,225.f,225.f));
    bool menuOpen=false;
    sf::RectangleShape rectangleTop({74.f,18.f});
    rectangleTop.setFillColor(sf::Color(195.f,195.f,195.f));
    menuSelected menuselected = Top;

    sf::Text textTop(fontA);
    textTop.setString("Above Windows");
    textTop.setCharacterSize(10);
    textTop.setFillColor(sf::Color::Black);
    sf::Text textBottom(fontA);
    textBottom.setString("Below Windows");
    textBottom.setCharacterSize(10);
    textBottom.setFillColor(sf::Color::Black);
    sf::Text textExit(fontA);
    textExit.setString("Exit");
    textExit.setCharacterSize(10);
    textExit.setFillColor(sf::Color::Black);

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
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        text.setOrigin({text.getLocalBounds().position.x+text.getLocalBounds().size.x/2,text.getLocalBounds().position.y+text.getLocalBounds().size.y/2});
        text.setPosition({85.f, 235.f});

        int w=24;

        while (text.getLocalBounds().size.x>150&&w>=10){
            w--;
            text.setCharacterSize(w);
            text.setOrigin({text.getLocalBounds().position.x+text.getLocalBounds().size.x/2,text.getLocalBounds().position.y+text.getLocalBounds().size.y/2});
        }

        while (window.isOpen()){
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();

                if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()){
                    float xPoz = sf::Mouse::getPosition().x-window.getPosition().x;
                    float yPoz = sf::Mouse::getPosition().y-window.getPosition().y;
                    
                    if (mousePress->button == sf::Mouse::Button::Left){
                        if (menuOpen && rectangle.getPosition().x<xPoz&&xPoz<rectangle.getPosition().x+rectangle.getSize().x && rectangle.getPosition().y<yPoz&&yPoz<rectangle.getPosition().y+rectangle.getSize().y){
                            switch(menuselected){
                                case Top:
                                    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                                    break;
                                case Bottom:
                                    SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                                    break;
                                case Exit:
                                    window.close();
                                    break;
                            }
                            menuOpen=false;
                        }else{
                            dragging = true;
                            dragOffset = sf::Mouse::getPosition() - window.getPosition();
                            menuOpen=false;
                        }
                    }

                    if (mousePress->button == sf::Mouse::Button::Right){
                        if (menuOpen){
                            menuOpen=false;
                        }else{
                            menuOpen=true;
                            float xPoz = sf::Mouse::getPosition().x-window.getPosition().x;
                            float yPoz = sf::Mouse::getPosition().y-window.getPosition().y;
                            xPoz = std::min(xPoz,window.getSize().x-rectangle.getSize().x);
                            yPoz = std::min(yPoz,window.getSize().y-rectangle.getSize().y);
                            rectangle.setPosition({xPoz,yPoz});
                            rectangleTop.setPosition({xPoz+3.f,yPoz+3.f});
                            textTop.setPosition({xPoz+5.f,yPoz+5.f});
                            textBottom.setPosition({xPoz+5.f,yPoz+25.f});
                            textExit.setPosition({xPoz+5.f,yPoz+45.f});
                        }
                    }

                    if (mousePress->button == sf::Mouse::Button::Middle) window.close();
                }

                if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()){
                    if (mouseRelease->button == sf::Mouse::Button::Left) dragging = false;
                }

                if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()){
                    if (dragging) window.setPosition(sf::Mouse::getPosition() - dragOffset);

                    float xPoz = sf::Mouse::getPosition().x-window.getPosition().x;
                    float yPoz = sf::Mouse::getPosition().y-window.getPosition().y;
                    if (menuOpen && rectangle.getPosition().x<xPoz&&xPoz<rectangle.getPosition().x+rectangle.getSize().x && rectangle.getPosition().y<yPoz&&yPoz<rectangle.getPosition().y+rectangle.getSize().y){
                        rectangleTop.setFillColor(sf::Color(195.f,195.f,195.f));
                        if(yPoz<rectangle.getPosition().y+22.f){
                            rectangleTop.setPosition({rectangle.getPosition().x+3.f,rectangle.getPosition().y+3.f});
                            menuselected=Top;
                        }else if(yPoz<rectangle.getPosition().y+42.f){
                            rectangleTop.setPosition({rectangle.getPosition().x+3.f,rectangle.getPosition().y+23.f});
                            menuselected=Bottom;
                        }else{
                            rectangleTop.setPosition({rectangle.getPosition().x+3.f,rectangle.getPosition().y+43.f});
                            menuselected=Exit;
                        }
                    }else{
                        rectangleTop.setFillColor(sf::Color(225.f,225.f,225.f));
                    }
                }

                if (const auto* unfocused = event->getIf<sf::Event::FocusLost>())
                {
                    menuOpen=false;
                }
            }

            window.clear(sf::Color::Black);

            window.draw(bgSprite);
            window.draw(text);
            window.draw(sprite);
            if (menuOpen){
                window.draw(rectangle);
                window.draw(rectangleTop);
                window.draw(textTop);
                window.draw(textBottom);
                window.draw(textExit);
            }

            window.display();
        }

        return 0;
    }else{
        while (window.isOpen()){
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();

                if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()){
                    float xPoz = sf::Mouse::getPosition().x-window.getPosition().x;
                    float yPoz = sf::Mouse::getPosition().y-window.getPosition().y;
                    
                    if (mousePress->button == sf::Mouse::Button::Left){
                        if (menuOpen && rectangle.getPosition().x<xPoz&&xPoz<rectangle.getPosition().x+rectangle.getSize().x && rectangle.getPosition().y<yPoz&&yPoz<rectangle.getPosition().y+rectangle.getSize().y){
                            switch(menuselected){
                                case Top:
                                    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                                    break;
                                case Bottom:
                                    SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                                    break;
                                case Exit:
                                    window.close();
                                    break;
                            }
                            menuOpen=false;
                        }else{
                            dragging = true;
                            dragOffset = sf::Mouse::getPosition() - window.getPosition();
                            menuOpen=false;
                        }
                    }

                    if (mousePress->button == sf::Mouse::Button::Right){
                        if (menuOpen){
                            menuOpen=false;
                        }else{
                            menuOpen=true;
                            float xPoz = sf::Mouse::getPosition().x-window.getPosition().x;
                            float yPoz = sf::Mouse::getPosition().y-window.getPosition().y;
                            xPoz = std::min(xPoz,window.getSize().x-rectangle.getSize().x);
                            yPoz = std::min(yPoz,window.getSize().y-rectangle.getSize().y);
                            rectangle.setPosition({xPoz,yPoz});
                            rectangleTop.setPosition({xPoz+3.f,yPoz+3.f});
                            textTop.setPosition({xPoz+5.f,yPoz+5.f});
                            textBottom.setPosition({xPoz+5.f,yPoz+25.f});
                            textExit.setPosition({xPoz+5.f,yPoz+45.f});
                        }
                    }

                    if (mousePress->button == sf::Mouse::Button::Middle) window.close();
                }

                if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()){
                    if (mouseRelease->button == sf::Mouse::Button::Left) dragging = false;
                }

                if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()){
                    if (dragging) window.setPosition(sf::Mouse::getPosition() - dragOffset);

                    float xPoz = sf::Mouse::getPosition().x-window.getPosition().x;
                    float yPoz = sf::Mouse::getPosition().y-window.getPosition().y;
                    if (menuOpen && rectangle.getPosition().x<xPoz&&xPoz<rectangle.getPosition().x+rectangle.getSize().x && rectangle.getPosition().y<yPoz&&yPoz<rectangle.getPosition().y+rectangle.getSize().y){
                        rectangleTop.setFillColor(sf::Color(195.f,195.f,195.f));
                        if(yPoz<rectangle.getPosition().y+22.f){
                            rectangleTop.setPosition({rectangle.getPosition().x+3.f,rectangle.getPosition().y+3.f});
                            menuselected=Top;
                        }else if(yPoz<rectangle.getPosition().y+42.f){
                            rectangleTop.setPosition({rectangle.getPosition().x+3.f,rectangle.getPosition().y+23.f});
                            menuselected=Bottom;
                        }else{
                            rectangleTop.setPosition({rectangle.getPosition().x+3.f,rectangle.getPosition().y+43.f});
                            menuselected=Exit;
                        }
                    }else{
                        rectangleTop.setFillColor(sf::Color(225.f,225.f,225.f));
                    }
                }

                if (const auto* unfocused = event->getIf<sf::Event::FocusLost>())
                {
                    menuOpen=false;
                }
            }

            window.clear(sf::Color::Black);
            window.draw(bgSprite);
            if (menuOpen){
                window.draw(rectangle);
                window.draw(rectangleTop);
                window.draw(textTop);
                window.draw(textBottom);
                window.draw(textExit);
            }
            window.display();
        }

        return 0;
    }
}