#pragma once
#include <string>

const static int SCREEN_WIDTH = 1024;
const static int SCREEN_HEIGHT = 768;
const static std::string GAME_NAME = "Runner";

const static int GAME_TILESIZE = 32;
const static int GAME_MENU_BUTTON_HEIGHT = 32;
const static int GAME_MENU_BUTTON_WIDTH = 192;

const static float BULLET_SPEED = 600.0f;
const static float PLAYER_RADIUS = 20.0f;
const static float PLAYER_SPEED = 300.0f;

const static float SAT_MAX_OVERLAP = (SCREEN_HEIGHT > SCREEN_WIDTH) ? SCREEN_HEIGHT : SCREEN_WIDTH;
const static int HITBOX_MAX_SIZE = 5;
const static float M_PI = 3.14159265358979323846f;
