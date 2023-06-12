#ifndef ENEMY_H
#define ENEMY_H

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
#include "Bullet.h"

using namespace std;

enum enemy_type {n, r, s, d, a, b};


class Enemy{
    public:
        Enemy(int y_value, int x_value, int frame_value, char type_value) \
        : y(y_value), x(x_value), create_frame_enemy(frame_value), type(type_value) {};
        int y, x;
        char type;
        int create_frame_enemy; //initialize when it creates
        int check_frame_enemy=0;
        enum enemy_type enemy_type;
        int hp;
        int cell_speed;
        int bullet_damage = 1;
        bool buffed = false;
        int buff_speed;
        bool in_buff_area(Enemy* buffed_target) {
            if ((buffed_target->x >= (this->x-3)) && (buffed_target->x <= (this->x+3))
                && (buffed_target->y >= (this->y-3)) && (buffed_target->y <= (this->y+3))) {
                return true;
                }
            else {
                return false;
            }
        };
};
#endif

class Enemy_1n: public Enemy
{
    public :
    Enemy_1n(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy(y_value, x_value, frame_value, type_value) 
    {enemy_type = n;
    hp = 10;};
    //vector<Enemy_bullet> enemy_bullet {};
};

class Enemy_2r: public Enemy_1n
{
    public :
    Enemy_2r(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_1n(y_value, x_value, frame_value, type_value) 
    {enemy_type = r;
    hp = 5;
    cell_speed = 3;};
};

class Enemy_3s: public Enemy_2r
{
    public :
    Enemy_3s(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_2r(y_value, x_value, frame_value, type_value) 
    {enemy_type = s;
    cell_speed = 9;
    hp = 4;};
};

class Enemy_4d: public Enemy_2r
{
    public :
    Enemy_4d(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_2r(y_value, x_value, frame_value, type_value) 
    {enemy_type = d;
    hp = 5;};
};

class Enemy_5a: public Enemy_1n
{
    public :
    Enemy_5a(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_1n(y_value, x_value, frame_value, type_value)
    {enemy_type = a;
    hp = 8;
    buff_speed = 6;};
};

class Enemy_6b: public Enemy_1n  // BOSS
{
    public :
    Enemy_6b(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_1n(y_value, x_value, frame_value, type_value)
    {enemy_type = b;
    hp = 300;
    cell_speed = 3;
    };
};

class Enemy_bullet
{
    public:
        Enemy_bullet(int y_value, int x_value, int frame_value, int frame_value_enemy, int damage_value, char enemy_b_type) \
        : y(y_value), x(x_value), create_frame_enemy_bullet(frame_value), frame_enemy(frame_value_enemy), damage(damage_value), enemy_b_type(enemy_b_type) {direction_e = 'm';};
        int damage;
        int y, x;
        int create_frame_enemy_bullet; //initialize when it creates
        int check_frame_enemy_bullet;
        int frame_enemy; // 식별자
        char enemy_b_type;
        char direction_e;
};

class Enemy_bullet_left : public Enemy_bullet
{
    public : 
    Enemy_bullet_left(int y_value, int x_value, int frame_value, int frame_value_enemy, int damage_value, char enemy_b_type) \
    : Enemy_bullet(y_value, x_value, frame_value, frame_value_enemy, damage_value, enemy_b_type) {direction_e = 'l';};
};

class Enemy_bullet_right : public Enemy_bullet
{
    public : 
    Enemy_bullet_right(int y_value, int x_value, int frame_value, int frame_value_enemy, int damage_value, char enemy_b_type) \
    : Enemy_bullet(y_value, x_value, frame_value, frame_value_enemy, damage_value, enemy_b_type) {direction_e = 'r';};
};