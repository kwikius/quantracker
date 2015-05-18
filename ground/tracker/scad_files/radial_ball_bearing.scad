include <MCAD/materials.scad>
include <MCAD/math.scad>
include <slipring12Wire.scad>

module radial_ball_bearing(
	od,bore,thickness
   
){
  ring_width =( od - bore) /3;
  ball_dia = (od-bore)/4;
  groove_dia = (od + bore)/2;
  groove_circum = PI * groove_dia;
  num_balls = floor( groove_circum / ( 2 * ball_dia) );
  translate([0,0,-thickness/2]){
	  Ring(od,od-ring_width,thickness);
	  Ring(bore+ring_width,bore,thickness);
  }
  for(i=[0:num_balls -1]){
		rotate((360 / num_balls) * i){
			translate([ groove_dia / 2,0,0]){
				sphere(d=ball_dia, $fn = 10);
			}
		}
	}

  module Ring(outerD,innerD,thickness){
 epsilon = 0.1;
	difference(){
		cylinder(h = thickness, d=outerD , $fn= 20) ;
		translate([0,0,-epsilon]){
	   	  cylinder(h = thickness + 2 * epsilon, d=innerD, $fn= 20);
	   }
   }
}
}







