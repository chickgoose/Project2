#ifndef BULLET_CHANGE_H
#define BULLET_CHANGE_H

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

enum bullet_type {P, L};

class Bullet_change{
    public:
        Bullet_change(int y_value, int x_value, int frame_value, char type_value) \
        : y(y_value), x(x_value), create_frame_bullet_change(frame_value), type(type_value){};
        int y, x;
        char type;
        int create_frame_bullet_change; //initialize when it creates
        int check_frame_bullet_change=0;
        enum bullet_type bullet_type;
};
#endif

class Powerup_bullet : public Bullet_change
{
    public :
    Powerup_bullet(int y_value, int x_value, int frame_value, char type_value) \
    : Bullet_change(y_value, x_value, frame_value, type_value) 
    {bullet_type = P;};
};

class Levelup_bullet : public Bullet_change
{
    public :
    Levelup_bullet(int y_value, int x_value, int frame_value, char type_value) \
    : Bullet_change(y_value, x_value, frame_value, type_value) 
    {bullet_type = L;};
};