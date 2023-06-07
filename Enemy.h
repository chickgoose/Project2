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

enum enemy_type {n, r, s, d, a};

class Enemy{
    public:
        Enemy(int y_value, int x_value, int frame_value, char type_value) \
        : y(y_value), x(x_value), create_frame_enemy(frame_value), type(type_value) {};
        int y, x;
        char type;
        int shot_frame_enemy=1;
        int create_frame_enemy; //initialize when it creates
        int check_frame_enemy=0;
        vector<Bullet> bullet;
        enum enemy_type enemy_type;
};
#endif

class Enemy_1n: public Enemy
{
    public :
    Enemy_1n(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy(y_value, x_value, frame_value, type_value) 
    {enemy_type = n;};
};

class Enemy_2r: public Enemy_1n
{
    public :
    Enemy_2r(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_1n(y_value, x_value, frame_value, type_value) 
    {enemy_type = r;};
};

class Enemy_3s: public Enemy_2r
{
    public :
    Enemy_3s(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_2r(y_value, x_value, frame_value, type_value) 
    {enemy_type = s;};
};

class Enemy_4d: public Enemy_2r
{
    public :
    Enemy_4d(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_2r(y_value, x_value, frame_value, type_value) 
    {enemy_type = d;};
};

class Enemy_5a: public Enemy_1n
{
    public :
    Enemy_5a(int y_value, int x_value, int frame_value, char type_value) \
    : Enemy_1n(y_value, x_value, frame_value, type_value)
    {enemy_type = a;};
};