#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <sys/wait.h>

#include "../src/balistics.hpp"
#include "../src/vector3.hpp"
#include "../src/transform.hpp"
#include "../src/simulation.hpp"

TEST_CASE("Balistics", "[balistics]")
{
    // double checking with caluclator
    REQUIRE(ProjectileElevationWithTarget(1200, 1500, 0) == Catch::Approx(0.2927).epsilon(0.001));  // can hit the point, small error is acceptable for comparing floats
    REQUIRE(ProjectileElevationWithTarget(1200, 35000, 0) == Catch::Approx(6.8948).epsilon(0.001)); // can hit the point, small error is acceptable for comparing floats

    // impossible cases
    REQUIRE(std::isnan(ProjectileElevationWithTarget(10, 100, 0)));  // target is too far
    REQUIRE(std::isnan(ProjectileElevationWithTarget(-10, 100, 0))); // shooting from wrong end of the gun
    REQUIRE(std::isnan(ProjectileElevationWithTarget(0, 100, 0)));   // cannot hit target with stationary projectile
    REQUIRE(std::isnan(ProjectileElevationWithTarget(350, -1, 0)));  // target is behind ( technicaly ok, but who would shoot backwards )
    REQUIRE(std::isnan(ProjectileElevationWithTarget(-350, -1, 0))); // shooting itself with stationary projectile?

    // really fast projectiles
    REQUIRE(ProjectileElevationWithTarget(10000000.0, 1.0, 1.0) == Catch::Approx(45).epsilon(0.001));
    REQUIRE(std::isnan(ProjectileElevationWithTarget(99999999.0, 1.0, -1.0)));
}

TEST_CASE("Transform", "[transform]")
{
    Transform test(Vector3(), Vector3(0, 1, 0), "test");
    REQUIRE(test.InHitRange(Vector3(), 0));
    REQUIRE(test.InHitRange(Vector3(1, 0, 0), 1.2));
    REQUIRE_FALSE(test.InHitRange(Vector3(1, 0, 0), 0.75));
    test.Update(1, 0);
    REQUIRE(test.InHitRange(Vector3(0, 1, 0), 0));
}

TEST_CASE("Vector3", "[vector3]")
{
    REQUIRE(Vector3(0, 0, 0).GetMagnitude() == 0);
    REQUIRE(Vector3(1, 0, 0).GetMagnitude() == 1);
    REQUIRE(Vector3(1, 1, 0).GetMagnitude() == Catch::Approx(std::sqrt(2)).epsilon(0.01));
    REQUIRE(Vector3(1, 1, 0).IsValid());
    float x = 0.0 / 0.0; // results to NaN
    REQUIRE_FALSE(Vector3(x, 0, 0).IsValid());
}

sim_data GetTestData(sim_settings settings)
{
    sim_data data;
    Vector3 localized_target = settings.target_position - settings.shooter_position;

    double target_flat_distance = std::sqrt(std::pow(localized_target.x, 2) + std::pow(localized_target.z, 2)); // distance on flat plane
    double target_height = settings.target_position.y - settings.shooter_position.y;

    data.elevation_angle = ProjectileElevationWithTarget(settings.projectile_velocity, target_flat_distance, target_height); // in degrees
    data.shooter_yaw = std::atan2(localized_target.z, localized_target.x) * 180 / M_PI;

    return data;
}

TEST_CASE("Simulation", "[simulation]")
{
    // invalid input data
    sim_settings settings = {Vector3(0.0 / 0.0, 0, 0), Vector3(100, 0, 0), 0.1, 0.001, -1};
    sim_data data = GetTestData(settings);
    Simulation simulation1(settings, data);
    REQUIRE_FALSE(simulation1.IsReady());
    REQUIRE_FALSE(simulation1.Run());

    // failed simulation
    settings = {Vector3(0, 0, 0), Vector3(100, 0, 0), 0.1, 0.001, -1};
    data = GetTestData(settings);
    Simulation simulation2(settings, data);
    REQUIRE_FALSE(simulation2.IsReady());
    REQUIRE_FALSE(simulation2.Run());
  
    // succesful simulation on mega long range
    settings = {Vector3(0, 0, 0), Vector3(500000, 0, 0), 12000, 0.0005, -1};
    data = GetTestData(settings);
    Simulation simulation3(settings, data);
    REQUIRE(simulation3.IsReady());
    REQUIRE(simulation3.Run());

    // succesful simulation on long range
    settings = {Vector3(0, 0, 0), Vector3(1500, 0, 0), 1200, 0.001, -1};
    data = GetTestData(settings);
    Simulation simulation4(settings, data);
    REQUIRE(simulation4.IsReady());
    REQUIRE(simulation4.Run());

    // succesful simulation on close range
    settings = {Vector3(0, 0, 0), Vector3(10, 0, 0), 15, 0.001, -1};
    data = GetTestData(settings);
    Simulation simulation5(settings, data);
    REQUIRE(simulation5.IsReady());
    REQUIRE(simulation5.Run());

    // succesful simulation on medium range
    settings = {Vector3(-25, 10, -25), Vector3(35, -10, -10), 250, 0.001, -1};
    data = GetTestData(settings);
    Simulation simulation6(settings, data);
    REQUIRE(simulation6.IsReady());
    REQUIRE(simulation6.Run());

    // succesful simulation on medium range, with shooter offset
    settings = {Vector3(-25, -6.41, 25), Vector3(100, 10, 10), 450, 0.0005, -1};
    data = GetTestData(settings);
    Simulation simulation7(settings, data);
    REQUIRE(simulation7.IsReady());
    REQUIRE(simulation7.Run());

    // succesful simulation on medium range, with shooter offset
    settings = {Vector3(-25, -6.41, 25), Vector3(-100, -3, -430), 450, 0.0005, -1};
    data = GetTestData(settings);
    Simulation simulation8(settings, data);
    REQUIRE(simulation8.IsReady());
    REQUIRE(simulation8.Run());    
}