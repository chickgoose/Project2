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

    int shot_frame, create_frame, check_frame;

    //Bullet part
    shot_frame = this->my_plane.shot_frame_my_plane;
    create_frame = this->my_plane.create_frame_my_plane;
    check_frame = this->my_plane.check_frame_my_plane;
    while ((curr_frame-create_frame)/shot_frame - check_frame > 0){ //bullet create
        Bullet bullet = Bullet(this->my_plane.y-1+shot_frame, this->my_plane.x, check_frame);

        this->my_plane.bullet.push_back(bullet);

        for (int i=0; i<(sizeof(frame_event)); i++) {
            if (curr_frame == frame_event[i]) {
                if (type_event[i] == 'n') // creating object
                {
                    Enemy_1n enemy_1n = Enemy_1n(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_1n);
                }
                else if (type_event[i] == 'r')
                {
                    Enemy_2r enemy_2r = Enemy_2r(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_2r);
                }
                else if (type_event[i] == 's')
                {
                    Enemy_3s enemy_3s = Enemy_3s(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_3s);
                }
                else if (type_event[i] == 'd')
                {
                    Enemy_4d enemy_4d = Enemy_4d(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_4d);
                }
                else if (type_event[i] == 'a')
                {
                    Enemy_5a enemy_5a = Enemy_5a(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_enemy.push_back(enemy_5a);
                }
                else if (type_event[i] == 'P')
                {
                    Powerup_bullet powerup_bullet = Powerup_bullet(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_bullet_change.push_back(powerup_bullet);
                }
                else if (type_event[i] == 'L')
                {
                    Levelup_bullet levelup_bullet = Levelup_bullet(y_event[i], x_event[i], frame_event[i], type_event[i]);
                    vec_bullet_change.push_back(levelup_bullet);
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
                this->my_plane.bullet.erase(iter);
            }
            else{
                if(iter!=(this->my_plane.bullet.end()-1) && curr_frame!=1){
                    board[iter->y][iter->x]=' ';
                }
                iter->y -= shot_frame;
                board[iter->y][iter->x]='\'';
                iter++;
            }
        }
        this->my_plane.check_frame_my_plane+=1;
        check_frame++;
        for (int i=0; i < vec_bullet_change.size(); i++) {
            vec_bullet_change[i].check_frame_bullet_change++;
        }
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
        if(vec_enemy[i].type == 'r') {
            //cursorYX(1, 1); printf("%c", '1');
            while ((curr_frame-vec_enemy[i].create_frame_enemy)/vec_enemy[i].cell_speed - vec_enemy[i].check_frame_enemy > 0) {
                //cursorYX(1, 2); printf("%c", '2');
                if(vec_enemy[i].y>=29){
                    board[vec_enemy[i].y][vec_enemy[i].x]=' ';
                    this->vec_enemy.erase(vec_enemy.begin()+i);
                }
                else{
                    board[vec_enemy[i].y][vec_enemy[i].x]=' ';
                    vec_enemy[i].y += 1;
                    board[vec_enemy[i].y][vec_enemy[i].x]=vec_enemy[i].type;
                    vec_enemy[i].check_frame_enemy++;                
                }
            }
        }
        else if(vec_enemy[i].type == 's') {
            //cursorYX(1, 1); printf("%c", '1');
            while ((curr_frame-vec_enemy[i].create_frame_enemy)/vec_enemy[i].cell_speed - vec_enemy[i].check_frame_enemy > 0) {
                //cursorYX(1, 2); printf("%c", '2');
                if(vec_enemy[i].y>=29){
                    board[vec_enemy[i].y][vec_enemy[i].x]=' ';
                    this->vec_enemy.erase(vec_enemy.begin()+i);
                }
                else{
                    board[vec_enemy[i].y][vec_enemy[i].x]=' ';
                    vec_enemy[i].y += 1;
                    board[vec_enemy[i].y][vec_enemy[i].x]=vec_enemy[i].type;
                    vec_enemy[i].check_frame_enemy++;                
                }
            }
        }
        else if(vec_enemy[i].type == 'd') {
            //cursorYX(1, 1); printf("%c", '1');
            while ((curr_frame-vec_enemy[i].create_frame_enemy)/vec_enemy[i].cell_speed - vec_enemy[i].check_frame_enemy > 0) {
                //cursorYX(1, 2); printf("%c", '2');
                if(vec_enemy[i].y>=29){
                    board[vec_enemy[i].y][vec_enemy[i].x]=' ';
                    this->vec_enemy.erase(vec_enemy.begin()+i);
                }
                else{
                    board[vec_enemy[i].y][vec_enemy[i].x]=' ';
                    vec_enemy[i].y += 1;
                    board[vec_enemy[i].y][vec_enemy[i].x]=vec_enemy[i].type;
                    vec_enemy[i].check_frame_enemy++;                
                }
            }
        }
    }
}

//print when key didn't pressed
void Screen_manager::print(){
    board[this->my_plane.y][this->my_plane.x]='M';
    for (int i=0; i < vec_enemy.size(); i++) { // printing object
        if (board[this->vec_enemy[i].y][this->vec_enemy[i].x] != 'M') {
            board[this->vec_enemy[i].y][this->vec_enemy[i].x] = vec_enemy[i].type;
        }
        //board[this->vec_enemy[i].y][this->vec_enemy[i].x] = vec_enemy[i].type;
    }
    for (int i=0; i < vec_bullet_change.size(); i++) {
        if (board[this->vec_bullet_change[i].y][this->vec_bullet_change[i].x] != 'M') {
            board[this->vec_bullet_change[i].y][this->vec_bullet_change[i].x] = vec_bullet_change[i].type;
        }
        //board[this->vec_bullet_change[i].y][this->vec_bullet_change[i].x] = vec_bullet_change[i].type;
    }
    print_share();
}

//print when key pressed
void Screen_manager::print(int ch){ //ascii
    for (int i=0; i < vec_enemy.size(); i++) { // printing object
        if (board[this->vec_enemy[i].y][this->vec_enemy[i].x] != 'M') {
            board[this->vec_enemy[i].y][this->vec_enemy[i].x] = vec_enemy[i].type;
        }
        //board[this->vec_enemy[i].y][this->vec_enemy[i].x] = vec_enemy[i].type;
    }
    for (int i=0; i < vec_bullet_change.size(); i++) {
        if (board[this->vec_bullet_change[i].y][this->vec_bullet_change[i].x] != 'M') {
            board[this->vec_bullet_change[i].y][this->vec_bullet_change[i].x] = vec_bullet_change[i].type;
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

    print_share();
}