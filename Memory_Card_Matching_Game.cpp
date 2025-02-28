#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <sstream>

// Constants for grid size, card size, and total number of cards
const int GRID_SIZE = 4;
const int CARD_SIZE = 100;
const int WINDOW_SIZE = GRID_SIZE * CARD_SIZE;
const int TOTAL_CARDS = GRID_SIZE * GRID_SIZE;

// Structure to represent a card
struct Card {
    sf::RectangleShape shape; // Shape for the card's appearance
    sf::Texture texture;      // Texture for the card's image
    sf::Sprite sprite;        // Sprite to display the texture
    int value;                // Card value (to match pairs)
    bool isFlipped;           // Whether the card is currently flipped
    bool isMatched;           // Whether the card is matched
};


// Function to shuffle the card values
void shuffleCards(std::vector<int>& cardValues) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cardValues.begin(), cardValues.end(), g);
}


// Function to initialize the game state
void initializeGame(std::vector<Card>& cards, std::vector<int>& cardValues, std::vector<sf::Texture>& textures, int& moves, sf::Clock& gameClock, bool& gameWon, float& finalTime) {
    shuffleCards(cardValues);
    for (int i = 0; i < TOTAL_CARDS; ++i) {
        cards[i].shape.setSize(sf::Vector2f(CARD_SIZE - 5, CARD_SIZE - 5));
        cards[i].shape.setPosition((i % GRID_SIZE) * CARD_SIZE, (i / GRID_SIZE) * CARD_SIZE);
        cards[i].shape.setFillColor(sf::Color::Blue);
        
        cards[i].value = cardValues[i];
        cards[i].isFlipped = false;
        cards[i].isMatched = false;
        
        // Set the card's texture and scale it properly
        cards[i].sprite.setTexture(textures[cardValues[i]]);
        cards[i].sprite.setPosition(cards[i].shape.getPosition());
        cards[i].sprite.setScale(
            (CARD_SIZE - 5) / cards[i].sprite.getLocalBounds().width, 
            (CARD_SIZE - 5) / cards[i].sprite.getLocalBounds().height
        );
    }
    moves = 0;
    gameClock.restart();
    gameWon = false;
    finalTime = -1.0f;  // Reset final time display
}

// Function to check if the game is won (all cards are matched)
bool isGameWon(const std::vector<Card>& cards) {
    for (const auto& card : cards) {
        if (!card.isMatched) {
            return false;
        }
    }
    return true;
}

int main() {
    // Create the game window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE + 120), "Memory Card Matching Game");
    
    // Generate card values (pairs of values for matching)
    std::vector<int> cardValues(TOTAL_CARDS / 2);
    for (int i = 0; i < TOTAL_CARDS / 2; ++i) {
        cardValues[i] = i;
    }
    cardValues.insert(cardValues.end(), cardValues.begin(), cardValues.end());
    
    // Load textures for cards
    std::vector<sf::Texture> textures(TOTAL_CARDS / 2);
    for (int i = 0; i < TOTAL_CARDS / 2; ++i) {
        if (!textures[i].loadFromFile("images/card" + std::to_string(i) + ".png")) {
            std::cerr << "Error loading images/card" << i << ".png" << std::endl;
            return 1;
        }
        textures[i].setSmooth(true);
    }
    
    // Create card objects
    std::vector<Card> cards(TOTAL_CARDS);
    int moves = 0;
    sf::Clock gameClock;
    bool gameWon = false;
    float finalTime = -1.0f;
    
    initializeGame(cards, cardValues, textures, moves, gameClock, gameWon, finalTime);
    
    // Create restart button
    sf::RectangleShape restartButton(sf::Vector2f(120, 40));
    restartButton.setPosition(WINDOW_SIZE / 2 - 60, WINDOW_SIZE + 10);
    restartButton.setFillColor(sf::Color::Green);
    
    // Load font for UI text
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return 1;
    }
    
    // Initialize UI text elements
    sf::Text restartText("Restart", font, 20);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(WINDOW_SIZE / 2 - 35, WINDOW_SIZE + 20);
    
    sf::Text moveText("Moves: 0", font, 20);
    moveText.setPosition(10, WINDOW_SIZE + 10);
    
    sf::Text timerText("Time: 0s", font, 20);
    timerText.setPosition(WINDOW_SIZE - 120, WINDOW_SIZE + 10);
    
    sf::Text finalTimeText("", font, 20);
    finalTimeText.setFillColor(sf::Color::Yellow);
    finalTimeText.setPosition(WINDOW_SIZE / 2 - 60, WINDOW_SIZE + 60);
    
    int firstFlipped = -1, secondFlipped = -1;
    bool waiting = false;
    sf::Clock clock;
    
    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                
                // Handle restart button click
                if (y >= WINDOW_SIZE + 10 && y <= WINDOW_SIZE + 50 && x >= WINDOW_SIZE / 2 - 60 && x <= WINDOW_SIZE / 2 + 60) {
                    initializeGame(cards, cardValues, textures, moves, gameClock, gameWon, finalTime);
                    firstFlipped = -1;
                    secondFlipped = -1;
                    waiting = false;
                }
                // Handle card flipping logic
                if (!waiting && y < WINDOW_SIZE && !gameWon) {
                    int index = (y / CARD_SIZE) * GRID_SIZE + (x / CARD_SIZE);
                    if (!cards[index].isFlipped && !cards[index].isMatched) {
                        cards[index].isFlipped = true;
                        moves++;
                        if (firstFlipped == -1) {
                            firstFlipped = index;
                        } else {
                            secondFlipped = index;
                            waiting = true;
                            clock.restart();
                        }
                    }
                }
            }
        }
        
        // Check card matching logic
        if (waiting && clock.getElapsedTime().asSeconds() > 1) {
            if (cards[firstFlipped].value == cards[secondFlipped].value) {
                cards[firstFlipped].isMatched = true;
                cards[secondFlipped].isMatched = true;
            } else {
                cards[firstFlipped].isFlipped = false;
                cards[secondFlipped].isFlipped = false;
            }
            firstFlipped = -1;
            secondFlipped = -1;
            waiting = false;
        }
        
        // Check if the game is won
        if (!gameWon && isGameWon(cards)) {
            gameWon = true;
            finalTime = gameClock.getElapsedTime().asSeconds();
        }
        
        std::ostringstream moveStream, timerStream, finalTimeStream;
        moveStream << "Moves: " << moves;
        timerStream << "Time: " << static_cast<int>(gameClock.getElapsedTime().asSeconds()) << "s";
        
        if (finalTime > 0) {
            finalTimeStream << "Final Time: " << static_cast<int>(finalTime) << "s";
            finalTimeText.setString(finalTimeStream.str());
        } else {
            finalTimeText.setString("");
        }
        
        moveText.setString(moveStream.str());
        timerText.setString(timerStream.str());
        
        window.clear();
        for (const auto& card : cards) {
            if (card.isFlipped || card.isMatched) {
                window.draw(card.sprite);
            } else {
                window.draw(card.shape);
            }
        }
        window.draw(restartButton);
        window.draw(restartText);
        window.draw(moveText);
        window.draw(timerText);
        window.draw(finalTimeText);
        window.display();
    }
    return 0;
}

