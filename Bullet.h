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
#include "Enemy.h"

class Bullet{
    public:
        Bullet(int y_value, int x_value, int frame_value) \
        : y(y_value), x(x_value), create_frame_bullet(frame_value) {};
        int damage=1;
        int y, x;
        int create_frame_bullet; //initialize when it creates
        int check_frame_bullet;
        int level=1;
        int enemy_idx;
        bool crash;
        Enemy* target;
        void attack(vector<Enemy*> vec_attacked_enemy) {
            for (int i=0; i<vec_attacked_enemy.size(); i++) {
                if (y == vec_attacked_enemy[i]->y && x == vec_attacked_enemy[i]->x) {
                    target = vec_attacked_enemy[i];
                    crash = true;
                    enemy_idx = i;
                }
            }
        }
};
#endif
