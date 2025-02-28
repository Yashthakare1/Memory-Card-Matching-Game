# 🎮 Memory Match SFML Game
A simple memory card matching game built using **C++ and SFML**.

## 📌 Features
- A 4x4 grid of cards with random values.
- Click to flip cards and find matching pairs.
- Keeps track of moves and time.
- Restart button to reset the game.

## 🔧 Installation & Setup (Windows)

### Windows
1. Install SFML: [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)
2. Install MinGW-w64 for `g++`
3. Compile:
   ```sh
   g++ -o Memory_Card_Matching_Game.exe Memory_Card_Matching_Game.cpp -I C:\SFML\include -L C:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system


## 🔧 Installation & Setup (Kali Linux)
### **Step 1: Install SFML**
Ensure you have **SFML** installed on your system:
```sh
sudo apt update
sudo apt install libsfml-dev g++



🎮 How to Play?

    Click on a card to flip it.
    Click on another card to find a match.
    If both cards match, they stay flipped; otherwise, they flip back.
    The game ends when all pairs are matched.
    Click Restart to play again.

