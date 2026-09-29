/**
 * GLOBALS
 * Allan Legemaate
 * 26/10/2017
 **/
#pragma once

#include <asw/asw.h>
#include <string>

// Config
extern bool config_double_click;
extern int difficulty;

// Text drop shadow, matching the title art
inline const asw::Color TEXT_SHADOW{128, 128, 128};
inline const asw::Vec2f SHADOW_OFFSET{2.0F, 2.0F};
