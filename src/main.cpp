#include <iostream>
#include "balistics.hpp"
#include "sim_settings.hpp"
#include "sim_data.hpp"
#include <string.h>
#include <stdexcept>
#include "main.hpp"
#include "terminal_colors.hpp"
#include "simulation.hpp"

void ShowHelp()
{
    printf("Usage:\n");
    printf("\t./sim --help \t\t prints help\n");
    printf("\t./sim sx sy sz tx ty yz velocity time_step\n");
    printf("s[xyz] - shooter position\n");
    printf("t[xyz] - target position\n");
    printf("velocity - initial projectile velocity (0 - 10,000,00) mps\n");
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

    if (projectile_velocity <= 0 || projectile_velocity > 1000000)
    {
        fprintf(stderr, "Projectile velocity outside allowed range <0, 1.000.000)\n");
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

sim_data GetSimData(sim_settings settings)
{
    sim_data data;
    Vector3 localized_target = settings.target_position - settings.shooter_position;
  
    double target_flat_distance = std::sqrt(std::pow(localized_target.x, 2) + std::pow(localized_target.z, 2)); // distance on flat plane
    double target_height = settings.target_position.y - settings.shooter_position.y;

    data.elevation_angle = ProjectileElevationWithTarget(settings.projectile_velocity, target_flat_distance, target_height); // in degrees
    data.shooter_yaw = std::atan2(localized_target.z, localized_target.x) * 180 / M_PI;

    if (std::isnan(data.elevation_angle))
    {
        printf("%sFAIL:%s Target is out of range\n", FAIL_COLOR, END_COLOR);
        exit(0);
    }
    return data;
}

int main(int argc, char *argv[])
{
    sim_settings settings = ParseUserInput(argc, argv);
    sim_data data = GetSimData(settings);   

    Simulation simulation(settings, data);

    printf("Shooter should hit the target at elevation angle of %f degrees\n", data.elevation_angle);
    printf("Simulation ready, press any key to begin the simulation\n");
    getchar(); // wait for user input

    simulation.Run();

    return 0;
}