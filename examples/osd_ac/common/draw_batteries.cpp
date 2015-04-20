
#include <cstdio>
#include <quan/constrain.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "osd.hpp"
#include "symbology.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

void draw_batteries()
{

	char buf [30];
	pxp_type pos;
	font_ptr font = get_font(FontID::OSD_Charset);


#if(0)
	  //want to draw voltage inside battery
	   // voltage up to 25 V with 1 decimal
	   // so 12.2V == 5 chars
	   // get font, get the size of
	   // size of box = ( 5 * x font.y + border * 2)

	if ( osd_show_aircraft_battery_voltage() == true)
	{
	   pos = get_osd_aircraft_battery_voltage_position();

	   if ( font)
	   {


		  size_type font_size = get_size(font);
		  size_type const border = {2,1};
		  pxp_type pt_array[]= {
				{-border.x,-border.y}
				,{font_size.x * 5 + border.x +1,-border.y}
				,{font_size.x * 5 + border.x +1,- border.y + (font_size.y + 2 * border.y)/3}
				,{font_size.x * 5 + border.x +5,- border.y +(font_size.y + 2 * border.y)/3}
				,{font_size.x * 5 + border.x +5,(font_size.y + 2 * border.y +1) * 2/3}
				,{font_size.x * 5 + border.x +1,(font_size.y + 2 * border.y +1)*2/3}
				,{font_size.x * 5 + border.x +1, border.y + (font_size.y + 2 * border.y)}
				,{- border.x, border.y + (font_size.y + 2 * border.y)}
		   };

		   uint32_t num_points = (sizeof(pt_array) / sizeof(pxp_type));
		   enum dirs{ inside_left, inside_right, inside_up,inside_down};
		   dirs const dirs_array[]  ={
				inside_up,inside_left,inside_up,inside_left,inside_down,inside_left,inside_down, inside_right
		   };
		   size_type dirs_move[] = {{-1,0},{1,0},{0,1},{0,-1}};
		   for ( uint32_t i = 1; i < num_points; ++i){
			 for( int32_t j = -1;j < 2; ++j){
				size_type const move = dirs_move[dirs_array[i-1]] * j;
				colour_type colour = (j == 0)?colour_type::white: colour_type::black;
				draw_line(pos + move + pt_array[i-1], pos + move + pt_array[i],colour );
			 }
		   }
		   for( int32_t j = -1;j < 2; ++j){
			 size_type const move = dirs_move[dirs_array[num_points-1]] * j;
			 colour_type colour = (j == 0)?colour_type::white: colour_type::black;
			 draw_line(pos + move + pt_array[num_points -1], pos + move + pt_array[0],colour );
		   }

		   char buf [30];
		   quan::voltage_<double>::V cur_battery_voltage = get_battery_voltage(0);
		   sprintf(buf,"%4.1fv",cur_battery_voltage.numeric_value());
		   draw_text(buf,pos + pxp_type{1,3},font);
	   }
	}
#endif


	if ( osd_show_aircraft_battery_voltage() == true)
	{
	   pos = get_osd_aircraft_battery_voltage_position();

	   if ( font)
	   {

		 quan::voltage_<double>::V cur_battery_voltage = get_battery_voltage(0);
		 sprintf(buf,"%4.1fv",cur_battery_voltage.numeric_value());
		 draw_text(buf,pos,font);
	   }
	}

	if ( osd_show_aircraft_battery_current() == true)
	{
	   pos = get_osd_aircraft_battery_current_position();

	   if ( font )
	   {
		 quan::current_<double>::A cur_battery_current = get_battery_current(0);
		 sprintf(buf,"%4.1fa",cur_battery_current.numeric_value());
		 draw_text(buf,pos,font);
	   }
	}

	if ( osd_show_aircraft_battery_remaining() == true)
	{
	   pos = get_osd_aircraft_battery_remaining_position();

	   if ( font )
	   {
		 double display_battery_remaining;
		 display_battery_remaining = get_battery_remaining(0);

		 quan::constrain<double> (
					  display_battery_remaining
					, 0.0
					, 100.0);

		 sprintf(buf,"%3.0f%%",display_battery_remaining);
		 draw_text(buf,pos,font);
	   }
	}

}
