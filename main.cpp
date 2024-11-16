#include <SFML/Graphics.hpp>
#include <chrono>
#include <vector>
#include <stdlib.h>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
using namespace sf;
using namespace std;
using namespace chrono;
// 定义结构体
struct coordinate {
    int x, y;
    void operator=(coordinate c) {
        x = c.x;
        y = c.y;
    }
    bool operator==(coordinate c) {
        return x == c.x && y == c.y;
    }
};
// 全局变量
RenderWindow window(sf::VideoMode(1200, 810), "Greedy Snake", Style::Titlebar | Style::Close);
vector<coordinate> snake;
int xDirection[4] = { 1, 0, -1, 0 };
int yDirection[4] = { 0, 1, 0, -1 };
int direction = 0;
coordinate food = { rand() % 40, rand() % 27 };
int speed = 1;
// 初始化
void init() {
    snake.push_back({ 2, 0 });
    snake.push_back({ 1, 0 });
    snake.push_back({ 0, 0 });
}
// 事件处理
void runEvent() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::W && direction != 1)
                direction = 3;
            else if (event.key.code == Keyboard::S && direction != 3)
                direction = 1;
            else if (event.key.code == Keyboard::A && direction != 0)
                direction = 2;
            else if (event.key.code == Keyboard::D && direction != 2)
                direction = 0;
            if (event.key.code == Keyboard::E && speed != 1)
                speed --;
            if (event.key.code == Keyboard::Q)
                speed ++;
        }
    }
}
// 绘制
void drawGrid() {
    RectangleShape rect(Vector2f(26, 26));
    rect.setFillColor(Color(50, 50, 50));
    for (int x = 2; x < 1200; x += 30) {
        for (int y = 2; y < 800; y += 30) {
            rect.setPosition(x, y);
            window.draw(rect);
        }
    }
}
void drawSnake() {
    RectangleShape rect(Vector2f(26, 26));
    rect.setFillColor(Color(255, 0, 0));
    for (auto i : snake) {
        rect.setPosition(i.x * 30 + 2, i.y * 30 + 2);
        window.draw(rect);
    }
}
void drawFood() {
    RectangleShape rect(Vector2f(26, 26));
    rect.setFillColor(Color(0, 0, 255));
    rect.setPosition(food.x * 30 + 2, food.y * 30 + 2);
    window.draw(rect);
}
void draw() {
    window.clear();
    drawGrid();
    drawFood();
    drawSnake();
    window.display();
}
// 每一帧要执行的程序
void snakeRun(long long& m, long long& ml) {
    if (!(snake[0].x == food.x && snake[0].y == food.y)) {
        for (int i = snake.size() - 1; i > 0; i --)
            snake[i] = snake[i - 1];
        snake[0].x += xDirection[direction];
        snake[0].y += yDirection[direction];
        ml = m;
        return;
    }
    snake.push_back({ snake[0].x + xDirection[direction], snake[0].y + yDirection[direction] });
    food = { rand() % 40, rand() % 27 };
}
void gameOver() {
    if (snake[0].x < 0 || snake[0].x > 39 || snake[0].y < 0 || snake[0].y > 26) {
        window.close();
        return;
    }
    for (int i = 1; i < snake.size(); i ++)
        if (snake[0] == snake[i]) {
            window.close();
            return;
        }
}
// 主函数
int main() {
    window.setFramerateLimit(60);
    init();
    long long millisec_since_epoch_last = 0;
    while (window.isOpen()) {
        long long millisec_since_epoch = duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch()).count();
        runEvent();
        if (millisec_since_epoch / 100 / speed != millisec_since_epoch_last / 100 / speed)
            snakeRun(millisec_since_epoch, millisec_since_epoch_last);
            gameOver();
        draw();
    }
    return 0;
}