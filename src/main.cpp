#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "VideoHub.h"
#include "VideoHubRender.h"

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include "spdlog/spdlog.h"

int main() {
    std::shared_ptr<VideoHub> videohub(new VideoHub(DeviceStatus::connected, "MTX201", 40, 40));
    std::vector<LockStatus> outputLocks = {LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,
                                           LockStatus::unlocked,};
    std::vector<std::string> inputNames = {"CAM201 (FOH Col)",
                                           "CAM202 (FOH PTZ)",
                                           "CAM211 (FOH IR)",
                                           "CAM212 (FOH Iron)",
                                           "",
                                           "",
                                           "CAM221 (LX1 O/H)",
                                           "CAM222 (X4 O/H)",
                                           "CAM231 (Wing SR)",
                                           "CAM241 (Bay1 SR)",
                                           "CAM243 (Bay3 SR)",
                                           "",
                                           "",
                                           "",
                                           "CAM251 (Bay1 SL)",
                                           "CAM253 (Bay3 SL)",
                                           "Cam232 (Wing SL)",
                                           "",
                                           "",
                                           "",//
                                           "CAM233 (Garage)",
                                           "Zabbix",
                                           "Arias MVW",
                                           "",
                                           "LX Control",
                                           "",
                                           "",
                                           "",
                                           "",
                                           "COL/BW Switch (op16)",
                                           "MD",
                                           "SRV111 Loop",
                                           "MVW FOH",
                                           "",
                                           "",
                                           "MVW STAGE",
                                           "MVW DSM",
                                           "MVW Auto",
                                           "",
                                           ""};
    std::vector<std::string> outputNames = {"CAM201",
                                            "CAM202",
                                            "CAM211",
                                            "CAM212",
                                            "CAM221",
                                            "CAM222",
                                            "CAM231",
                                            "CAM232",
                                            "CAM233",
                                            "CAM241",
                                            "CAM243",
                                            "CAM251",
                                            "CAM253",
                                            "MD",
                                            "FOH Switch (to MTX202)",
                                            "FOH Switch (Col/BW)",
                                            "DSM PiP",
                                            "Auto PiP",
                                            "",
                                            "",//
                                            "Local Mon RK102",
                                            "SRV111 (AudEmb)",
                                            "SND 1",
                                            "SND 2",
                                            "LX 1 (Show)",
                                            "LX 2 (Show)",
                                            "AUTO 1",
                                            "AUTO 2",
                                            "MON SR",
                                            "",
                                            "DSM 1",
                                            "DSM 2",
                                            "MON SL",
                                            "Unknown",
                                            "Dock Office",
                                            "Dock SL",
                                            "Local Mon RK101",
                                            "SFX Monitor",
                                            "Auto Office",
                                            "Lazy Audience TV"};
    std::vector<std::uint8_t> portRouting = {0,
                                             1,
                                             2,
                                             28,
                                             6,
                                             7,
                                             8,
                                             16,
                                             20,
                                             9,
                                             10,
                                             14,
                                             15,
                                             30,
                                             29,
                                             0,
                                             30,
                                             16,
                                             0,
                                             4,
                                             3,
                                             35,
                                             1,
                                             0,
                                             1,
                                             35,
                                             37,
                                             35,
                                             2,
                                             32,
                                             35,
                                             36,
                                             2,
                                             1,
                                             0,
                                             2,
                                             0,
                                             2,
                                             37,
                                             28,};

    videohub->setVideoInputLabels(inputNames);
    videohub->setVideoOutputLabels(outputNames);
    videohub->setVideoOutputLocks(outputLocks);
    videohub->setVideoOutputRouting(portRouting);

    spdlog::info("Test log message");


    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1200,1000)), "Blackmagic Videohub");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Font monofont;
    if (!monofont.loadFromFile(".\\resources\\fonts\\VeraMono.ttf")) {
    //    // Todo: handle exception
    }

    sf::Clock deltaClock;

    VideoHubRender videohubrender(videohub, monofont, sf::Vector2u(1200, 1000));

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();

        // Draw stuff here
        videohubrender.render(&window);
        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}