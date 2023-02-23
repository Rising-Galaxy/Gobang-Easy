#ifndef _GoBang_
#define _GoBang_

#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#pragma comment(lib, "winmm.lib")

typedef struct bt
{
    int left, right, up, down;
} Button;
typedef struct reviewer_
{
    int x, y, color;
    struct reviewer_ *next;
} review;

int bt_if(const int x, const int y, const int mode);
void drawPNG(IMAGE *picture, int picture_x, int picture_y);
int folder(char *folder_name);
void load_gui();
void _bgm_play();
int mycmp(char *a);
void _bgm_repeat();
void clear_chess();
void _bg_put();
void menu_put_1();
void menu_put_2();
void menu_put_3();
void _tips_put();
void _tips_put_2();
void _tips_put_3();
void _tips_put_4();
void screenshot_();
void draw_bg();
void restart_();
void draw_piece(int num, const int m, const int n);
void change_chess(int num, const int m, const int n);
void review_(review *head);
void free_list(review *head);
int AI_score(int x, int y);
void AI_random(int *x, int *y);
void AI_thinking(int *x, int *y);
int victory(int chess_color, int x, int y);
void _play_hard(review *head);
void _play_easy(review *head);
void _begin__();

#endif