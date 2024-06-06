#ifndef _MAIN_H
#define _MAIN_H
#include "../include/entt/entt.hpp"
#include "sim_settings.hpp"

/**
 * Prints help
 */
void ShowHelp();

/**
 * Parses user input and return filled sim_settings struct
 */
sim_settings ParseUserInput(int argc, char *argv[]);

/**
 * creates, fills and return sim_data struct
 */
sim_data GetSimData(sim_settings settings);

int main(int argc, char *argv[]);

#endif