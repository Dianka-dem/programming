#include "paint_core.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm> // Для std::min, std::max, std::clamp
#include <cmath>

std::stack<Canvas> undoStack;

enum class Tool { BRUSH, RECTANGLE, LINE, ERASER, PICKER };

int main() {
    const int UI_HEIGHT = 120;
    sf::RenderWindow window(
        sf::VideoMode(WIDTH * PIXEL_SIZE, HEIGHT * PIXEL_SIZE + UI_HEIGHT),
        "Paint++"
    );
    auto desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        (desktop.width - window.getSize().x) / 2,
        (desktop.height - window.getSize().y) / 2
    ));

    Canvas canvas(HEIGHT, std::vector<sf::Color>(WIDTH, sf::Color::White));
    clearCanvas(canvas);

    Tool currentTool = Tool::BRUSH; // BRUSH по умолчанию, чтобы сразу рисовать при запуске
    sf::Color currentColor = sf::Color::Black;
    int brushRadius = 3;

    // Палитра с четко прописанным альфа-каналом (255) во избежание багов пипетки
    std::vector<sf::Color> palette = {
        sf::Color::Black, sf::Color::White, sf::Color::Red,
        sf::Color::Green, sf::Color::Blue, sf::Color::Yellow,
        sf::Color::Cyan, sf::Color::Magenta, 
        sf::Color(255, 128, 0, 255), 
        sf::Color(128, 0, 128, 255)
    };
    int selectedPaletteIndex = 0;

    bool isDrawing = false;
    int startX = 0, startY = 0, lastX = 0, lastY = 0;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {}
    sf::Text status;
    status.setFont(font);
    status.setCharacterSize(14);
    status.setFillColor(sf::Color::Black);
    status.setPosition(10, HEIGHT * PIXEL_SIZE + 10);

    std::cout << "=== PAINT with STL ===\n"
              << "Tools:\n  B - Brush   R - Rectangle   L - Line   E - Eraser   P - Picker (Pipette)\n"
              << "  +/- - Brush size\n  C - Clear canvas\n  U - Undo\n"
              << "Filters:\n  G - Grayscale   N - Negative   M - Blur\n"
              << "Colors: 0..9 or click on palette or Right Click on Canvas\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // ==================== КЛАВИАТУРА ====================
            if (event.type == sf::Event::KeyPressed) {
                
                // Стабильная обработка Ctrl+Z по физической клавише (работает на любой раскладке)
                if (event.key.control && event.key.code == sf::Keyboard::Z) {
                    undo(canvas);
                    std::cout << "Undo via Ctrl+Z. Stack size: " << undoStack.size() << std::endl;
                }
                else {
                    switch (event.key.code) {
                        // Выбор инструментов по коду клавиш (Англ раскладка)
                        case sf::Keyboard::B: 
                            currentTool = Tool::BRUSH; 
                            std::cout << "Tool: BRUSH" << std::endl;
                            break;
                        case sf::Keyboard::R: 
                            currentTool = Tool::RECTANGLE; 
                            std::cout << "Tool: RECTANGLE" << std::endl;
                            break;
                        case sf::Keyboard::L: 
                            currentTool = Tool::LINE; 
                            std::cout << "Tool: LINE" << std::endl;
                            break;
                        case sf::Keyboard::E: 
                            currentTool = Tool::ERASER; 
                            std::cout << "Tool: ERASER" << std::endl;
                            break;
                        case sf::Keyboard::P: 
                        case sf::Keyboard::LBracket: // Физическая клавиша P / З
                            currentTool = Tool::PICKER; 
                            std::cout << "Tool: PICKER (pipette)" << std::endl;
                            break;

                        // Действия с холстом
                        case sf::Keyboard::C: 
                            saveToUndo(canvas); 
                            clearCanvas(canvas); 
                            std::cout << "Canvas cleared" << std::endl;
                            break;
                        case sf::Keyboard::U: 
                            undo(canvas); 
                            std::cout << "Undo via U. Stack size: " << undoStack.size() << std::endl;
                            break;

                        // Фильтры
                        case sf::Keyboard::G: 
                            applyGrayscale(canvas); 
                            std::cout << "Grayscale applied" << std::endl;
                            break;
                        case sf::Keyboard::N: 
                            applyNegative(canvas); 
                            std::cout << "Negative applied" << std::endl;
                            break;
                        case sf::Keyboard::M: 
                            applyBlur(canvas); 
                            std::cout << "Blur applied" << std::endl;
                            break;

                        // Изменение размера кисти (Исправлен дубликат Dash/Hyphen)
                        case sf::Keyboard::Add:
                        case sf::Keyboard::Equal:
                            if (brushRadius < 20) brushRadius++;
                            std::cout << "Radius: " << brushRadius << std::endl;
                            break;
                        case sf::Keyboard::Hyphen:
                        case sf::Keyboard::Subtract:
                            if (brushRadius > 1) brushRadius--;
                            std::cout << "Radius: " << brushRadius << std::endl;
                            break;

                        // Палитра цифр
                        case sf::Keyboard::Num0:
                            selectedPaletteIndex = 0; currentColor = palette[0];
                            break;
                        case sf::Keyboard::Num1:
                            selectedPaletteIndex = 1; currentColor = palette[1];
                            break;
                        case sf::Keyboard::Num2:
                            selectedPaletteIndex = 2; currentColor = palette[2];
                            break;
                        case sf::Keyboard::Num3:
                            selectedPaletteIndex = 3; currentColor = palette[3];
                            break;
                        case sf::Keyboard::Num4:
                            selectedPaletteIndex = 4; currentColor = palette[4];
                            break;
                        case sf::Keyboard::Num5:
                            selectedPaletteIndex = 5; currentColor = palette[5];
                            break;
                        case sf::Keyboard::Num6:
                            selectedPaletteIndex = 6; currentColor = palette[6];
                            break;
                        case sf::Keyboard::Num7:
                        case sf::Keyboard::Quote: // Клавиша 7 / Э
                            selectedPaletteIndex = 7; currentColor = palette[7];
                            break;
                        case sf::Keyboard::Num8:
                            selectedPaletteIndex = 8; currentColor = palette[8];
                            break;
                        case sf::Keyboard::Num9:
                            selectedPaletteIndex = 9; currentColor = palette[9];
                            break;
                        default:
                            // Резервный физический скан-код для русской раскладки (если switch выше пропустил)
                            if (event.key.scancode == sf::Keyboard::Scan::B) currentTool = Tool::BRUSH;
                            else if (event.key.scancode == sf::Keyboard::Scan::R) currentTool = Tool::RECTANGLE;
                            else if (event.key.scancode == sf::Keyboard::Scan::L) currentTool = Tool::LINE;
                            else if (event.key.scancode == sf::Keyboard::Scan::E) currentTool = Tool::ERASER;
                            else if (event.key.scancode == sf::Keyboard::Scan::P) currentTool = Tool::PICKER;
                            else if (event.key.scancode == sf::Keyboard::Scan::C) { saveToUndo(canvas); clearCanvas(canvas); }
                            else if (event.key.scancode == sf::Keyboard::Scan::U) undo(canvas);
                            else if (event.key.scancode == sf::Keyboard::Scan::G) applyGrayscale(canvas);
                            else if (event.key.scancode == sf::Keyboard::Scan::N) applyNegative(canvas);
                            else if (event.key.scancode == sf::Keyboard::Scan::M) applyBlur(canvas);
                            break;
                    }
                }
            }

            // ==================== МЫШЬ ====================
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse = sf::Mouse::getPosition(window);
                int paletteY = HEIGHT * PIXEL_SIZE + 50;

                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (mouse.y >= paletteY && mouse.y <= paletteY + 30) {
                        for (size_t i = 0; i < palette.size(); ++i) {
                            int boxX = 10 + i * 35;
                            if (mouse.x >= boxX && mouse.x <= boxX + 30) {
                                selectedPaletteIndex = i;
                                currentColor = palette[i];
                                std::cout << "Color from palette: " << i << std::endl;
                                break;
                            }
                        }
                    } 
                    else if (mouse.y < HEIGHT * PIXEL_SIZE) {
                        if (currentTool == Tool::PICKER) {
                            int pX = mouse.x / PIXEL_SIZE;
                            int pY = mouse.y / PIXEL_SIZE;
                            if (pX >= 0 && pX < WIDTH && pY >= 0 && pY < HEIGHT) {
                                currentColor = canvas[pY][pX];
                                selectedPaletteIndex = -1;
                                for (size_t i = 0; i < palette.size(); ++i) {
                                    if (palette[i].r == currentColor.r && 
                                        palette[i].g == currentColor.g && 
                                        palette[i].b == currentColor.b) {
                                        selectedPaletteIndex = i;
                                        break;
                                    }
                                }
                            }
                        } 
                        else {
                            isDrawing = true;
                            startX = mouse.x / PIXEL_SIZE;
                            startY = mouse.y / PIXEL_SIZE;
                            lastX = startX; lastY = startY;

                            if (currentTool == Tool::BRUSH || currentTool == Tool::ERASER) {
                                saveToUndo(canvas);
                                sf::Color drawColor = (currentTool == Tool::ERASER) ? sf::Color::White : currentColor;
                                drawBrush(canvas, startX, startY, drawColor, brushRadius);
                            }
                        }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (mouse.y < HEIGHT * PIXEL_SIZE) {
                        int pX = mouse.x / PIXEL_SIZE;
                        int pY = mouse.y / PIXEL_SIZE;
                        if (pX >= 0 && pX < WIDTH && pY >= 0 && pY < HEIGHT) {
                            currentColor = canvas[pY][pX];
                            selectedPaletteIndex = -1;
                            for (size_t i = 0; i < palette.size(); ++i) {
                                if (palette[i].r == currentColor.r && 
                                    palette[i].g == currentColor.g && 
                                    palette[i].b == currentColor.b) {
                                    selectedPaletteIndex = i;
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (isDrawing) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);
                    int endX = std::clamp(mouse.x / PIXEL_SIZE, 0, WIDTH - 1);
                    int endY = std::clamp(mouse.y / PIXEL_SIZE, 0, HEIGHT - 1);
                    sf::Color drawColor = (currentTool == Tool::ERASER) ? sf::Color::White : currentColor;
                    
                    if (currentTool == Tool::RECTANGLE)
                        drawRectangle(canvas, startX, startY, endX, endY, drawColor);
                    else if (currentTool == Tool::LINE)
                        drawLine(canvas, startX, startY, endX, endY, drawColor);
                }
                isDrawing = false;
            }

            if (event.type == sf::Event::MouseMoved && isDrawing) {
                sf::Vector2i mouse = sf::Mouse::getPosition(window);
                int x = std::clamp(mouse.x / PIXEL_SIZE, 0, WIDTH - 1);
                int y = std::clamp(mouse.y / PIXEL_SIZE, 0, HEIGHT - 1);
                
                if ((currentTool == Tool::BRUSH || currentTool == Tool::ERASER) && (x != lastX || y != lastY)) {
                    sf::Color drawColor = (currentTool == Tool::ERASER) ? sf::Color::White : currentColor;

                    int dx = std::abs(x - lastX);
                    int dy = std::abs(y - lastY);
                    int sx = (lastX < x) ? 1 : -1;
                    int sy = (lastY < y) ? 1 : -1;
                    int err = dx - dy;
                    int curX = lastX;
                    int curY = lastY;

                    while (true) {
                        drawBrush(canvas, curX, curY, drawColor, brushRadius);
                        if (curX == x && curY == y) break;
                        int e2 = 2 * err;
                        if (e2 > -dy) { err -= dy; curX += sx; }
                        if (e2 < dx)  { err += dx; curY += sy; }
                    }

                    lastX = x; lastY = y;
                }
            }
        }

        window.clear(sf::Color(200, 200, 200));
        renderToWindow(window, canvas);

        for (size_t i = 0; i < palette.size(); ++i) {
            sf::RectangleShape box(sf::Vector2f(30, 30));
            box.setFillColor(palette[i]);
            box.setPosition(10 + i * 35, HEIGHT * PIXEL_SIZE + 50);
            box.setOutlineThickness(2);
            box.setOutlineColor((i == selectedPaletteIndex) ? sf::Color::Red : sf::Color::Black);
            window.draw(box);
        }

        std::string toolStr;
        switch (currentTool) {
            case Tool::BRUSH: toolStr = "BRUSH"; break;
            case Tool::RECTANGLE: toolStr = "RECT"; break;
            case Tool::LINE: toolStr = "LINE"; break;
            case Tool::ERASER: toolStr = "ERASER"; break;
            case Tool::PICKER: toolStr = "PICKER"; break;
        }
        status.setString(toolStr + "  |  radius:" + std::to_string(brushRadius) +
                        "  |  undo stack:" + std::to_string(undoStack.size()));
        window.draw(status);
        window.display();
    }
    return 0;
}
