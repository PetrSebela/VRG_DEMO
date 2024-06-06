#ifndef _SIMULATION_H
#define _SIMULATION_H
#include "../include/entt/entt.hpp"
#include "sim_data.hpp"
#include "sim_settings.hpp"

#define ALLOWED_TARGET_DIVIATION_PER_METER 0.0025

class Simulation
{
private:
    bool simulation_running = true;
    bool simulation_ready = false;
    float last_distance;
    entt::registry registry;
    sim_settings settings;
    sim_data data;
    bool simulation_succes;

    // end simulation and prints the result
    void OnSimulationSucces();

    // end simulation and prints the reason why it failed
    void OnSimulationFailed();

    // creates and registers all necesary objects
    void SceneSetup();

    // checks if all settings and data are valid
    bool ValidInput();

    /**
     * Updates all entities with transform inside provided registry and checks if any entities colided
     * Compensates for simulation error
     * @param registry reference to entt registry
     */
    void UpdateScene();

public:
    Simulation(sim_settings settings, sim_data data);
    Simulation();
    ~Simulation();

    // launches the simulation and return true if simulation was succes, false otherwise
    bool Run();
    bool IsReady();
    bool UpdateSettings(sim_settings settings, sim_data data);
};

#endif