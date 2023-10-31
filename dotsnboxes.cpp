#include <SFML/Graphics.hpp>
#include <iostream>

const int GRID_SIZE = 4;
const float DOT_RADIUS = 10.0f;
const float LINE_THICKNESS = 4.0f;

sf::RenderWindow window(sf::VideoMode(600, 600), "Dots and Boxes");

bool player1Turn = true;
int boxes[GRID_SIZE - 1][GRID_SIZE - 1] = {0};
bool lines[GRID_SIZE * 2][GRID_SIZE * 2] = {false};

void drawGrid() {
    // Draw dots
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            sf::CircleShape dot(DOT_RADIUS);
            dot.setFillColor(sf::Color::Black);
            dot.setPosition(100.0f + x * 100.0f, 100.0f + y * 100.0f);
            window.draw(dot);
        }
    }

    // Draw lines
    for (int x = 0; x < GRID_SIZE * 2; x++) {
        for (int y = 0; y < GRID_SIZE * 2; y++) {
            if (x % 2 != y % 2) {
                if (lines[x][y]) {
                    sf::RectangleShape line;
                    line.setSize(sf::Vector2f(LINE_THICKNESS, 100.0f));
                    if (x % 2 == 0) {
                        line.setPosition(100.0f + x / 2 * 100.0f, 100.0f + y / 2 * 100.0f);
                    } else {
                        line.setSize(sf::Vector2f(100.0f, LINE_THICKNESS));
                        line.setPosition(100.0f + x / 2 * 100.0f, 100.0f + y / 2 * 100.0f);
                    }
                    line.setFillColor(player1Turn ? sf::Color::Blue : sf::Color::Red);
                    window.draw(line);
                }
            }
        }
    }

    // Draw boxes
    for (int x = 0; x < GRID_SIZE - 1; x++) {
        for (int y = 0; y < GRID_SIZE - 1; y++) {
            if (boxes[x][y] == 1) {
                sf::RectangleShape box;
                box.setSize(sf::Vector2f(100.0f - LINE_THICKNESS, 100.0f - LINE_THICKNESS));
                box.setPosition(100.0f + x * 100.0f + LINE_THICKNESS / 2, 100.0f + y * 100.0f + LINE_THICKNESS / 2);
                box.setFillColor(sf::Color::Blue);
                window.draw(box);
            } else if (boxes[x][y] == 2) {
                sf::RectangleShape box;
                box.setSize(sf::Vector2f(100.0f - LINE_THICKNESS, 100.0f - LINE_THICKNESS));
                box.setPosition(100.0f + x * 100.0f + LINE_THICKNESS / 2, 100.0f + y * 100.0f + LINE_THICKNESS / 2);
                box.setFillColor(sf::Color::Red);
                window.draw(box);
            }
        }
    }
}

void checkForBoxCapture(int x, int y) {
    if (x % 2 == 0 && y % 2 == 0) {
        int capturedBoxes = 0;

        if (x > 0 && lines[x - 1][y]) {
            capturedBoxes++;
        }
        if (x < GRID_SIZE * 2 - 2 && lines[x + 1][y]) {
            capturedBoxes++;
        }
        if (y > 0 && lines[x][y - 1]) {
            capturedBoxes++;
        }
        if (y < GRID_SIZE * 2 - 2 && lines[x][y + 1]) {
            capturedBoxes++;
        }

        if (capturedBoxes > 0) {
            boxes[x / 2][y / 2] = player1Turn ? 1 : 2;
            player1Turn = !player1Turn;
        }
    }
}

int main() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int x = (mousePos.x - 100) / 100;
                int y = (mousePos.y - 100) / 100;

                if (x >= 0 && x < GRID_SIZE * 2 && y >= 0 && y < GRID_SIZE * 2 && !lines[x][y]) {
                    lines[x][y] = true;
                    checkForBoxCapture(x, y);
                }
            }
        }

        window.clear(sf::Color::White);
        drawGrid();
        window.display();
    }

    return 0;
}
