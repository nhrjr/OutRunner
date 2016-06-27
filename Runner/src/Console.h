#pragma once
#include <SFML/Graphics/Drawable.hpp>

#include "Game.h"

#include <unordered_map>
#include <functional>
#include <queue>
#include <list>


template < class ContainerT >
void tokenize(const std::string& str, ContainerT& tokens,
	const std::string& delimiters = " ", bool trimEmpty = false)
{
	std::string::size_type pos, lastPos = 0;

	using value_type = typename ContainerT::value_type;
	using size_type = typename ContainerT::size_type;

	while (true)
	{
		pos = str.find_first_of(delimiters, lastPos);
		if (pos == std::string::npos)
		{
			pos = str.length();

			if (pos != lastPos || !trimEmpty)
				tokens.push_back(value_type(str.data() + lastPos,
				(size_type)pos - lastPos));

			break;
		}
		else
		{
			if (pos != lastPos || !trimEmpty)
				tokens.push_back(value_type(str.data() + lastPos,
				(size_type)pos - lastPos));
		}

		lastPos = pos + 1;
	}
}

class ConsoleStream : public std::ostream
{
	class ConsoleStreamBuf : public std::stringbuf
	{
		std::ostringstream& output;
		bool& read;
	public:
		ConsoleStreamBuf(std::ostringstream& str, bool& r) : output(str), read(r) { };
	
		virtual int sync()
		{
			//output << std::ctime_s(&ttp) << str();
			//str("");
			output << "<log: " << str();
			str("");
			output.flush();
			read = true;
			return 0;
		}
	};

	ConsoleStreamBuf buffer;
public:
	ConsoleStream() : std::ostream(&buffer), buffer(str,read) {};
	bool read;
	std::ostringstream str;
	std::string getString() { 
		read = false;
		std::string returnstring = str.str();
		str.str("");
		str.clear();
		return returnstring;
	};
};

typedef std::function<std::string(std::vector<std::string>&)> Command;

class Console :
	public sf::Drawable
{
	
private:
	std::map<std::string, Command> commands;
	std::string inputBuffer;
	ConsoleStream outputBufferStream;
	std::list<std::string> outputBuffer;
	bool show;

	sf::Text inputText;
	sf::Text outputText;
	sf::RectangleShape background;
	sf::Font font;

	Game* game;
public:
	static Console& Instance()
	{
		static Console instance;
		return instance;
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const ;

	template <typename T>
	Console &operator<<(const T &a) {
		outputBufferStream << a;
		if (outputBufferStream.read == true)
		{
			this->Print(outputBufferStream.getString());
		}
		return *this;
	}

	Console &operator<<(std::ostream& (*pf) (std::ostream&)) {
		outputBufferStream << pf;
		if (outputBufferStream.read == true)
		{
			this->Print(outputBufferStream.getString());
		}
		return *this;
	}



	Console(Console const&) = delete;
	void operator=(Console const&) = delete;
private:
	Console();


	void UpdateInputText();
	void UpdateOutputText();
	void ParseCommand();
	void Print(const std::string& str);

public:
	void Add(const std::string& name, Command cmd);
	void Remove(const std::string& name);
	void HandleEvent(const sf::Event& event);
	
	
	void ToggleShow()
	{
		this->show = (show == true) ? false : true;
		this->UpdateOutputText();
	}

	bool getShow() { return show; };
	void setShow(bool s) { show = s; };

};

