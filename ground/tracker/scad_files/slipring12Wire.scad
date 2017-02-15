

module slipring()
{
	union(){
		translate ([0,0,-25.5 + 5 + 1.2]){
			cylinder(h = 25.5, d = 12.6, $fn = 30);
		}

		translate([0,0,1.2 + 5]){
			cylinder (h = 3, d = 5,$fn = 12);
		}
	} // union 
}
