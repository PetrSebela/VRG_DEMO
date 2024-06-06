#ifndef _MAIN_H
#define _MAIN_H
#include "../include/entt/entt.hpp"
#include "sim_settings.hpp"

/**
 * Updates all entities with transform inside provided registry and checks if any entities colided
 * Compensates for simulation error
 * @param registry reference to entt registry
 * @param interation count of iteration steps, used to compensate simulation error
 */
void UpdateScene(entt::registry &registry, int iteration);

/**
 * Prints help
 */
void ShowHelp();

/**
 * Parses user input
 */
sim_settings ParseUserInput(int argc, char *argv[]);

/**
 * Creates bullet and target objects in registry
 */
void SceneSetup(entt::registry &registry);


/**
 * Runs simulation and prints its result
 */
void RunSimulation(entt::registry &registry);

int main(int argc, char *argv[]);

#endif