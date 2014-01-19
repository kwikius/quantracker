include <gear.scad>

gear_thickness = 6;
hole_dia = 3;
num_teeth = 20;

difference() {
union(){
gear(number_of_teeth=num_teeth,circular_pitch=300, clearance = 0.1, rim_thickness = gear_thickness);

for ( i = [0:num_teeth]){
	rotate([0,0, (i) * 360 / num_teeth]){
		translate([0,18,gear_thickness/2]){
			cylinder(h=gear_thickness,d = .9,center = true,$fn = 20);
		}
	}
}
}

union(){
translate([0,0,-5]){
	cylinder(h= 25,d=hole_dia, center = true, $fn = 20);
}

for ( i = [0:num_teeth]){
	rotate([0,0, (i +.5) * 360 / num_teeth]){
		translate([0,15.3,0]){
			cylinder(h= 2 * gear_thickness,d = 1.35,center = true,$fn = 20);
		}
	}
}
}
}




