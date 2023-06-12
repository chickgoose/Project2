#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <variant>
#include <vector>
#include <map>
#include "My_plane.h"
#include "Enemy.h"
#include "Bullet_change.h"

using namespace std;

//Screen manager.
class Screen_manager{
    public:
        int frame_length=100000; //each frame's length == 0.1sec(10,000 microseconds)
        int curr_frame=0; //1frame=0.1sec

        int y, x;
        int width=60;
        int height=30;
        int frame_event[300]={0,}, y_event[300]={0,}, x_event[300]={0,};
        char type_event[300] = {' ',};

        void print(int ascii);
        void print();
        void print_share();
        void render();

        char board[30][60]={'a'};
        vector<Enemy*> vec_enemy_board[30][60] = {};
        vector<Enemy_bullet> vec_enemy_bullet_board[30][60] = {};
        vector<Bullet> vec_bullet_board[30][60] = {};
        My_plane my_plane = My_plane(height-2, 1, 0);
        vector<Enemy*> vec_enemy {};
        vector<Bullet_change*> vec_bullet_change {};

        int erase_idx_vec_bullet_board;
        int erase_idx_vec_enemy_bullet_board;
        bool cor_vec_bullet_board;
        bool cor_vec_enemy_bullet_board;
        int erase_idx_vec_enemy_board;
        bool cor_vec_enemy_board;
        int erase_idx_vec_enemy;
        int check;
        int end = 0;
        map<char, int> score_map;
        int score = 0;
        int initial_map = 0;
        vector<Enemy_bullet> vec_enemy_bullet {};
        int kill = 0;
        bool charge_bullet_call = false;
};
#endif