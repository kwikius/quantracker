include <gear_shape.scad>
/*
test to reduce the size of the part at z = 0 to allow for 
spreading of the plastic on the first layer
*/
linear_extrude( height= 10,scale = 1, convexity = 10)
   {
      gear_shape(circular_pitch = 500);
   }

translate([0,0,10]){
linear_extrude( height= 6,scale = 0.8, convexity = 10)
{
   gear_shape(circular_pitch = 500);
}
}



