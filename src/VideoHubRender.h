//
// Created by samla on 21/03/2023.
//

#ifndef VIDEOHUB_VIDEOHUBRENDER_H
#define VIDEOHUB_VIDEOHUBRENDER_H

#include <memory>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "VideoHub.h"

class VideoHubRender {
    std::shared_ptr<VideoHub> videohub;
    sf::Font font;
    sf::Text inputText, outputText;
    bool dirty = true; // Until we do flag checking this will remain true

    sf::RenderTexture videohubTexture;
    sf::Vector2<unsigned int> videohubTextureSize;

    // Test properties (in pixels)
    int margin = 20;
    int textSize = 16;
    int lineThickness = 3;

    sf::FloatRect inputTextSize, outputTextSize;



    void render() {
        videohubTexture.clear();
        // Draw debug margins
        sf::RectangleShape rect;
        sf::RectangleShape rect2;
        rect.setFillColor(sf::Color::Red);
        rect.setSize(sf::Vector2f(margin, videohubTextureSize.y));
        rect.setPosition(sf::Vector2f(0.f, 0.f));
        videohubTexture.draw(rect);
        rect.setPosition(sf::Vector2f(videohubTextureSize.x - margin, 0.f));
        videohubTexture.draw(rect);


        rect.setSize(sf::Vector2f(videohubTextureSize.x, margin));
        rect.setPosition(sf::Vector2f(0.f, 0.f));
        videohubTexture.draw(rect);
        rect.setPosition(sf::Vector2f(0.f, videohubTextureSize.y - margin));
        videohubTexture.draw(rect);

        rect.setFillColor(sf::Color::Cyan);
        rect.setPosition(sf::Vector2f(margin, margin));
        rect.setSize(sf::Vector2f(inputTextSize.width, outputTextSize.width));
        videohubTexture.draw(rect);

        inputText.setCharacterSize(textSize);
        inputText.setFont(font);
        outputText.setCharacterSize(textSize);
        outputText.setFont(font);

        // Determine longest input word
        inputText.setString(videohub->longestInputLabel());
        inputTextSize = inputText.getLocalBounds();


        // Determine longest output word
        outputText.setString(videohub->longestOutputLabel());
        outputTextSize = outputText.getLocalBounds();

        float inputWordGap  = (videohubTextureSize.y - (2 * margin + outputTextSize.width + (videohub->inputs() - 1) * inputTextSize.height * 1.2)) / (videohub->inputs() - 1);


        float outputWordGap = (videohubTextureSize.x - (2 * margin + inputTextSize.width + (videohub->outputs() - 1) * outputTextSize.height * 1.2)) / (videohub->outputs() - 1);



        // Draw the inputs
        //rect.setFillColor(sf::Color::Blue);
        //rect.setSize(sf::Vector2f(inputTextSize.width, inputTextSize.height));

        //rect2.setFillColor(sf::Color::Green);
        //rect2.setSize(sf::Vector2f(inputTextSize.width, inputWordGap - inputTextSize.height));

        sf::Vector2f textPosition;
        //sf::Vector2f rect2Position;
        for (int i = 0; i < videohub->inputs(); i++) {
            std::string str = videohub->getVideoInputLabel(i);
            inputText.setString(str);
            textPosition.y = 2 * margin + outputTextSize.width + i * inputWordGap;
            textPosition.x = margin + (inputTextSize.width - inputText.getLocalBounds().width);
            inputText.setPosition(textPosition);


            //rect.setPosition(textPosition);
            //rect2Position.x = margin;
            //rect2Position.y = margin + outputTextSize.width + i * inputWordGap + inputTextSize.height * 1.2;

            //rect2.setPosition(rect2Position);
            //videohubTexture.draw(rect2);

            videohubTexture.draw(inputText);

        }


        // Draw the Outputs
        outputText.setRotation(sf::degrees(270));

        for (int i = 0; i < videohub->outputs(); i++) {
            std::string str = videohub->getVideoOutputLabel(i);
            outputText.setString(str);
            textPosition.y = margin + outputTextSize.width;
            textPosition.x = 2 * margin + inputTextSize.width + outputWordGap * i;
            outputText.setPosition(textPosition);
            videohubTexture.draw(outputText);
        }

        // Draw the lines
        sf::RectangleShape horizLine, vertLine;
        horizLine.setFillColor(sf::Color::White);
        vertLine.setFillColor(sf::Color::White);
        for (int output = 0; output < videohub->outputs(); output++) {
            int input = videohub->getVideoOutputRouting(output);
            float inputHeight = 2 * margin + outputTextSize.width + (output) * inputWordGap + inputTextSize.height * 0.5 - lineThickness / 2;
            float outputWidth = 2 * margin + inputTextSize.width + (input) * outputWordGap + outputTextSize.height * 0.5 - lineThickness / 2;

            horizLine.setSize(sf::Vector2f( 50, lineThickness)); //outputWidth - (2 * margin - inputTextSize.width)
            horizLine.setPosition(sf::Vector2f(margin + inputTextSize.width, inputHeight));

            vertLine.setSize(sf::Vector2f(lineThickness, 50)); // inputHeight - (2 * margin - outputTextSize.width)
            vertLine.setPosition(sf::Vector2f(outputWidth, 2*margin + outputTextSize.width));

            videohubTexture.draw(horizLine);
            videohubTexture.draw(vertLine);
        }


        videohubTexture.draw(inputText);

        videohubTexture.display();
    }

public:
    VideoHubRender(std::shared_ptr<VideoHub> videohub, sf::Font font, sf::Vector2<unsigned int> textureSize) {
        this->videohub = videohub;
        this->font = font;

        this->videohubTextureSize = textureSize;
        if(!this->videohubTexture.create(textureSize)) {
        }
    };

    void render(sf::RenderWindow *window){
        if(dirty) this->render();

        const sf::Texture& texture = videohubTexture.getTexture();
        sf::Sprite sprite(texture);
        window->draw(sprite);
    }
};


#endif //VIDEOHUB_VIDEOHUBRENDER_H
