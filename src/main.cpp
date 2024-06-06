#include <iostream>
#include "../include/entt/entt.hpp"
#include "transform.hpp"
#include "balistics.hpp"
#include "vector3.hpp"
#include <cmath>
#include "sim_settings.hpp"
#include "sim_data.hpp"
#include <string.h>
#include <stdexcept>
#include "main.hpp"

#define SIM_STEP_LIMIT 10000
#define ALLOWED_TARGET_DEVIAION_RATIO 0.75

sim_settings settings;
sim_data data;
bool projectile_hit = false;

void UpdateScene(entt::registry &registry, int iteration)
{
    auto view = registry.view<Transform>();

    for (auto entity : view)
    {
        Transform &transform = view.get<Transform>(entity);
        transform.Update(settings.time_step, GRAVITY_ACCELERATION);

        if (strcmp(transform.GetName().c_str(), "bullet") == 0)
            printf("%s\n", transform.ToString().c_str());

        for (auto tested_entity : view)
        {
            if (entity == tested_entity)
                continue;

            Transform t = view.get<Transform>(tested_entity);
            float allowed_diviation = iteration * settings.time_step * ALLOWED_TARGET_DEVIAION_RATIO; // compensation for simulation error, should accound for bullet velocity as well
            if (transform.InHitRange(t.GetPosition(), allowed_diviation))
            {
                printf("hit!\n");
                projectile_hit = true;
            }
        }
    }
}

void ShowHelp()
{
    printf("Usage:\n");
    printf("\t./main --help \t\t prints help\n");
    printf("\t./main sx sy sz tx ty yz velocity time_step\n");
    printf("s[xyz] - shooter position\n");
    printf("t[xyz] - target posiiton\n");
    printf("velocity - initial projectile velocity\n");
    printf("time_step - how many seconds passes between each simulation step (usualy 0.1 or lower)\n");
    printf("NOTE: Y axis is pointing upwards\n");
}

sim_settings ParseUserInput(int argc, char *argv[])
{
    float shooter[3];
    float target[3];
    double projectile_velocity;
    float time_step;

    if (argc == 1)
    {
        fprintf(stdout, "use --help to see help\n");
        exit(0);
    }

    else if (argc == 2 && strcmp("--help", argv[1]) == 0)
    {
        ShowHelp();
        exit(0);
    }

    else if (argc == 9)
    {

        try
        {
            for (size_t i = 0; i < 3; i++)
                shooter[i] = std::stof(argv[1 + i]);
            for (size_t i = 0; i < 3; i++)
                target[i] = std::stof(argv[4 + i]);
            projectile_velocity = std::stod(argv[7]);
            time_step = std::stof(argv[8]);
        }
        catch (const std::invalid_argument &e)
        {
            fprintf(stderr, "Error ocured while parsing inputs. Cause: %s\n", e.what());
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Invalid number of args, see --help for help\n");
        exit(1);
    }

    Vector3 shooter_position(
        shooter[0],
        shooter[1],
        shooter[2]);

    Vector3 target_position(
        target[0],
        target[1],
        target[2]);

    sim_settings settings = {
        shooter_position,
        target_position,
        projectile_velocity,
        time_step,
        -1};

    return settings;
}

void SceneSetup(entt::registry &registry)
{
    Vector3 shooting_vector = Vector3::PitchYawToVector(data.elevation_angle, data.shooter_yaw);
    Vector3 projectile_velocity = shooting_vector * settings.projectile_velocity;

    // creating projection
    entt::entity projectile = registry.create();
    registry.emplace<Transform>(projectile, settings.shooter_position, projectile_velocity, "bullet", false);

    entt::entity target = registry.create();
    registry.emplace<Transform>(target, settings.target_position, Vector3(), "target", true);
}

void RunSimulation(entt::registry &registry)
{
    int sim_steps = 0;
    float compensated_step_limit = SIM_STEP_LIMIT * (settings.projectile_velocity / 2) / settings.time_step; // when running simulaiton at higher resolution, more steps are needed to reach the target

    while (!projectile_hit && sim_steps < compensated_step_limit)
    {
        UpdateScene(registry, sim_steps);
        sim_steps++;
    }

    if (projectile_hit)
    {
        printf("Shooter hit target at elevation angle of %f degrees\n", data.elevation_angle);
        exit(0);
    }
    else if (sim_steps == compensated_step_limit)
    {
        printf("ERROR: Shooter should hit the projectile at %f degrees but the simulation reached step limit, it may be because of inaccuracy caused by large time step. Please try running simulation again with lower time step\n", data.elevation_angle);
        exit(1);
    }
    else
    {
        fprintf(stderr, "Unknown error has occured\n");
        exit(1);
    }
}


int main(int argc, char *argv[])
{
    settings = ParseUserInput(argc, argv);
    entt::registry registry;

    Vector3 localized_target = settings.target_position - settings.shooter_position;

    float target_flat_distance = std::sqrt(std::pow(settings.target_position.x, 2) + std::pow(settings.target_position.z, 2)); // distance on flat plane
    float target_height = settings.target_position.y;

    data.elevation_angle = ProjectileElevationWithTarget(settings.projectile_velocity, target_flat_distance, target_height); // in degrees
    data.shooter_yaw = std::atan2(localized_target.z, localized_target.x) * 180 / M_PI;

    if (std::isnan(data.elevation_angle))
    {
        printf("Target is out of range\n");
        exit(0);
    }

    SceneSetup(registry);

    printf("Shooter should hit the target at elevation angle of %f degrees\n", data.elevation_angle);
    printf("Simulation ready, press any key to begin the simulation\n");
    getchar(); // wait for user input

    // running simulation
    RunSimulation(registry);

    return 0;
}