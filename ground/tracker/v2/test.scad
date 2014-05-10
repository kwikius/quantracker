include <slipring12Wire.scad>
include <radial_ball_bearing.scad>
include <gear.scad>
include <MCAD/materials.scad>
include <MCAD/metric_fastners.scad>
include <MCAD/nuts_and_bolts.scad>


module standard_hex_nut(size )
{
	thickness = METRIC_NUT_THICKNESS[size];
	overall_radius = (METRIC_NUT_AC_WIDTHS[size])/2;
   sin_c = sin(30) * overall_radius;
	cos_c = cos(30) * overall_radius;

   module hexagon(){
   polygon(points = [[cos_c, -sin_c], [0,-overall_radius],[-cos_c, -sin_c]
	,[-cos_c, sin_c], [0,overall_radius],[cos_c, sin_c]]);
	}
   linear_extrude( height = thickness){
		difference () {
			hexagon();
			circle(d = size, $fn = 10);
		}
	}
	
}

ballrace_inner_dia = 25;
ballrace_outer_dia = 37;
ballrace_thickness = 7;
gear_thickness = 6;

flange_thickness = 3;
ballrace_clearance = 0.5;

module bezel_screws(){
  num_screws = 6;
  screw_length = 18;
   for (i = [0 : 5]){
		rotate([0,0,i *(360.0 / 6)]){
			translate([ballrace_outer_dia/2 + 3.5,0,ballrace_thickness/2 + 3.1]){
    			
				rotate([0,180,0]){
					csk_bolt(3,screw_length);
				}
            translate([0,0,-screw_length +2.5]){
					standard_hex_nut(3);
				}
			}
	   }
	}
}


translate([0,0,9]){
	# slipring();
}
//ballrace
#color(Stainless){
	translate([0,0,0])
	{
		radial_ball_bearing(
			ballrace_outer_dia,
			ballrace_inner_dia,
			ballrace_thickness);
	}
}

//main gear
translate([0,0,10]){
   difference(){
		gear(number_of_teeth=20,circular_pitch=300, clearance = 0.1, rim_thickness = gear_thickness);
	   translate([0,0,-5]){
			cylinder(h= 25,d=12.6, center = true);
		}
   }
}


module pinion()
{

   rotate([0,0,18]){
      
   		difference(){
			gear(number_of_teeth=10,circular_pitch=300,clearance = 0, rim_thickness = gear_thickness);
			cylinder(h= 15,d=3, center = true, $fn=10);
   		}
     
   }

}

// motor 

module motor(){
  translate([0,0,20]){
   rotate([180,0,0]){
   union(){
	   intersection(){
	  	   cylinder (h=17,d=12 ,center = true);
	      translate([0,0,-1]){
		 		cube([20,10,20],center = true);
		   }
      }
      translate([0,0,17/2]){
			cube([12,10,9],center = true);
	   }
      translate([0,0,17]){
         difference(){
				cylinder( h = 10, d=3, center = true);
				translate([2.4 ,0,-1]){
					cube ([3,5,14],center = true);
				}
			}
		}
   } // union
  }// translate
  } // rotate
}

translate([25.3,0,10]){
	motor();
	pinion();
}
 

module bezel()
{
   bottom_dia = ballrace_outer_dia + ballrace_clearance +19;
   top_dia = bottom_dia - 2;
   thickness_above_ballrace = 3;
   height = ballrace_thickness/2 + thickness_above_ballrace;
   echo("bezel dia = " , bottom_dia);
   
	color([0.25,0.5,1]){

	difference()
	{
			translate([0,0,0]){
				cylinder(h= height, d1 = bottom_dia, d2 = top_dia);
			}
            union(){
              
					translate([0,0,-1]){
						cylinder(h= height + 2,
						d=ballrace_outer_dia - (ballrace_outer_dia - ballrace_inner_dia)/3 );
					}
               translate([0,0,0])
	            cylinder(h= ballrace_thickness ,d = ballrace_outer_dia + ballrace_clearance, center = true);
				
			}
		}
	}
}
module upper_base()
{
	thickness = 3;
   bezel_bottom_dia = ballrace_outer_dia + ballrace_clearance +19;
   bezel_top_dia = bezel_bottom_dia - 2;
   thickness_above_ballrace = 2;
   bezel_height = ballrace_thickness/2 + thickness_above_ballrace;
	difference(){
      union(){
			translate([-50,-50,-thickness]){
				cube(size = [100,100,thickness]);
			}
			translate([0,0,-thickness + 0.01]){
            rotate([180,0,0]){
						translate([0,0,0]){
							cylinder(h= bezel_height, d1 = bezel_bottom_dia, d2 = bezel_top_dia);
					}
			   }
			}
		}
      union(){
			translate([0,0,0]){ // ballrace cutout
		 		cylinder(h= ballrace_thickness ,d = ballrace_outer_dia + ballrace_clearance, center = true);
			}
         translate([0,0,-(thickness + bezel_height +1)]){ // hole 
				cylinder(h = thickness + bezel_height + 2, d=ballrace_outer_dia - (ballrace_outer_dia - ballrace_inner_dia)/3);
			}
		}
	}
}

// show section

difference(){

	union(){
		bezel();
      upper_base();
	 	
	}

   translate([0,-100,-50]){
   		cube(100,100,100); 
	}

}

 bezel_screws();



 











