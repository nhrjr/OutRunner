#include "stdafx.h"
#include "Console.h"
#include "GameSettingConstants.h"

#include <utility>



Console::Console()
{
	Command c = [this](std::vector<std::string> l) -> std::string {
		for (const auto& i : this->commands)
		{
			Print(i.first + "\n");
		}
		return std::to_string(this->commands.size()) + " commands found.";
	};
	this->Add("help", c);

	this->font.loadFromFile("media/font.ttf");

	this->background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT / 2));
	this->background.setFillColor(sf::Color(255, 255, 255, 128));
	
	this->outputText.setFont(font);
	this->outputText.setCharacterSize(10);
	this->outputText.setColor(sf::Color::Black);
	this->outputText.setPosition(sf::Vector2f(10, 10));

	this->inputText.setFont(font);
	this->inputText.setCharacterSize(14);
	this->inputText.setColor(sf::Color::Black);
	this->inputText.setPosition(sf::Vector2f(10, SCREEN_HEIGHT / 2 - 16));
}


void Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (show)
	{
		target.draw(background);
		target.draw(outputText);
		target.draw(inputText);
	}
}

void Console::UpdateInputText()
{
	this->inputText.setString("> " + inputBuffer);
}

void Console::UpdateOutputText()
{
	std::string output;
	for (const auto& s : outputBuffer)
	{
		output += s;
	}
	this->outputText.setString(output);
}

void Console::ParseCommand()
{
	if (!this->inputBuffer.empty())
	{
		std::vector<std::string> inputCommands;
		tokenize<std::vector<std::string>>(inputBuffer, inputCommands);
		const auto c = this->commands.find(inputCommands[0]);
		if (c != this->commands.end())
		{
			inputCommands.erase(inputCommands.begin());
			std::string result = c->second(inputCommands);
			if (!result.empty())
				Print(result + "\n");
		}
		else
		{
			Print(inputCommands[0] + ": command not founnd. \n");
		}
		this->inputBuffer.clear();
		this->UpdateInputText();
	}
}

void Console::Add(const std::string& name, Command cmd)
{
	//std::assert(this->commands.find(name) == this->commands.end());
	auto it = this->commands.insert(std::make_pair(name, cmd));
	if (it.second == false)
	{
		this->Print("Command: " + name + " already exists. \n");
	}
}

void Console::Remove(const std::string& name)
{
	const auto c = this->commands.find(name);
	if (c != this->commands.end())
	{
		this->commands.erase(c);
	}
}

void Console::HandleEvent(const sf::Event& event)
{
	if (this->show)
	{
		if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode > 31 && event.text.unicode < 127 && this->inputBuffer.size() < GAME_CONSOLE_INPUTBUFFER_LENGTH)
			{
				this->inputBuffer += static_cast<char>(event.text.unicode);
				this->UpdateInputText();
			}
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::BackSpace:
			{
				if (!this->inputBuffer.empty())
				{
					this->inputBuffer.pop_back();
				}
				this->UpdateInputText();
				break;
			}
			case sf::Keyboard::Return:
			{
				this->ParseCommand();
				break;
			}
			
			}
		}
	}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
		ToggleShow();
}

void Console::Print(const std::string& str)
{
	outputBuffer.emplace_back(str);
	if (outputBuffer.size() > GAME_CONSOLE_HISTORYLENGTH)
		outputBuffer.pop_front();

	if (this->show)
		UpdateOutputText();
}

