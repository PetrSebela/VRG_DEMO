#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <sys/wait.h>

#include "../src/balistics.hpp"
#include "../src/vector3.hpp"
#include "../src/transform.hpp"

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
    REQUIRE(ProjectileElevationWithTarget(100000.0, 1.0, 1.0) == Catch::Approx(45));
    REQUIRE(ProjectileElevationWithTarget(100000.0, 1.0, -1.0) == Catch::Approx(-45));
}



TEST_CASE("Transform", "[transform]")
{
    Transform test(Vector3(),Vector3(0,1,0),"test");

    REQUIRE(test.InHitRange(Vector3(),0));
    REQUIRE(test.InHitRange(Vector3(1,0,0),1.2));
    REQUIRE_FALSE(test.InHitRange(Vector3(1,0,0),0.75));

    test.Update(1,0);
    REQUIRE(test.InHitRange(Vector3(0,1,0),0));
}

TEST_CASE("Vector3", "[vector3]")
{
    REQUIRE(Vector3(0,0,0).GetMagnitude() == 0);
    REQUIRE(Vector3(1,0,0).GetMagnitude() == 1);
    REQUIRE(Vector3(1,1,0).GetMagnitude() == Catch::Approx(std::sqrt(2)).epsilon(0.01));
}