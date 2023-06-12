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
#include <cctype>
#include "Screen_manager.h"

using namespace std;

//move cursor
void cursorYX(int y, int x)
{
    COORD pos;
    pos.X = x;        
    pos.Y = y;            
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Screen_manager::render(){
    for(int j=0; j<height; j++){
        for(int i=0; i<width; i++){
            cursorYX(j, i);
            printf("%c", board[j][i]);
        }
    }
}

//print sharing things.
void Screen_manager::print_share(){
    //wall
    for(int i=0; i<width; i++){board[height-1][i]='w';}
    for(int j=0; j<height; j++){board[j][0]='w'; board[j][width-1]='w';}

    board[1][51] = 'E';
    board[1][52] = 'n';
    board[1][53] = 'e';
    board[1][54] = 'm';
    board[1][55] = 'y';
    board[1][56] = ':';

    //kill = score_map['n'] + score_map['r'] + score_map['s'] + score_map['d'] + score_map['a'];
    if (kill >= 7) {
        my_plane.charged = true;
    }
    else {
        my_plane.charged = false;
    }

    if (my_plane.charged == true) {
        board[2][52] = 'C';
        board[2][53] = 'h';
        board[2][54] = 'a';
        board[2][55] = 'r';
        board[2][56] = 'g';
        board[2][57] = 'e';
        board[2][58] = 'd';
    }
    else {
        board[2][52] = ' ';
        board[2][53] = ' ';
        board[2][54] = ' ';
        board[2][55] = ' ';
        board[2][56] = ' ';
        board[2][57] = ' ';
        board[2][58] = ' ';
    }

    board[3][54] = 'h';
    board[3][55] = 'p';
    board[3][56] = ':';

    int shot_frame, create_frame, check_frame;

    if (initial_map == 0) {
        score_map['n'] = 0;
        score_map['r'] = 0;
        score_map['s'] = 0;
        score_map['d'] = 0;
        score_map['a'] = 0;
        initial_map = 1;
    }

    //Bullet part
    shot_frame = this->my_plane.shot_frame_my_plane;
    create_frame = this->my_plane.create_frame_my_plane;
    check_frame = this->my_plane.check_frame_my_plane;
    
    while ((curr_frame-create_frame)/shot_frame - check_frame > 0){ //bullet create
        if (my_plane.charged == true && charge_bullet_call == true) {
            Charged_bullet charged_bullet = Charged_bullet(this->my_plane.y, this->my_plane.x, check_frame, this->my_plane.bullet_level);
            this->my_plane.bullet.push_back(charged_bullet);
            vec_bullet_board[my_plane.y-1+shot_frame][my_plane.x].push_back(charged_bullet);
            charge_bullet_call = false;
            kill = 0;
        }
        else {
            Bullet bullet = Bullet(this->my_plane.y-1+shot_frame, this->my_plane.x, check_frame, this->my_plane.bullet_level);
            this->my_plane.bullet.push_back(bullet);
            vec_bullet_board[my_plane.y-1+shot_frame][my_plane.x].push_back(bullet);
        }
        
        
        if (this->my_plane.power_up == true) {
            Bullet_left bullet_left = Bullet_left(this->my_plane.y-1+shot_frame, this->my_plane.x-1, check_frame, this->my_plane.bullet_level);
            this->my_plane.bullet.push_back(bullet_left);
            vec_bullet_board[my_plane.y-1+shot_frame][my_plane.x-1].push_back(bullet_left);
            Bullet_right bullet_right = Bullet_right(this->my_plane.y-1+shot_frame, this->my_plane.x+1, check_frame, this->my_plane.bullet_level);
            this->my_plane.bullet.push_back(bullet_right);
            vec_bullet_board[my_plane.y-1+shot_frame][my_plane.x+1].push_back(bullet_right);
        }
        
        cursorYX(3, 58); printf("%d", my_plane.hp % 10);
        cursorYX(3, 57); printf("%d", my_plane.hp / 10);

        cursorYX(1, 58); printf("%d", vec_enemy.size() % 10);
        cursorYX(1, 57); printf("%d", vec_enemy.size() / 10);

        //cursorYX(1, 57); printf("%d", (vec_enemy.size() / 10));

        /*if (vec_enemy_board[bullet.y][bullet.x].size()>0) {
        for (int i=0; i<vec_enemy_board[bullet.y][bullet.x].size(); i++) { // enemy hp redusing
            vec_enemy_board[bullet.y][bullet.x][i]->hp-=bullet.level;
            if (vec_enemy_board[bullet.y][bullet.x][i]->hp <= 0) {
                for (int j=0; j < vec_enemy.size(); j++) {
                    if (vec_enemy[j]->create_frame_enemy == vec_enemy_board[bullet.y][bullet.x][i]->create_frame_enemy) {
                        delete vec_enemy[j];
                        vec_enemy.erase(vec_enemy.begin() + j);
                        vec_enemy_board[bullet.y][bullet.x].erase(vec_enemy_board[bullet.y][bullet.x].begin() + i);
                        return;
                    }
                //delete vec_enemy_board[bullet.y][bullet.x][i];
                //vec_enemy_board[bullet.y][bullet.x].erase(vec_enemy_board[bullet.y][bullet.x].begin() + i);
                //return;
                }
            }
        }
        }*/

        for (int i=0; i<(sizeof(frame_event)); i++) {
            if (curr_frame == frame_event[i]) {
                if (type_event[i] == 'n') // creating object
                {
                    Enemy_1n* enemy_1n = new Enemy_1n(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_1n);
                    vec_enemy_board[enemy_1n->y][enemy_1n->x].push_back(enemy_1n);
                }
                else if (type_event[i] == 'r')
                {
                    Enemy_2r* enemy_2r = new Enemy_2r(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_2r);
                    vec_enemy_board[enemy_2r->y][enemy_2r->x].push_back(enemy_2r);
                }
                else if (type_event[i] == 's')
                {
                    Enemy_3s* enemy_3s = new Enemy_3s(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_3s);
                    vec_enemy_board[enemy_3s->y][enemy_3s->x].push_back(enemy_3s);
                }
                else if (type_event[i] == 'd')
                {
                    Enemy_4d* enemy_4d = new Enemy_4d(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_4d);
                    vec_enemy_board[enemy_4d->y][enemy_4d->x].push_back(enemy_4d);
                }
                else if (type_event[i] == 'a')
                {
                    Enemy_5a* enemy_5a = new Enemy_5a(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_5a);
                    vec_enemy_board[enemy_5a->y][enemy_5a->x].push_back(enemy_5a);
                }
                else if (type_event[i] == 'b')
                {
                    Enemy_6b* enemy_6b = new Enemy_6b(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_6b);
                    vec_enemy_board[enemy_6b->y][enemy_6b->x].push_back(enemy_6b);
                }
                else if (type_event[i] == 'P')
                {
                    Powerup_bullet* powerup_bullet = new Powerup_bullet(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_bullet_change.push_back(powerup_bullet);
                }
                else if (type_event[i] == 'L')
                {
                    Levelup_bullet* levelup_bullet = new Levelup_bullet(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_bullet_change.push_back(levelup_bullet);
                }
                else if (type_event[i] == 'H')
                {
                    Hp_plus* hp_plus = new Hp_plus(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_bullet_change.push_back(hp_plus);
                }
            }
        }

        /*for(auto iter=vec_enemy.begin(); iter<vec_enemy.end(); ) {
            if(iter->type == 'r') {
                if ((curr_frame-iter->create_frame_enemy)/iter->cell_speed - iter->check_frame_enemy > 0) {
                    if(iter->y>=31){
                        board[iter->y][iter->x]=' ';
                        this->vec_enemy.erase(iter);
                    }
                    else{
                        board[iter->y][iter->x]=' ';
                        iter->y -= 1;
                        board[iter->y][iter->x]=iter->type;
                        iter++;
                    }
                }
            }
        }*/

        for(auto iter=this->my_plane.bullet.begin(); iter<this->my_plane.bullet.end(); ){
            if(iter->y<=0){
                board[iter->y][iter->x]=' ';
                this->cor_vec_bullet_board = false;
                for (int i=0; i<vec_bullet_board[iter->y][iter->x].size(); i++) {
                    if ((vec_bullet_board[iter->y][iter->x][i].create_frame_bullet == iter->create_frame_bullet) && (vec_bullet_board[iter->y][iter->x][i].direction == iter->direction)) {
                        this->erase_idx_vec_bullet_board = i;
                        //cursorYX(1, 2); printf("%c", '2');
                        this->cor_vec_bullet_board = true;
                    }
                }
                if (this->cor_vec_bullet_board == true) {
                    vec_bullet_board[iter->y][iter->x].erase(vec_bullet_board[iter->y][iter->x].begin() + this->erase_idx_vec_bullet_board);
                }
                this->my_plane.bullet.erase(iter);
            }
            else{
                if(iter!=(this->my_plane.bullet.end()-1) && curr_frame!=1){
                    board[iter->y][iter->x]=' ';
                    this->cor_vec_bullet_board = false;
                    for (int i=0; i<vec_bullet_board[iter->y][iter->x].size(); i++) {
                        if ((vec_bullet_board[iter->y][iter->x][i].create_frame_bullet == iter->create_frame_bullet) && (vec_bullet_board[iter->y][iter->x][i].direction == iter->direction)) {
                            this->erase_idx_vec_bullet_board = i;
                            //cursorYX(1, 2); printf("%c", '2');
                            this->cor_vec_bullet_board = true;
                        }
                    }
                    if (this->cor_vec_bullet_board == true) {
                        vec_bullet_board[iter->y][iter->x].erase(vec_bullet_board[iter->y][iter->x].begin() + this->erase_idx_vec_bullet_board);
                    }
                }
                iter->y -= shot_frame;
                board[iter->y][iter->x]=iter->bullet_shape;
                vec_bullet_board[iter->y][iter->x].push_back(*iter);
                if (vec_enemy_board[iter->y][iter->x].size()>0) {
                    for (int i=0; i<vec_enemy_board[iter->y][iter->x].size(); i++) { // enemy hp redusing
                        vec_enemy_board[iter->y][iter->x][i]->hp-=iter->level;
                        if (vec_enemy_board[iter->y][iter->x][i]->hp <= 0) {
                            for (int j=0; j < vec_enemy.size(); j++) {
                                if (vec_enemy[j]->create_frame_enemy == vec_enemy_board[iter->y][iter->x][i]->create_frame_enemy) {
                                    score_map[vec_enemy[j]->type] ++;
                                    delete vec_enemy[j];
                                    kill ++;
                                    vec_enemy.erase(vec_enemy.begin() + j);
                                    vec_enemy_board[iter->y][iter->x].erase(vec_enemy_board[iter->y][iter->x].begin() + i);
                                    break;
                                }
                            //delete vec_enemy_board[bullet.y][bullet.x][i];
                            //vec_enemy_board[iter->y][iter->x].erase(vec_enemy_board[iter->y][iter->x].begin() + i);
                            //return;
                            }
                        }
                    }
                }
                iter++;
            }
        }

        for (auto iter = this->vec_enemy_bullet.begin(); iter<this->vec_enemy_bullet.end();) {
            if (((iter->enemy_b_type == 's' || iter->enemy_b_type == 'b') && iter->y>=28) || (iter->enemy_b_type == 'd' && (iter->y>=28 || iter->x>=58))) {
                board[iter->y][iter->x]=' ';
                this->cor_vec_enemy_bullet_board = false;
                for (int i=0; i<vec_enemy_bullet_board[iter->y][iter->x].size(); i++) {
                    if ((vec_enemy_bullet_board[iter->y][iter->x][i].create_frame_enemy_bullet == iter->create_frame_enemy_bullet) \
                    && (vec_enemy_bullet_board[iter->y][iter->x][i].frame_enemy == iter->frame_enemy) \
                    && (vec_enemy_bullet_board[iter->y][iter->x][i].direction_e == iter->direction_e)) {
                        this->erase_idx_vec_enemy_bullet_board = i;
                        //cursorYX(1, 2); printf("%c", '2');
                        this->cor_vec_enemy_bullet_board = true;
                    }    
                }
                if (this->cor_vec_enemy_bullet_board == true) {
                    //vec_enemy_bullet_board[iter->y][iter->x] = {};
                    vec_enemy_bullet_board[iter->y][iter->x].erase(vec_enemy_bullet_board[iter->y][iter->x].begin() + this->erase_idx_vec_enemy_bullet_board);
                }

                
                this->vec_enemy_bullet.erase(iter);
            }
            else {
                //if(iter!=(this->vec_enemy_bullet.end()-1) && curr_frame!=1){
                if(curr_frame!=1){
                    board[iter->y][iter->x]=' ';
                    this->cor_vec_enemy_bullet_board = false;
                    for (int i=0; i<vec_enemy_bullet_board[iter->y][iter->x].size(); i++) {
                        if ((vec_enemy_bullet_board[iter->y][iter->x][i].create_frame_enemy_bullet == iter->create_frame_enemy_bullet) \
                        && (vec_enemy_bullet_board[iter->y][iter->x][i].frame_enemy == iter->frame_enemy) \
                        && (vec_enemy_bullet_board[iter->y][iter->x][i].direction_e == iter->direction_e)) {
                            this->erase_idx_vec_enemy_bullet_board = i;
                            //cursorYX(1, 2); printf("%c", '2');
                            this->cor_vec_enemy_bullet_board = true;
                        }    
                    }
                    if (this->cor_vec_enemy_bullet_board == true) {
                        //vec_enemy_bullet_board[iter->y][iter->x] = {};
                        vec_enemy_bullet_board[iter->y][iter->x].erase(vec_enemy_bullet_board[iter->y][iter->x].begin() + this->erase_idx_vec_enemy_bullet_board);
                    }
                }
                if (iter->enemy_b_type == 's' || iter->enemy_b_type == 'b') {
                    iter->y += shot_frame;
                }
                else if (iter->enemy_b_type == 'd') {
                    iter->y += shot_frame;
                    iter->x += shot_frame;
                }
                board[iter->y][iter->x]='*';
                vec_enemy_bullet_board[iter->y][iter->x].push_back(*iter);
                if ((my_plane.y == iter->y) && (my_plane.x == iter->x)) {
                    my_plane.hp-=iter->damage;
                }
                iter++;
            }  
        }
        

        this->my_plane.check_frame_my_plane+=1;
        check_frame++;
        for (int i=0; i < vec_bullet_change.size(); i++) {
            vec_bullet_change[i]->check_frame_bullet_change++;
        }


        /*if (vec_bullet_board[1][28].size()==0) {
            cursorYX(1, 2); printf("%c", '2');
        }
        else if (vec_bullet_board[1][28].size()==2) {
            cursorYX(1, 2); printf("%c", '3');
        }
        else {
            cursorYX(1, 2); printf("%c", '1');
        }*/
    }
    //Bullet part ends
    /*for(auto iter=vec_enemy.begin(); iter<vec_enemy.end(); ) {
        //
        if(iter->type == 'r') {
            cursorYX(1, 1); printf("%c", '1');
            while ((curr_frame-iter->create_frame_enemy)/iter->cell_speed - iter->check_frame_enemy > 0) {
                cursorYX(1, 2); printf("%c", '2');
                if(iter->y>=31){
                    board[iter->y][iter->x]=' ';
                    this->vec_enemy.erase(iter);
                }
                else{
                    board[iter->y][iter->x]=' ';
                    iter->y -= 1;
                    board[iter->y][iter->x]=iter->type;
                    iter->check_frame_enemy++;
                    iter++;
                }
            }
        }
    }*/
    for(int i=0; i<vec_enemy.size(); i++) {
        //
        for (int k=0; k<vec_enemy.size(); k++) {
            if (vec_enemy[k]->type == 'a') {
                if ((vec_enemy[k]->in_buff_area(vec_enemy[i]) == true) && (vec_enemy[k]->create_frame_enemy != vec_enemy[i]->create_frame_enemy)){
                    vec_enemy[i]->buffed = true;
                }
            }
        }
        if(vec_enemy[i]->type == 'r') {
            //cursorYX(1, 1); printf("%c", '1');
            while ((curr_frame-vec_enemy[i]->create_frame_enemy)/vec_enemy[i]->cell_speed - vec_enemy[i]->check_frame_enemy > 0) {
                //cursorYX(1, 2); printf("%c", '2');
                if(vec_enemy[i]->y>=28){
                    board[vec_enemy[i]->y][vec_enemy[i]->x]=' ';
                    this->cor_vec_enemy_board = false;
                    for (int j=0; j<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                        if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][j]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                            this->erase_idx_vec_enemy_board = j;
                            //cursorYX(1, 2); printf("%c", '2');
                            this->cor_vec_enemy_board = true;
                        }
                    }
                    if (this->cor_vec_enemy_board == true) {
                        vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() + this->erase_idx_vec_enemy_board);
                    }
                    delete vec_enemy[i];
                    vec_enemy.erase(vec_enemy.begin() + i);
                    return;
                }
                else{
                    board[vec_enemy[i]->y][vec_enemy[i]->x]=' ';
                    this->cor_vec_enemy_board = false;
                    for (int j=0; j<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                        if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][j]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                            this->erase_idx_vec_enemy_board = j;
                            //cursorYX(1, 2); printf("%c", '2');
                            this->cor_vec_enemy_board = true;
                        }
                    }
                    if (this->cor_vec_enemy_board == true) {
                        vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() + this->erase_idx_vec_enemy_board);
                    }
                    vec_enemy[i]->y += 1;
                    for (int k=0; k<vec_enemy.size(); k++) {
                        if (vec_enemy[k]->type == 'a') {
                            if (vec_enemy[k]->in_buff_area(vec_enemy[i]) == false){
                                vec_enemy[i]->buffed = false;
                            }
                        }
                    }

                    board[vec_enemy[i]->y][vec_enemy[i]->x]=vec_enemy[i]->type;

                    vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].push_back(vec_enemy[i]);
                    vec_enemy[i]->check_frame_enemy++;
                    if (vec_enemy[i]->y == my_plane.y && vec_enemy[i]->x == my_plane.x) {
                        my_plane.hp --;
                    }
                    if (vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].size() > 0) {
                        for (int j=0; j < vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                            vec_enemy[i]->hp -= vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x][j].level;
                        }
                        if (vec_enemy[i]->hp <= 0) {
                            for (int k=0; k<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); k++) {
                                if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][k]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                                    vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() + k);
                                }
                            }
                            score_map[vec_enemy[i]->type] ++;
                            delete vec_enemy[i];
                            kill++;
                            vec_enemy.erase(vec_enemy.begin() + i);
                            //vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin()
                            // + (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size() -1));
                            return;
                        }
                    }
                    /*if (vec_enemy_board[5][5].size()==1) {
                        cursorYX(1, 2); printf("%c", '2');
                    }
                    else {
                        cursorYX(1, 2); printf("%c", '1');
                    }*/
                }
            }
        }
        else if(vec_enemy[i]->type == 's') {
            //cursorYX(1, 1); printf("%c", '1');
            while ((curr_frame-vec_enemy[i]->create_frame_enemy)/vec_enemy[i]->cell_speed - vec_enemy[i]->check_frame_enemy > 0) {
                //cursorYX(1, 2); printf("%c", '2');
                if(vec_enemy[i]->y>=28){
                    board[vec_enemy[i]->y][vec_enemy[i]->x]=' ';
                    this->cor_vec_enemy_board = false;
                    for (int j=0; j<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                        if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][j]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                            this->erase_idx_vec_enemy_board = j;
                            //cursorYX(1, 2); printf("%c", '2');
                            this->cor_vec_enemy_board = true;
                        }
                    }
                    if (this->cor_vec_enemy_board == true) {
                        vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() + this->erase_idx_vec_enemy_board);
                    }
                    delete vec_enemy[i];
                    vec_enemy.erase(vec_enemy.begin() + i);
                    return;
                }
                else{
                    board[vec_enemy[i]->y][vec_enemy[i]->x]=' ';
                    this->cor_vec_enemy_board = false;
                    for (int j=0; j<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                        if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][j]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                            this->erase_idx_vec_enemy_board = j;
                            //cursorYX(1, 2); printf("%c", '2');
                            this->cor_vec_enemy_board = true;
                        }
                    }
                    if (this->cor_vec_enemy_board == true) {
                        vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() + this->erase_idx_vec_enemy_board);
                    }
                    vec_enemy[i]->y += 1;
                    for (int k=0; k<vec_enemy.size(); k++) {
                        if (vec_enemy[k]->type == 'a') {
                            if (vec_enemy[i]->y > (vec_enemy[k]->y+3)){
                                vec_enemy[i]->buffed = false;
                            }
                        }
                    }

                    board[vec_enemy[i]->y][vec_enemy[i]->x]=vec_enemy[i]->type;
                    vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].push_back(vec_enemy[i]);
                    Enemy_bullet enemy_bullet = Enemy_bullet(vec_enemy[i]->y, vec_enemy[i]->x, vec_enemy[i]->check_frame_enemy, // vec_enemy[i]->check_frame_enemy : 식별자
                      vec_enemy[i]->create_frame_enemy, vec_enemy[i]->bullet_damage, vec_enemy[i]->type);
                    this->vec_enemy_bullet.push_back(enemy_bullet);
                    vec_enemy_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].push_back(enemy_bullet);
                    vec_enemy[i]->check_frame_enemy++;
                    if (vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].size() > 0) {
                        for (int j=0; j < vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                            vec_enemy[i]->hp -= vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x][j].level;
                        }
                        if (vec_enemy[i]->hp <= 0) {
                            for (int k=0; k<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); k++) {
                                if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][k]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                                    vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() + k);
                                }
                            }
                            score_map[vec_enemy[i]->type] ++;
                            delete vec_enemy[i];
                            kill++;
                            vec_enemy.erase(vec_enemy.begin() + i);
                            //vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin()
                            // + (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size() -1));
                            return;
                        }
                    }         
                    /*if (vec_enemy_board[7][40].size()==1) {
                        cursorYX(1, 2); printf("%c", '2');
                    }
                    else {
                        cursorYX(1, 2); printf("%c", '1');
                    } */     
                }
            }
        }
        else if(vec_enemy[i]->type == 'd') {
            //cursorYX(1, 1); printf("%c", '1');
            while ((curr_frame-vec_enemy[i]->create_frame_enemy)/vec_enemy[i]->cell_speed - vec_enemy[i]->check_frame_enemy > 0) {
                //cursorYX(1, 2); printf("%c", '2');
                if(vec_enemy[i]->y>=28){
                    board[vec_enemy[i]->y][vec_enemy[i]->x]=' ';
                    this->cor_vec_enemy_board = false;
                    for (int j=0; j<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                        if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][j]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                            this->erase_idx_vec_enemy_board = j;
                            //cursorYX(1, 2); printf("%c", '2');
                            this->cor_vec_enemy_board = true;
                        }
                    }
                    if (this->cor_vec_enemy_board == true) {
                        vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() + this->erase_idx_vec_enemy_board);
                    }
                    delete vec_enemy[i];
                    vec_enemy.erase(vec_enemy.begin() + i);
                    return;
                }
                else{
                    board[vec_enemy[i]->y][vec_enemy[i]->x]=' ';
                    this->cor_vec_enemy_board = false;
                    for (int j=0; j<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                        if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][j]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                            this->erase_idx_vec_enemy_board = j;
                            //cursorYX(1, 2); printf("%c", '2');
                            this->cor_vec_enemy_board = true;
                        }
                    }
                    if (this->cor_vec_enemy_board == true) {
                        //cursorYX(1, 2); printf("%c", '2');
                        vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() 
                        + this->erase_idx_vec_enemy_board);
                        if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size()==0) {
                            //cursorYX(1, vec_enemy[i]->check_frame_enemy); printf("%c", '2');
                        }
                    }
                    vec_enemy[i]->y += 1;
                    for (int k=0; k<vec_enemy.size(); k++) {
                        if (vec_enemy[k]->type == 'a') {
                            if (vec_enemy[k]->in_buff_area(vec_enemy[i]) == false){
                                vec_enemy[i]->buffed = false;
                            }
                        }
                    }

                    board[vec_enemy[i]->y][vec_enemy[i]->x]=vec_enemy[i]->type;
                    vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].push_back(vec_enemy[i]);
                    Enemy_bullet enemy_bullet = Enemy_bullet(vec_enemy[i]->y, vec_enemy[i]->x, vec_enemy[i]->check_frame_enemy, \
                      vec_enemy[i]->create_frame_enemy, vec_enemy[i]->bullet_damage, vec_enemy[i]->type);
                    this->vec_enemy_bullet.push_back(enemy_bullet);
                    vec_enemy_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].push_back(enemy_bullet);
                    vec_enemy[i]->check_frame_enemy++;
                    if (vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].size() > 0) {
                        for (int j=0; j < vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); j++) {
                            vec_enemy[i]->hp -= vec_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x][j].level;
                        }
                        if (vec_enemy[i]->hp <= 0) {
                            for (int k=0; k<vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size(); k++) {
                                if (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x][k]->create_frame_enemy == vec_enemy[i]->create_frame_enemy) {
                                    vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin() + k);
                                }
                            }
                            score_map[vec_enemy[i]->type] ++;
                            delete vec_enemy[i];
                            kill++;
                            vec_enemy.erase(vec_enemy.begin() + i);
                            //vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].erase(vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].begin()
                            // + (vec_enemy_board[vec_enemy[i]->y][vec_enemy[i]->x].size() -1));
                            return;
                        }
                    }
                    /*if (vec_enemy_board[15][50].size()==1) {
                        cursorYX(1, 2); printf("%c", '2');
                    }
                    else {
                        cursorYX(1, 2); printf("%c", '1');
                    }*/
                }
            }
        }
        else if (vec_enemy[i]->type == 'b') {
            while ((curr_frame-vec_enemy[i]->create_frame_enemy)/vec_enemy[i]->cell_speed - vec_enemy[i]->check_frame_enemy > 0) {
                Enemy_bullet enemy_bullet = Enemy_bullet(vec_enemy[i]->y, vec_enemy[i]->x, vec_enemy[i]->check_frame_enemy, \
                  vec_enemy[i]->create_frame_enemy, vec_enemy[i]->bullet_damage, vec_enemy[i]->type);
                Enemy_bullet_left enemy_bullet_left = Enemy_bullet_left(vec_enemy[i]->y, (vec_enemy[i]->x-1), vec_enemy[i]->check_frame_enemy, \
                  vec_enemy[i]->create_frame_enemy, vec_enemy[i]->bullet_damage, vec_enemy[i]->type);
                Enemy_bullet_right enemy_bullet_right = Enemy_bullet_right(vec_enemy[i]->y, (vec_enemy[i]->x+1), vec_enemy[i]->check_frame_enemy, \
                  vec_enemy[i]->create_frame_enemy, vec_enemy[i]->bullet_damage, vec_enemy[i]->type);
                this->vec_enemy_bullet.push_back(enemy_bullet);
                this->vec_enemy_bullet.push_back(enemy_bullet_left);
                this->vec_enemy_bullet.push_back(enemy_bullet_right);
                vec_enemy_bullet_board[vec_enemy[i]->y][vec_enemy[i]->x].push_back(enemy_bullet);
                vec_enemy_bullet_board[vec_enemy[i]->y][(vec_enemy[i]->x-1)].push_back(enemy_bullet_left);
                vec_enemy_bullet_board[vec_enemy[i]->y][(vec_enemy[i]->x+1)].push_back(enemy_bullet_right);
                vec_enemy[i]->check_frame_enemy++;
            }
        }
        else if (vec_enemy[i]->type == 'a') {
            while ((curr_frame-vec_enemy[i]->create_frame_enemy)/vec_enemy[i]->buff_speed - vec_enemy[i]->check_frame_enemy > 0) {
                for (int j=0; j<vec_enemy.size(); j++) {
                    if ((vec_enemy[j]->create_frame_enemy!=vec_enemy[i]->create_frame_enemy) && (vec_enemy[i]->in_buff_area(vec_enemy[j]))) {
                        if (vec_enemy[j]->type != 'b') {
                            vec_enemy[j]->bullet_damage += 1;
                        }
                    }
                }
                vec_enemy[i]->check_frame_enemy++;
            }
        }
    }



    score = (score_map['n']*1) + (score_map['r']*2) + (score_map['s']*3) + (score_map['d']*4) + (score_map['a']*5);
    if ((my_plane.hp <= 0 || vec_enemy.size() <= 0) && curr_frame >= 3) {
        end = 1;
    }
}

//print when key didn't pressed
void Screen_manager::print(){
    board[this->my_plane.y][this->my_plane.x]='M';
    for (int i=0; i < vec_enemy.size(); i++) { // printing object
        if (board[this->vec_enemy[i]->y][this->vec_enemy[i]->x] != 'M') {
            if (vec_enemy[i]->buffed == true) {
                board[this->vec_enemy[i]->y][this->vec_enemy[i]->x] = toupper(vec_enemy[i]->type);
            }
            else {
                board[this->vec_enemy[i]->y][this->vec_enemy[i]->x] = vec_enemy[i]->type;
            }
            
        }
        //board[this->vec_enemy[i].y][this->vec_enemy[i].x] = vec_enemy[i].type;
    }
    for (int i=0; i < vec_bullet_change.size(); i++) {
        if (board[this->vec_bullet_change[i]->y][this->vec_bullet_change[i]->x] != 'M') {
            board[this->vec_bullet_change[i]->y][this->vec_bullet_change[i]->x] = vec_bullet_change[i]->type;
        }
        //board[this->vec_bullet_change[i].y][this->vec_bullet_change[i].x] = vec_bullet_change[i].type;
    }
    print_share();
}

//print when key pressed
void Screen_manager::print(int ch){ //ascii
    for (int i=0; i < vec_enemy.size(); i++) { // printing object
        if (board[this->vec_enemy[i]->y][this->vec_enemy[i]->x] != 'M') {
            if (vec_enemy[i]->buffed == true) {
                board[this->vec_enemy[i]->y][this->vec_enemy[i]->x] = toupper(vec_enemy[i]->type);
            }
            else {
                board[this->vec_enemy[i]->y][this->vec_enemy[i]->x] = vec_enemy[i]->type;
            }
            
        }
        //board[this->vec_enemy[i].y][this->vec_enemy[i].x] = vec_enemy[i].type;
    }
    for (int i=0; i < vec_bullet_change.size(); i++) {
        if (board[this->vec_bullet_change[i]->y][this->vec_bullet_change[i]->x] != 'M') {
            board[this->vec_bullet_change[i]->y][this->vec_bullet_change[i]->x] = vec_bullet_change[i]->type;
        }
        //board[this->vec_bullet_change[i].y][this->vec_bullet_change[i].x] = vec_bullet_change[i].type;
    }
    if(ch=='d'){ //right
        if(this->my_plane.y >=0 && this->my_plane.y <(height-1) && this->my_plane.x >0 && this->my_plane.x < (width-2)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.x +=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='a'){ //left
        if(this->my_plane.y >=0 && this->my_plane.y <(height-1) && this->my_plane.x >1 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.x -=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='w'){ //up
        if(this->my_plane.y >0 && this->my_plane.y <(height-1) && this->my_plane.x >0 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.y -=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='s'){ //down
        if(this->my_plane.y >=0 && this->my_plane.y <(height-2) && this->my_plane.x >0 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.y +=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }    
    }
    else if (ch == 'c') {
        charge_bullet_call = true;

    }
    if (vec_enemy_board[my_plane.y][my_plane.x].size() > 0) {
        my_plane.hp --;
        //cursorYX(0, 1); printf("%c", '2');
    }
    if (vec_enemy_bullet_board[my_plane.y][my_plane.x].size() > 0) {
        for (int i=0; i<vec_enemy_bullet_board[my_plane.y][my_plane.x].size(); i++) {
            //cursorYX(1, 56); printf("%c", '2');
            my_plane.hp-=vec_enemy_bullet_board[my_plane.y][my_plane.x][i].damage;
        }
    }
    for (int i=0; i<vec_bullet_change.size(); i++) {
        if ((my_plane.y == vec_bullet_change[i]->y) && (my_plane.x == vec_bullet_change[i]->x)) {
            if (vec_bullet_change[i]->type == 'P') {
                my_plane.power_up = true;
            }
            else if (vec_bullet_change[i]->type == 'L') {
                if (my_plane.bullet_level < 3) {
                    my_plane.bullet_level++;
                }
            }
            else if (vec_bullet_change[i]->type == 'H') {
                my_plane.hp+=5;
            }
            delete vec_bullet_change[i];
            vec_bullet_change.erase(vec_bullet_change.begin() + i);
        }
    }

    print_share();
}
