
module vector()
{
	rotate([0,90,0]){
		cylinder(h = 10,d=1,$fn = 20);
		translate([0,0,5]){
			cylinder(d1= 3, d2 = 1, h = 3);
	   }
	}
	
}

module dir()
{
   rotate_extrude(){
		translate([10,0,0]){
			circle (r=1,$fn = 30);
		}
	}
	translate([10,0,0]){
   rotate([-90,0,0]){
		cylinder(d1 = 2,d2 = 5, h= 3);
	}
   }
}

import("out.stl", convexity = 5);

translate([3,10,5]){
	rotate([0,-135,0]){
		vector();
	}
	rotate([0,135,0]){
		dir();
	}
}










