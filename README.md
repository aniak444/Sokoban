# Sokoban Logic Engine (C++/SFML) 📦

A 2D grid-based puzzle engine developed as a university project. It’s a clean implementation of the classic Sokoban mechanics, focusing on entity interaction and state management using the **SFML** framework.

## 🚀 What's inside?
* **Grid Logic:** Custom movement system with collision detection (Wall/Player/Box).
* **Map Parser:** Simple ASCII-to-Game-World translation. You edit `plansza.txt`, the engine does the rest.
* **Entity Management:** Clean handling of player and box positions via `sf::Vector2i`.
* **Sfx & UI:** Integration of `sf::Music` for background atmosphere and a real-time session timer.

## 🎮 How it works
* **Push Mechanics:** You can push one box at a time—no "super strength" to push two or more. 
* **Victory Check:** The engine constantly monitors "parking" spots. Once all boxes hit the targets, the game triggers a victory state.
* **Scaling:** Automatic tile scaling based on the window size (no more tiny maps on 4K screens).

## 🛠 Tech Stack
* **Language:** C++ (with some modern bits like `std::tuple` and `std::optional`).
* **Graphics/Audio:** SFML 2.6.x.
* **Environment:** Developed and tested in Visual Studio.

## 📁 Setup
1. Clone this repository.
2. Make sure you have **SFML** linked (standard procedure).
3. Keep all `.png`, `.ogg`, and `.ttf` files in the executable's directory.
4. Run it, move the boxes, don't get stuck.
