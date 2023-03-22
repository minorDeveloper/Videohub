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
    int textSize = 14;
    int lineThickness = 2;

    sf::FloatRect inputTextSize, outputTextSize;


    int count_digit(int number) {
        return int(log10(number) + 1);
    }

    std::string generateDigitString(int inputIndex, int maxInputIndex) {
        int maxInputDigit = count_digit(maxInputIndex);
        int currentInputDigit = count_digit(inputIndex);
        std::string digitString = "";
        for (int i = 0; i < maxInputDigit - currentInputDigit; i++)
            digitString += "0";

        digitString += std::to_string(inputIndex);
        return digitString;
    }

    void render() {
        videohubTexture.clear(sf::Color::White);

        inputText.setCharacterSize(textSize);
        inputText.setFont(font);
        inputText.setFillColor(sf::Color::Black);

        outputText.setCharacterSize(textSize);
        outputText.setFont(font);
        outputText.setFillColor(sf::Color::Black);

        // Determine longest input word
        inputText.setString(videohub->longestInputLabel() + " - " + generateDigitString(1, videohub->inputs()));
        inputTextSize = inputText.getLocalBounds();

        // Determine longest output word
        outputText.setString(videohub->longestOutputLabel() + " - " + generateDigitString(1, videohub->outputs()));
        outputTextSize = outputText.getLocalBounds();

        float inputZeroOffset  = 3 * margin + outputTextSize.width;
        float outputZeroOffset = 3 * margin + inputTextSize.width;

        float inputWordGap  = (videohubTextureSize.y - (2 * margin + inputZeroOffset +  (videohub->inputs() - 1)  * inputTextSize.height )) / (videohub->inputs()  - 1);
        float outputWordGap = (videohubTextureSize.x - (2 * margin + outputZeroOffset + (videohub->outputs() - 1) * outputTextSize.height)) / (videohub->outputs() - 1);

        // Draw the inputs
        sf::Vector2f textPosition;
        for (int inputIndex = 0; inputIndex < videohub->inputs(); inputIndex++) {
            std::string str = videohub->getVideoInputLabel(inputIndex);
            inputText.setString(str + " - " + generateDigitString(inputIndex + 1, videohub->inputs()));
            textPosition.y = inputZeroOffset + inputIndex * (inputWordGap + inputTextSize.height);
            textPosition.x = margin + (inputTextSize.width - inputText.getLocalBounds().width);

            inputText.setPosition(textPosition);
            videohubTexture.draw(inputText);

        }


        // Draw the Outputs
        outputText.setRotation(sf::degrees(270));

        for (int outputIndex = 0; outputIndex < videohub->outputs(); outputIndex++) {
            std::string str = videohub->getVideoOutputLabel(outputIndex);
            outputText.setString(generateDigitString(outputIndex + 1, videohub->outputs()) + " - " + str);
            textPosition.y = margin + outputTextSize.width;
            textPosition.x = outputZeroOffset + outputIndex * (outputWordGap + outputTextSize.height);

            outputText.setPosition(textPosition);
            videohubTexture.draw(outputText);
        }

        // Draw the lines
        sf::RectangleShape horizLine, vertLine;


        for (int outputIndex = 0; outputIndex < videohub->outputs(); outputIndex++) { //
            int inputIndex = videohub->getVideoOutputRouting(outputIndex);

            vertLine.setPosition(sf::Vector2f(outputZeroOffset + outputIndex * (outputWordGap + outputTextSize.height) + 0.5 * (outputTextSize.height + lineThickness), inputZeroOffset - margin));
            vertLine.setSize(sf::Vector2f(lineThickness, inputIndex * (inputWordGap + inputTextSize.height) + 0.5 * inputTextSize.height + 1.5 * lineThickness + margin));

            horizLine.setPosition(sf::Vector2f(outputZeroOffset - margin, inputZeroOffset + inputIndex * (inputWordGap + inputTextSize.height) + 0.5 * (inputTextSize.height + lineThickness)));
            horizLine.setSize(sf::Vector2f(outputIndex * (outputWordGap + outputTextSize.height) + 0.5 * outputTextSize.height + 1.5 * lineThickness + margin, lineThickness));

            sf::Color lineColor;
            switch (videohub->getVideoOutputLock(outputIndex)) {
                case LockStatus::owned:
                    lineColor = sf::Color::Blue;
                    break;
                case LockStatus::unlocked:
                    lineColor = sf::Color::Black;
                    break;
                case LockStatus::locked:
                    lineColor = sf::Color::Red;
            }
            horizLine.setFillColor(lineColor);
            vertLine.setFillColor(lineColor);

            videohubTexture.draw(vertLine);
            videohubTexture.draw(horizLine);
        }

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
