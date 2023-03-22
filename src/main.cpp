#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "VideoHub.h"
#include "VideoHubRender.h"

int main() {
    std::shared_ptr<VideoHub> videohub(new VideoHub(DeviceStatus::connected, "TestHub", 4, 5));
    std::vector<LockStatus> outputLocks = {LockStatus::unlocked, LockStatus::unlocked, LockStatus::unlocked, LockStatus::unlocked, LockStatus::owned};
    std::vector<std::string> inputNames = {"Cam 101", "MD Cam", "FOH IR", "Really long one to check"};
    std::vector<std::string> outputNames = {"DSM Left", "SFX", "Auto 1", "Really long one to check2", "Another one"};
    std::vector<std::uint8_t> portRouting = {0,1,2,0,3};

    videohub->setVideoInputLabels(inputNames);
    videohub->setVideoOutputLabels(outputNames);
    videohub->setVideoOutputLocks(outputLocks);
    videohub->setVideoOutputRouting(portRouting);


    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800,700)), "SFML Might Work!");
    sf::Font monofont;
    if (!monofont.loadFromFile(".\\resources\\fonts\\VeraMono.ttf")) {
    //    // Todo: handle exception
    }

    VideoHubRender videohubrender(videohub, monofont, sf::Vector2u(800, 700));

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }
        window.clear();

        // Draw stuff here
        videohubrender.render(&window);

        window.display();
    }
    return 0;
}