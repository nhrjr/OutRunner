#pragma once
#include <string>

const static int SCREEN_WIDTH = 1400;
const static int SCREEN_HEIGHT = 768;
const static std::string GAME_NAME = "Runner";
const static unsigned int NETWORK_BINARYBUFFER_SIZE = 1200;

const static std::string GAME_MAP_NAME = "data/map.dat";
const static std::string GAME_MAP_NAME_REMOTE = "data/map_multiplayer.dat";

const static int GAME_TILESIZE = 32;
const static int GAME_MENU_BUTTON_HEIGHT = 32;
const static int GAME_MENU_BUTTON_WIDTH = 256;
const static int GAME_CONSOLE_HISTORYLENGTH = 30;
const static int GAME_CONSOLE_INPUTBUFFER_LENGTH = 300;

const static float BULLET_SPEED = 1200.0f;
const static float PLAYER_RADIUS = 20.0f;
const static float PLAYER_SPEED = 500.0f;
const static float PLAYER_TIMEOUT = 10.0f;

const static float SAT_MAX_OVERLAP = (SCREEN_HEIGHT > SCREEN_WIDTH) ? SCREEN_HEIGHT : SCREEN_WIDTH;
const static int HITBOX_MAX_SIZE = 5;
const static float M_PI = 3.14159265358979323846f;
