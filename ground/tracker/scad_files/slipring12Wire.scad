

module slipring()
{
	union(){
		translate ([0,0,-25.5 + 5 + 1.2]){
			cylinder(h = 25.5, d = 12.6, $fn = 30);
		}
		difference(){
			cylinder (h = 1.2, d = 24,$fn = 30);
			for ( i =[0:2]){
				rotate([0,0,120 * i]){
					translate([9,0,-.1]){
						cylinder(h=1.4,d = 3, $fn = 12);
					}
				}
			} 
		} // difference
		translate([0,0,1.2 + 5]){
			cylinder (h = 3, d = 5,$fn = 12);
		}
	} // union 
}
