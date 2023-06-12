#ifndef BULLET_H
#define BULLET_H

#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <vector>

enum Direction {middle, left, right};

class Bullet{
    public:
        Bullet(int y_value, int x_value, int frame_value, int level_value) \
        : y(y_value), x(x_value), create_frame_bullet(frame_value), level(level_value) {direction = middle;};
        int y, x;
        int create_frame_bullet; //initialize when it creates
        int check_frame_bullet;
        int level;
        char Level_var[4] = {' ', '\'', '^', '!'};
        char bullet_shape = Level_var[level];
        enum Direction direction;
};

class Bullet_left : public Bullet
{
    public : 
        Bullet_left(int y_value, int x_value, int frame_value, int level_value) \
        : Bullet(y_value, x_value, frame_value, level_value) {direction = left;};
};


class Bullet_right : public Bullet
{
    public : 
        Bullet_right(int y_value, int x_value, int frame_value, int level_value) \
        : Bullet(y_value, x_value, frame_value, level_value) {direction = right;};
};

class Charged_bullet : public Bullet
{
    public : 
        Charged_bullet(int y_value, int x_value, int frame_value, int level_value) \
        : Bullet(y_value, x_value, frame_value, level_value) {bullet_shape = '0';};
};

#endif