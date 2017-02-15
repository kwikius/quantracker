include <slipring12Wire.scad>
include <radial_ball_bearing.scad>
include <gear.scad>
include <MCAD/materials.scad>
include <MCAD/metric_fastners.scad>
include <MCAD/nuts_and_bolts.scad>

/*
   parts
   main_gear_outer_bearing  1
   outer_bearing_holder_p2
   circlip
   inner_bearing_holder
   inner_bearing_golder_p2
   inner_bearing_bracket
   motor_mount
   
   
*/
// smb part S6707-2RS
ballrace_inner_dia = 35;
ballrace_outer_dia = 44;
ballrace_thickness = 5;
gear_thickness = 6;
// 35x44x5
// next up 40x52x7 available on ebay

flange_thickness = 3;
ballrace_clearance = 0.1;

show_ballrace = true;
show_inner_bearing_holder = true;
show_inner_bearing_bracket = false;
show_inner_bearing_holder_p2 = true;
show_outer_bearing_holder = true;
show_circlip = true;
show_outer_bearing_holder_p2 = true;
show_motor = true;
show_main_gear = true;
show_pinion = true;
show_pinion_as_cylinder = false;
show_pinion_as_gear = true;
show_section = true;
show_motor_mount = false;
show_slipring = true;

do_print_transform = false;// false;

module slipring1(){
     translate([0,0,0]){
    rotate([180,0,0]){
       
            slipring();
        }
    }
}

module main_gear(){
   shroud_height = 2;
   outer_dia = ballrace_outer_dia + 8.2;
   wall_thickness = 2;
   union(){
  	   difference(){
			cylinder(h= gear_thickness,d=outer_dia);
	  		translate([0,0,-1]){
				gear(number_of_teeth=25,circular_pitch=300,
					clearance = -0.1, rim_thickness = gear_thickness +2);
		  	}
	   }
      translate([0,0,-shroud_height]){
      	difference(){
	         cylinder(h=shroud_height,d=outer_dia);
		  		translate([0,0,-1]){
				  cylinder(h= shroud_height + 2,d1 = (outer_dia ) - (wall_thickness*2) +.5, d2 = (outer_dia ) - (wall_thickness*2)  ); 
			  	}
			}
		}
   }
}



module pinion(){
   // should get this from gear
   outer_radius = 9.16667;
   rotate([0,0,0]){
      difference(){
         union(){
            if (show_pinion_as_cylinder){
               translate([0,0,0]){
                  cylinder(r = outer_radius, h = gear_thickness);
               }
            }
            if (show_pinion_as_gear){
               translate([0,0,0]){
                  gear(number_of_teeth=7,circular_pitch=300,clearance = 0.0, rim_thickness = gear_thickness);
               }
            }
         }
         intersection(){
         cylinder(h= 15,d=3.1, center = true, $fn=20);
            translate([0.4,0,0]){
               cube([3,4,16], center = true);
            }
         }
      }
   }
}

module motor(){
   translate([0,0,25]){
      rotate([180,0,90]){
         union(){
            intersection(){
               cylinder (h=17,d=12 ,center = true);
               cube([20,10,17],center = true);
            }
            translate([0,0, (17 + 9) / 2]){
               cube([12,10,9],center = true);
            }
            translate([0,0,(17 + 9 + 10)/2]){
               difference(){
                  cylinder( h = 10, d=3, center = true);
                  translate([2.4 ,0,-1]){
                     cube ([3,5,14],center = true);
                  }
               }
            }
         } // union
      } // rotate
   } // translate
}

module motor_mount()
{

    translate([0,0,25]){
      rotate([180,0,0]){
         difference(){
           // union(){
               motor_mount_blank();
				//	brackets();
				//}
            motor_block();
         }
      } // rotate
   } // translate

   module motor_mount_blank(){
      thickness = 2;
      slot_width = 3;
      union(){
         difference(){
            intersection(){
               cylinder (h=16.5,d=12 + thickness * 2 ,center = true);
               translate([0,0,0]){
                  cube([20 + thickness *2,10 + thickness *2,20],center = true);
               }
            }
            translate([-7,0,-5]){
               cube([20,slot_width,30],center = true);
            }
         }//difference
         // screw mounts
         translate([-9,1+slot_width/2,0]){
            cube([5,2,16.5], center = true);
         }
         translate([-9,-1-slot_width/2,0]){
            cube([5,2,16.5],center = true);
         }
      }//union
   }

   module motor_block(){
      clearance = 0.2;
      intersection(){
         cylinder (h=17,d=12 + clearance *2 ,center = true);
         translate([0,0,-1]){
            cube([20 + clearance *2,10 + clearance *2,20],center = true);
         }
      }
   }
}

module inner_bearing_holder(){
   thickness = 2;
   bearer_thickness =  (ballrace_outer_dia - ballrace_inner_dia)/6;
   vertical_clearance = 0.1;
   horizontal_clearance = 0.1;
   ibh_flange_thickness = 6;
   x0 = ballrace_inner_dia/2 + bearer_thickness - horizontal_clearance;
   x1 = ballrace_inner_dia/2 - (ballrace_clearance);
   
   x2 = ballrace_inner_dia/2 - (ballrace_clearance + thickness) - horizontal_clearance;
   x3 = ballrace_inner_dia/2 - (ballrace_clearance + 2* thickness);
   x4 = x3 - ibh_flange_thickness;
   y0 = - (ballrace_thickness / 2 + thickness);
   y1 = -(ballrace_thickness / 2 + vertical_clearance);
   y2 = ballrace_thickness / 2 +thickness;
   rotate_extrude(convexity = 10){
      polygon (points = [
      [x0, y0 ],[x0,y1], [x1,y1],
      [x1,0],[x2,0],[x2,y2], [x3,y2], [x3,y1],[x4,y1],[x4,y0]
      ]);
   }
}

module inner_bearing_holder_p2(){
  thickness = 2;
  bearer_thickness =  (ballrace_outer_dia - ballrace_inner_dia)/6;
  vertical_clearance1 = 0.2;
  vertical_clearance2 = 0.2;
  horizontal_clearance = 0.2;
  rotate_extrude(convexity = 10){
 	 polygon (points = [
       [ballrace_inner_dia/2 - ballrace_clearance,vertical_clearance1],
       [ballrace_inner_dia/2 - ballrace_clearance,ballrace_thickness / 2 + vertical_clearance2],
       [ballrace_inner_dia/2 + bearer_thickness - .2, ballrace_thickness / 2 + vertical_clearance2],
       [ballrace_inner_dia/2 + bearer_thickness - .2, ballrace_thickness / 2 + thickness],
       [ballrace_inner_dia/2 -(ballrace_clearance + thickness)+ horizontal_clearance,ballrace_thickness / 2 +thickness ],
		[ballrace_inner_dia/2 -(ballrace_clearance + thickness)+ horizontal_clearance,vertical_clearance1]
	]);
  }
}

module inner_bearing_bracket(){
   height = 37;
   bottom_width = 20;
   top_width = 10;
   bthickness = 3;
   // upstand
   translate([-3,0,4.5]){
      rotate([90,0,90]){
         linear_extrude(height = bthickness){
            polygon(points = [
               [-bottom_width/2,0],[bottom_width/2,0],
               [top_width / 2, height], [-top_width / 2, height]
            ]);
         }
      }
   }
   // buttress
   translate([-12,bthickness/2,5]){
      rotate([90,0,0]){
         linear_extrude( height = bthickness){

            polygon(points = [
               [-2,0] ,[10,0] , [10,36]
            ]);
         }
      }
   }
   translate([-0.5,1.5,18]){
   // motor bracket
   //   cube([6,3,16],center = true);
      rotate([90,0,0]){
       //  color([1,.5,.5]){
         linear_extrude(height = 3){
            polygon(points = [[0,0],[7,7],[7,23],[0,23]]);
         }
        // }
      }
   }
    // base of bracket
   translate([0,0,4.5]){
      difference(){
         cylinder(h=3, d = 33, $fn= 20);
         translate([12.5,0,-.25]){
            //cube([20,40,8],center = true);
              cylinder(h= 8,d=20.5);
         }
      }
   }
} //module

module outer_bearing_holder()
{
	thickness = 2;
	vertical_clearance = 0.1;
   horizontal_clearance = 0.1;
   height_above_bearing_top = 4;
   depth_below_bearing_bottom = 3;
   bearer_thickness =  (ballrace_outer_dia - ballrace_inner_dia)/6;
   circlip_thickness = 2;
 
   y0 = -ballrace_thickness / 2 - depth_below_bearing_bottom;
   y1 = -ballrace_thickness / 2;
   y2 = ballrace_thickness / 2 + vertical_clearance;
   y3 = ballrace_thickness / 2 + height_above_bearing_top ;
   y3a = y3 - 2;

   x0 = ballrace_outer_dia/2 + ballrace_clearance + 2 * thickness;

	x1 = ballrace_outer_dia/2 + ballrace_clearance + thickness;
   x2 = ballrace_outer_dia/2 + ballrace_clearance;
   x3 = ballrace_outer_dia / 2 - bearer_thickness;
   
   rotate_extrude(convexity = 10){
		polygon ( points = [
		//	[x0,y0],[x0,y3], /* [x0a,y3] , [x0a,y3a],  [x0b,y3a], [x0b,y3], */ [x3,y3],[x3,y2],[x2,y2],[x2,y1],[x1,y1],[x1,y0]
      	[x0,y0],[x0,y3], [x2 +.3,y3],[x3,y2],[x2,y2],[x2,y1],[x1,y1],[x1,y0]
		]);
   }
}
module outer_bearing_holder_p2()
{
  thickness = 2;
	vertical_clearance = 0.1;
   horizontal_clearance = 0.2;
   height_above_bearing_top = 3;
   depth_below_bearing_bottom = 10;
   bearer_thickness =  (ballrace_outer_dia - ballrace_inner_dia)/6;
   circlip_thickness = 2;

   y0 = -ballrace_thickness / 2 - depth_below_bearing_bottom;
   y1 = -ballrace_thickness / 2;
   y2 = ballrace_thickness / 2 + vertical_clearance;
   y3 = ballrace_thickness / 2 + height_above_bearing_top ;

   x0 = ballrace_outer_dia/2 + ballrace_clearance + 2 * thickness;
	x1 = ballrace_outer_dia/2 + ballrace_clearance + thickness;
   x2 = ballrace_outer_dia/2 + ballrace_clearance;
   x3 = ballrace_outer_dia / 2 - bearer_thickness;

   rotate_extrude(convexity = 10){
		polygon ( points = [
		[x1-(horizontal_clearance + ballrace_clearance), y1 -circlip_thickness],
		[x2 + horizontal_clearance,y1 -circlip_thickness],
		[x2 + horizontal_clearance,y0],
		[x1-(horizontal_clearance + ballrace_clearance),y0]
	]);
   }
  }

module circlip()
{
	difference(){
		cylinder(h = 1.7, d = ballrace_outer_dia + 4, center = true);
	union()
		{
		cylinder(h = 3, d = ballrace_outer_dia - 2, center = true);
		 translate([ballrace_outer_dia/2,0,0]){
			rotate([0,0,60]){
				cube([20,2,3],center = true);
			}
		 }
		}
	}
}
module main_object()
{

   union(){
      translate([0,0,7.5]){

			if (show_circlip){
				translate([0,0,-11]){
					circlip();
			   }
         }
         
         if(show_slipring){
            slipring1(); 
         }

         if ( show_main_gear){
            main_gear();
         }

         translate([14.5,0,0]){
            if (show_motor){
               motor();
            } 
            if (show_pinion){
               pinion();
            }
            if ( show_motor_mount){
               color([0,1,1,0.5]){
                  motor_mount();
               }
            }
			}
      }

      if (show_ballrace){
         color(Stainless){
            translate([0,0,0]){
               radial_ball_bearing(
                  ballrace_outer_dia,
                  ballrace_inner_dia,
                  ballrace_thickness
               );
            }
         }
      }

      if ( show_inner_bearing_holder){
         rotate([180,0,0]){
            color([.2,.5,.7]){
               inner_bearing_holder();
            }
         }
      }
		if (show_inner_bearing_bracket){

			 inner_bearing_bracket();
		}
      if (show_inner_bearing_holder_p2){
         rotate([180,0,0]){
            color([.7,.5,.2]){
               inner_bearing_holder_p2();
            }
         }
      }
      
      if (show_outer_bearing_holder){
         color([.4,.8,.6]){
            outer_bearing_holder();
         }
      }

      if (show_outer_bearing_holder_p2){
         color([.4,.8,.6]){
            outer_bearing_holder_p2();
         }
      }
   }// union
}
// show section
if ( do_print_transform){
   if ( show_outer_bearing_holder){
      translate([0,0,13.5]){
         rotate([180,0,0]){
          translate([0,0,7.5]){
          main_gear();
          }
          outer_bearing_holder();
         }
      }
   }else{
      if (show_pinion){
         pinion();
      }else {
         if (show_outer_bearing_holder_p2){
            translate([0,0,12.5]){
               outer_bearing_holder_p2();
            }
         }else{
            if(show_inner_bearing_holder){
               translate([0,0,4.5]){
                  inner_bearing_holder();
                 // inner_bearing_bracket();
               }
            }else{

					if(show_inner_bearing_holder_p2){
                 rotate([180,0,0]){
                   translate([0,0,-4.5]){
      
							inner_bearing_holder_p2();
						}
                  }
					}
               else{
                  if (show_circlip){
                     translate([0,0,.9]){
                        circlip();
                     }
                  }else {
                    
                     if(show_inner_bearing_bracket){
                        translate([0,0,-4.5]){
                           inner_bearing_bracket();
                        }
                     }else{
                        if (show_motor_mount){
                           translate([0,0,-16.8]){
                              motor_mount();
                           }
                        }else {
                           if (show_ballrace){
                                /*
                                  radial_ball_bearing(
                                    ballrace_outer_dia,
                                    ballrace_inner_dia,
                                    ballrace_thickness
                                 );
                              
                                    */
                              translate([0,0,ballrace_thickness/2]){
                                 difference(){
                                    cylinder(h = ballrace_thickness,d = ballrace_outer_dia, $fn = 100, center = true);
                                    cylinder(h = ballrace_thickness +1,d = ballrace_inner_dia, $fn = 100, center = true);
                                 }
                              }
                           }
                        }
                     }
                  }
               }
				}
         }
      }
   }
}
else{
   if( show_section){
      difference(){
      
         main_object();
      
         translate([0,-100,-50]){
               cube(100,100,100); 
         }
      }
   }else{
      main_object();
   }
}
