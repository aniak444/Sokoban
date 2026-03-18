#include "SFML/Graphics.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <SFML/Audio.hpp>

enum class Field { VOID, FLOOR, WALL, BOX, PARK, PLAYER };

class Sokoban : public sf::Drawable
{
public:
	void LoadMapFromFile(std::string fileName);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void SetDrawParameters(sf::Vector2u draw_area_size);
	void Move_Player_Left();
	void Move_Player_Right();
	void Move_Player_Up();
	void Move_Player_Down();
	bool Is_Victory() const;
	void LoadTextures();

private:
	std::vector<std::vector<Field>> map;
	sf::Vector2f shift, tile_size;
	sf::Vector2i player_position;
	std::vector<sf::Vector2i> park_positions;

	void move_player(int dx, int dy);

	Field under_player = Field::FLOOR;
	std::vector<sf::Vector2i> box_positions;

	sf::Texture void_texture;
	sf::Texture wall_texture;
	sf::Texture floor_texture;
	sf::Texture box_texture;
	sf::Texture park_texture;
	sf::Texture player_texture;

};

void Sokoban::LoadTextures()
{
	if (!void_texture.loadFromFile("void.png"))  std::cerr << "blad void" << std::endl;
	if (!wall_texture.loadFromFile("wall.png"))  std::cerr << "blad wall" << std::endl;
	if (!floor_texture.loadFromFile("floor.png"))  std::cerr << "blad floor" << std::endl;
	if (!box_texture.loadFromFile("box.png"))  std::cerr << "blad box" << std::endl;
	if (!park_texture.loadFromFile("park.png"))  std::cerr << "blad park" << std::endl;
	if (!player_texture.loadFromFile("player.png"))  std::cerr << "blad player" << std::endl;
}


void Sokoban::LoadMapFromFile(std::string fileName)
{
	std::string str;
	std::vector<std::string> vos;

	std::ifstream in(fileName.c_str());
	if (!in)
	{
	}
	while (std::getline(in, str)) { vos.push_back(str); }
	in.close();

	map.clear();
	map.resize(vos.size(), std::vector<Field>(vos[0].size()));
	for (auto [row, row_end, y] = std::tuple{ vos.cbegin(), vos.cend(), 0 }; row != row_end; ++row, ++y)
		for (auto [element, end, x] = std::tuple{ row->begin(), row->end(), 0 }; element != end; ++element, ++x)
			switch (*element)
			{
			case 'X': map[y][x] = Field::WALL; break;
			case '*': map[y][x] = Field::VOID; break;
			case ' ': map[y][x] = Field::FLOOR; break;
			case 'B': map[y][x] = Field::FLOOR; box_positions.push_back(sf::Vector2i(x, y)); break;
			case 'P': map[y][x] = Field::PARK; park_positions.push_back(sf::Vector2i(x, y));  break;
			case 'S': map[y][x] = Field::FLOOR; player_position = sf::Vector2i(x, y);  break;
			}
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{  
	sf::Sprite sprite(void_texture);
    //sf::RectangleShape tile(tile_size);  
    for (int y = 0; y < map.size(); ++y)  
        for (int x = 0; x < map[y].size(); ++x)  
        {  
        
			
		
			switch (map[y][x])
			{
			case Field::WALL:   sprite.setTexture(wall_texture, true); break;
			case Field::FLOOR:  sprite.setTexture(floor_texture, true); break;
			case Field::BOX:    break; //sprite.setTexture(box_texture, true); break;
			case Field::PARK:   sprite.setTexture(park_texture, true); break;
			case Field::PLAYER: break; // sprite.setTexture(player_texture, true); break;
			case Field::VOID:   sprite.setTexture(void_texture, true); break;;
			}
			sprite.setPosition(sf::Vector2f(shift.x + x * tile_size.x, shift.y + y * tile_size.y));
			sprite.setScale({tile_size.x / sprite.getTexture().getSize().x, tile_size.y / sprite.getTexture().getSize().y});


			target.draw(sprite, states);

		/* tile.setPosition(sf::Vector2f(shift.x + x * tile_size.x, shift.y + y * tile_size.y));  
            
			switch (map[y][x])  
            {  
            case Field::WALL: tile.setFillColor(sf::Color::Black);  
                break;  
            case Field::VOID: tile.setFillColor(sf::Color::Blue);  
                break;  
            case Field::FLOOR: tile.setFillColor(sf::Color::Green);  
                break;  
            case Field::BOX: tile.setFillColor(sf::Color::Yellow);  
                break;  
            case Field::PARK: tile.setFillColor(sf::Color::Red);  
                break;  
            case Field::PLAYER: tile.setFillColor(sf::Color::Magenta);  
                break;  
            }  
            target.draw(tile, states);  */
        } 

	for (auto& box : box_positions)
	{
		sf::Sprite boxSprite(box_texture);
		boxSprite.setScale({
			tile_size.x / box_texture.getSize().x,
			tile_size.y / box_texture.getSize().y
			});
		boxSprite.setPosition(sf::Vector2f(
			shift.x + box.x * tile_size.x,
			shift.y + box.y * tile_size.y
		));
		target.draw(boxSprite, states);
	}


	sf::Sprite playerSprite(player_texture);
	playerSprite.setScale({
		tile_size.x / player_texture.getSize().x,
		tile_size.y / player_texture.getSize().y
		});
	playerSprite.setPosition(sf::Vector2f(
		shift.x + player_position.x * tile_size.x,
		shift.y + player_position.y * tile_size.y
	));
	target.draw(playerSprite, states);

}

void Sokoban::SetDrawParameters(sf::Vector2u draw_area_size)
{
	this->tile_size = sf::Vector2f(
		std::min(std::floor((float)draw_area_size.x / (float)map[0].size()), std::floor((float)draw_area_size.y / (float)map.size())),
		std::min(std::floor((float)draw_area_size.x / (float)map[0].size()), std::floor((float)draw_area_size.y / (float)map.size()))
	);
	this->shift = sf::Vector2f(
		((float)draw_area_size.x - this->tile_size.x * map[0].size()) / 2.0f,
		((float)draw_area_size.y - this->tile_size.y * map.size()) / 2.0f
	);
}

void Sokoban::Move_Player_Left()
{
	move_player(-1, 0);
}

void Sokoban::Move_Player_Right()
{
	move_player(1, 0);
}

void Sokoban::Move_Player_Up()
{
	move_player(0, -1);
}

void Sokoban::Move_Player_Down()
{
	move_player(0, 1);
}

void Sokoban::move_player(int dx, int dy)
{
	bool allow_move = false; 
	sf::Vector2i new_pp(player_position.x + dx, player_position.y + dy); 
	Field fts = map[new_pp.y][new_pp.x];
	Field ftsa = map[new_pp.y + dy][new_pp.x + dx]; 

	if (fts == Field::FLOOR || fts == Field::PARK) allow_move = true;
	

	for (auto& box : box_positions)
	{
		if (box == new_pp)
		{
			sf::Vector2i new_box_pos = box + sf::Vector2i(dx, dy);
			Field behind_box = map[new_box_pos.y][new_box_pos.x];

			bool box_blocking = false;
			for (auto& other_box : box_positions)
				if (other_box == new_box_pos) box_blocking = true;

			if ((behind_box == Field::FLOOR || behind_box == Field::PARK) && !box_blocking)
			{
				box = new_box_pos;
				allow_move = true;
			}
			else
			{
				return;
			}
		}
	}


	if (allow_move)
	{
		Field next_under = map[new_pp.y][new_pp.x];

		
		player_position = new_pp;
		under_player = next_under;

	}

	for (auto park_position : park_positions) if (map[park_position.y][park_position.x] == Field::FLOOR) map[park_position.y][park_position.x] = Field::PARK;
}

bool Sokoban::Is_Victory() const
{

	for (auto park_position : park_positions)
	{
		bool box_found = false;
		for (auto& box : box_positions)
			if (box == park_position)
				box_found = true;

		if (!box_found)
			return false;
	}
	return true;
}




int main()
{
	sf::Clock timer;

	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "IUiBG Lab 01", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

	sf::Music sound("spongebob_theme_song.ogg");
	sound.setVolume(30);
	sound.setLooping(true);
	sound.play();

	sf::Font font("spongefont.ttf");
	sf::Text timerText(font);
	timerText.setFont(font);
	timerText.setCharacterSize(20);
	timerText.setFillColor(sf::Color::Red);
    timerText.setPosition({10,10});

	sf::Text victoryText(font);
	victoryText.setFont(font);
	victoryText.setCharacterSize(30);
	victoryText.setFillColor(sf::Color::Red);
	victoryText.setPosition({15, window.getSize().y / 2.f });
	victoryText.setString("Congratulations!! You won! You delivered all the krabby patty!");

	sf::RectangleShape victoryBar;
	victoryBar.setSize(sf::Vector2f(window.getSize().x, 60));
	victoryBar.setFillColor(sf::Color(0, 0, 0, 200));
	victoryBar.setPosition({ 0, window.getSize().y / 2.f - 10 });


	Sokoban sokoban;
	sokoban.LoadTextures();
	std::optional<sf::Event> event;

	sokoban.LoadMapFromFile("plansza.txt");
	sokoban.SetDrawParameters(window.getSize());

	while (window.isOpen())
	{
		while (auto event = window.pollEvent())
		{
			if (event->getIf<sf::Event::Closed>())
			{
				sound.stop();
				window.close();
			}
			if (const auto resized = event->getIf<sf::Event::Resized>())
			{
				window.setView(sf::View(sf::FloatRect({ 0, 0 }, static_cast<sf::Vector2f>(resized->size))));
				sokoban.SetDrawParameters(window.getSize());
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			{
				sokoban.Move_Player_Left();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			{
				sokoban.Move_Player_Right();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				sokoban.Move_Player_Up();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				sokoban.Move_Player_Down();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				sound.stop();
				window.close();
			}
			if (sokoban.Is_Victory())
			{
				window.clear();
				window.draw(sokoban);
				window.draw(timerText);
				window.draw(victoryBar);
				window.draw(victoryText);
				window.display();
				sf::sleep(sf::seconds(8));
				sound.stop();
				window.close();
			}

			

			//sokoban.Is_Victory();
		}
		sf::Time elapsed = timer.getElapsedTime();
		int seconds = static_cast<int>(elapsed.asSeconds());
		timerText.setString("Time: " + std::to_string(seconds) + "s");
		
		window.clear();


		window.draw(sokoban);
		window.draw(timerText);
		window.display();
	}

	return 0;
}