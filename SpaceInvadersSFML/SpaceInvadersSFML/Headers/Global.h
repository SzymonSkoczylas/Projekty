#ifndef GLOBAL_H
#define GLOBAL_H
//Tutaj przechowywane sa zmienne stale 

//Bazowa wielkosc pomocna do ustawiania obiektow
constexpr unsigned char BASE_SIZE = 60;

//Gracz
constexpr unsigned char PLAYER_BASE_SPEED = 4;
constexpr unsigned char PLAYER_BULLET_BASE_SPEED = 4;
constexpr double		PLAYER_RELOAD_SPEED = 1.0;

//Enemy
constexpr unsigned char ENEMY_BASE_SPEED = 4;


//Wlasciwosci ekranu 
constexpr unsigned short SCREEN_HEIGHT = 180;
constexpr unsigned short SCREEN_WIDTH = 320;
constexpr unsigned char  SCREEN_RESIZE = 4;

#endif 