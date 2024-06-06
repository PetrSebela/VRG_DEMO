#include "simulation.hpp"
#include "balistics.hpp"
#include "transform.hpp"
#include "sim_data.hpp"
#include "sim_settings.hpp"
#include "terminal_colors.hpp"
#include <string.h>
#include <cmath>

Simulation::Simulation() {}
Simulation::~Simulation() {}

Simulation::Simulation(sim_settings settings, sim_data data)
{
    this->settings = settings;
    this->data = data;

    if (!ValidInput())
        return;

    SceneSetup();
}

void Simulation::OnSimulationSucces()
{
    printf("%sSUCCES:%s Shooter hit target at elevation angle of %f degrees with error of %f meters\n", OKGREEN_COLOR, END_COLOR, data.elevation_angle, last_distance);
    simulation_running = false;
    simulation_succes = true;
}

void Simulation::OnSimulationFailed()
{
    printf("%sFAIL:%s Shooter should have hit the target at elevation of %f degrees, but instead missed the target by %f meters. \n", FAIL_COLOR, END_COLOR, data.elevation_angle, last_distance);
    printf("%sThis may be because of inaccuracy caused by large time step. Please try running simulation again with lower time step%s\n", WARNING_COLOR, END_COLOR);
    simulation_running = false;
    simulation_succes = false;
}

void Simulation::UpdateScene()
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

            float allowed_diviation = transform.GetTravelDistance() * ALLOWED_TARGET_DIVIATION_PER_METER; // compensation for simulation error, should account for bullet velocity as well
            float current_distance = (t.GetPosition() - transform.GetPosition()).GetMagnitude();

            if (current_distance <= allowed_diviation)
            {
                printf("Projectile hit the target!\n");
                OnSimulationSucces();
            }
            else if (current_distance > last_distance)
            {
                printf("%sDEBUG:%s Projectile missed the target by %f meters. Allowed diviation was %f meters\n", OKCYAN_COLOR, END_COLOR, current_distance, allowed_diviation);
                last_distance = current_distance;
                OnSimulationFailed();
            }
            last_distance = current_distance;
        }
    }
}

bool Simulation::Run()
{
    if (!simulation_ready)
        return false;

    while (simulation_running)
        UpdateScene();

    return simulation_succes;
}

bool Simulation::IsReady()
{
    return simulation_ready;
}

bool Simulation::UpdateSettings(sim_settings settings, sim_data data)
{
    this->settings = settings;
    this->data = data;

    if (!ValidInput())
        return false;

    SceneSetup();
    return true;
}

bool Simulation::ValidInput()
{
    this->simulation_ready = settings.target_position.IsValid() &&
                             settings.shooter_position.IsValid() &&
                             !std::isnan(settings.projectile_velocity) &&
                             !std::isnan(settings.time_step) &&
                             !std::isnan(data.elevation_angle) &&
                             !std::isnan(data.shooter_yaw);
    return this->simulation_ready;
}

void Simulation::SceneSetup()
{
    // clearing all present entities (if simulation is rerun)
    auto all = registry.view<Transform>();
    registry.destroy(all.begin(), all.end());

    Vector3 shooting_vector = Vector3::PitchYawToVector(data.elevation_angle, data.shooter_yaw);
    Vector3 projectile_velocity = shooting_vector * settings.projectile_velocity;

    last_distance = (settings.target_position - settings.shooter_position).GetMagnitude();

    // creating projection
    entt::entity projectile = registry.create();
    registry.emplace<Transform>(projectile, settings.shooter_position, projectile_velocity, "bullet", false);

    entt::entity target = registry.create();
    registry.emplace<Transform>(target, settings.target_position, Vector3(), "target", true);
}