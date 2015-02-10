
#include <quan/uav/osd/api.hpp>
#include "on_draw.hpp"

void quan::uav::osd::on_draw()
{
     draw_compass();
     draw_artificial_horizon();
     draw_home();
     draw_altitude();
}