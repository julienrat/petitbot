difference(){

union(){
 translate([-0.5,8.5,2]) flexbatterAA(n=4);

 linear_extrude(height = 4, center = false, convexity = 10)
   import (file = "chassis_servo_9g.dxf",layer="0");
   translate([0,0,0])linear_extrude(height = 25, center = false, convexity = 10)
   import (file = "chassis_servo_9g.dxf",layer="porte_crayon");
}

translate ([30,2,0])cube([2,4,30]);
translate ([30,70-5.5,0])cube([2,4,30]);
}
difference(){
translate ([-13,35,0]) cylinder(r=5,h=20);
translate ([-13,35,0]) cylinder(r=3,h=20);
}
// build a cube with chamfered edges
module chamfered_cube(size,d=1){
   hull(){
     translate([d,d,0])cube(size-2*[d,d,0]);
     translate([0,d,d])cube(size-2*[0,d,d]);
     translate([d,0,d])cube(size-2*[d,0,d]);
   }
}


// draw an arc width height h between radius r1..r2 and angles a1..a2
module arc(r1,r2,h,a1=0,a2=0){
     if(a2-a1<=180){
        difference(){
           cylinder(r=r2,h=h);
           translate([0,0,-1])cylinder(r=r1,h=h+2);
	   rotate(a2)translate([-r1-r2,0,-1])cube([2*(r1+r2),2*(r1+r2),h+2]);
	   rotate(a1+180)translate([-r1-r2,0,-1])cube([2*(r1+r2),2*(r1+r2),h+2]);
        }
     } else {
           difference(){
              cylinder(r=r2,h=h);
              translate([0,0,-1])cylinder(r=r1,h=h+2);
              intersection(){
	       rotate(a2)translate([-r1-r2,0,-1])cube([2*(r1+r2),2*(r1+r2),h+2]);
	       rotate(a1+180)translate([-r1-r2,0,-1])cube([2*(r1+r2),2*(r1+r2),h+2]);
	      }
           }
     }
}

/////////////////////////////////////////////////////////////////////////////////
// sline - generate a "snake line" of width w and height h 
// with a arbitrary sequence of segments defined by a radius and a turning angle
//
//   angle[i] > 0  left turn / counter-clockwise
//   angle[i] < 0  left turn / clockwise
//   angle[i] = 0  straight segment with length radius[i]
//
/////////////////////////////////////////////////////////////////////////////////
// Heinz Spiess, 2014-09-06 (CC BY-SA)
/////////////////////////////////////////////////////////////////////////////////
module sline(angle,radius,i,w,h){
   scale([angle[i]>=0?1:-1,1,1])
      assign(r=abs(radius[i]))assign(a=angle[i])
         translate([a?r:0,0,0]){
	    translate([-w/2,-r-0.01,0])cube([w,0.02,h]); // tiny overlap!
            if(a)arc(r-w/2,r+w/2,0,a,h=h);
	    else if(r>0)translate([-w/2,-r,0])cube([w,r,h]);
      if(i+1<len(angle))
           rotate(angle[i])
	      translate([a?-r:0,a?0:-r,0])
	         sline(angle,radius,i+1,w,h);
  }
}

//////////////////////////////////////////////////////////////////////////////////
//
//  FLEXBATTER:  Flexing battery holder with integrated plastic spring
//
//////////////////////////////////////////////////////////////////////////////////
module flexbatter(n=1,l=65,d=18,hf=0.75,r=4,shd=3,eps=0.28,el=0,xchan=[1/4,3/4],$fn=24){
ew=0.56;   // extrusion width
eh=0.25;   // extrusion height
w = 4*ew;  // case wall thickness
ws = 2*ew; // spring wall thickness
ch = w-ws; // edge chamfering
deepen=0; //deepening for side grip of batteries
//el = 0;  // extra length in spring
//eps = 0.28;
//shd = 3; // screw hole diameter
//hf=0.75;
//xchan=[1/4,3/4]; // relative position of traversal wire channels

r = d/5+2*ws; // linear spring length (depends on sline() call!)

   for(i=[0:n-1])translate([0,i*(d+w+ws),0]){ // generate n battery cases
      difference(){
         
         union(){
            difference(){
               // main body
               translate([0,-w-d/2,0])
	          chamfered_cube([l+w,d+2*w+(i<n-1?ws:0),hf*d+w+ch],ch);
	       // main cavity
               translate([-2,-d/2,w])cube([2+l,d,d+1]);
	       // cavity for locating plastic spring
               translate([-1,-d/2,-1])cube([2,d,d+w+2]);
	       // small cylindrical carving of walls and bottom
	       if(eps>0)translate([-1,0,d/2+w])rotate([0,90,0])cylinder(r=d/2+eps,h=l+1);
            }
      
            // plastic spring for minus pole
            for(sy=[-1,1])scale([1,sy,1])assign(D=d+2*w-2*ws-0.7){
               translate([ch,d/2+w-ws/2,0])rotate(-90)
		  //sline([90,0,120,90,120,90,0],[d/8+2,d/6,d/8-1,d/8,-d/8,d/8,d/2],0,ws,hf*d+w);
		  sline([0,180,0,180,0,-180,0,90,0],[r+ch+el,D/4,el,D/12,el/2,D/12,1+el/2,D/5,D/3],0,ws,hf*d+w);


            }
         }
      
         // lower and upper holes for contacts
         for(z=[-2*ws,2*ws])
            translate([-2*ws,-w,w-ws/2+d/2+z])cube([l+2*w+2,2*w,ws]);
      
         // longitudinal bottom wire channel
         translate([-2*ws,0,0])rotate([0,90,0])cylinder(r=w/2,h=l+w+2+r,$fn=5);
      
         // traversal bottom wire channels
         for(x=l*xchan)translate([x,-d/2-w-1,eh]) rotate([-90,0,0])cylinder(r=w/2,h=d+2*w+ws+2,$fn=6);
   
         // grip deepening
	 if(deepen>0)
            translate([w+l/2-0.5,-d/2-w-0.01,w+d+l])
	       rotate([-90,0,0]){
	          cylinder(r=l+deepen*d,h=d+2*w+2*ws+2,$fn=72);
	          if(i==0)cylinder(r1=l+deepen*d+ch,r2=l+deepen*d,h=ch+0.02,$fn=72);
	          if(i==n-1)translate([0,0,d+2*w-ch])cylinder(r2=l+deepen*d+ch,r1=l+deepen*d,h=ch+0.02,$fn=72);

               }
         // conical screw holes in corners
         for(x=[7+shd,l-2*shd])for(y=[-d/2+shd,d/2-shd])
             translate([x,y,-1]){
                cylinder(r=shd/2,h=w+2);
                translate([0,0,w-shd/2+1])cylinder(r1=shd/2,r2=shd,h=shd/2+0.01);
             }

         // holes for wires passing inside
         for(sy=[-1,1])scale([1,sy,1]){
	    translate([l-1,-d/2,w])cube([w+2,2,2]);
            for(x=[3,l-7])translate([x,-d/2-w-ws-1,w])cube([3,w+ws+3,2]); 
            translate([3,-d/2+w/2-0.75,-1])cube([3,1.5,w+2]); 
            translate([-0.5,-d/2+w/2,0])rotate([0,90,0])cylinder(r=w/2,h=6.5,$fn=5);
         }

         // engrave battery symbol
	 translate([w+l/2,d/4+1,w])cube([l/5,d/4.5,4*eh],true);
	 translate([w+l/2+l/10,d/4+1,w])cube([d/7,d/10,4*eh],true);
	 // engrave plus symbol
	 assign(sy=(l>12*shd)?1:-1){ // for short batteries +- on the side
	    translate([w+l/2+l/(sy>0?5:10),sy*(d/4+1),w]){
	       cube([1,d/4,4*eh],true);
	       cube([d/4,1,4*eh],true);
            }
	 // engrave minus symbol
	    translate([w+l/2-l/(sy>0?5:10),sy*(d/4+1),w])
	       cube([1,d/4,4*eh],true);
         }
   
         //correction for middle separators
         //if(i<n-1) translate([-d,d/2+w-ws/2,-1])cube([d,ws/2+0.1,d+2]);
	 //else translate([1,d/2+w,-0.01])cylinder(r1=ch,r2=0,h=ch);
         //if(i>0) translate([-d,-d/2-w-0.1,-1])cube([d,ws/2+0.1,d+2]);
	 //else translate([1,-d/2-w,-0.01])cylinder(r1=ch,r2=0,h=ch);


      }


      // horizontal contact bulges (+ and - pole)
      for(x=[-0.3,l])
         hull()for(y=[-3+el,3-el])
            translate([x,y,w+d/2])sphere(r=ws);
   
      // vertical contact bulge (+ pole only)
      if(0)hull()for(z=[-3+el,3-el])for(x=[0,w-ws])
            translate([l+x,0,w+d/2+z])sphere(r=ws);



   }

}

module flexbatter18650(n=1){
   flexbatter(n=n,l=65.5,d=18.4,hf=0.75,shd=3,eps=0.28);
}  

module flexbatter18650P(n=1){
   flexbatter(n=n,l=67.5,d=18.4,hf=0.75,shd=3,eps=0.28);
}  

module flexbatterCR123A(n=1){
   flexbatter(n=n,l=35.1,d=16.7,hf=0.75,shd=3,xchan=[0.5],eps=0.28);
}  

module flexbatterD(n=1){
   flexbatter(n=n,l=61.5,d=34.0,hf=0.75,shd=3,eps=0.28);
}  

module flexbatterC(n=1){
   flexbatter(n=n,l=51.6,d=26.4,hf=0.75,shd=3,eps=0.28);
}  

module flexbatterAA(n=1){
   flexbatter(n=n,l=51.6,d=14.4,hf=0.80,shd=2.5,el=0.5,eps=0.28);
}  

module flexbatterAAA(n=1){
   flexbatter(n=n,l=46.1,d=10.45,hf=0.84,shd=2,el=1,xchan=[0.5],eps=0);
}  

module flexbatter18650x1(){ // AUTO_MAKE_STL
  flexbatter18650(n=1);
}

module flexbatter18650Px1(){ // AUTO_MAKE_STL
  flexbatter18650P(n=1);
}

module flexbatterCR123Ax1(){ // AUTO_MAKE_STL
  flexbatterCR123A(n=1);
}

module flexbatterAAx1(){ // AUTO_MAKE_STL
  flexbatterAA(n=1);
}

module flexbatterAAAx1(){ // AUTO_MAKE_STL
  flexbatterAAA(n=1);
}

module flexbatterCx1(){ // AUTO_MAKE_STL
  flexbatterC(n=1);
}

module flexbatter18650x2(){ // AUTO_MAKE_STL
  flexbatter18650(n=2);
}

module flexbatterAAx2(){ // AUTO_MAKE_STL
  flexbatterAA(n=2);
}

module flexbatterAAAx2(){ // AUTO_MAKE_STL
  flexbatterAAA(n=2);
}

module flexbatterCx2(){ // AUTO_MAKE_STL
  flexbatterC(n=2);
}

module flexbatter18650x3(){ // AUTO_MAKE_STL
  flexbatter18650(n=3);
}

module flexbatterAAx3(){ // AUTO_MAKE_STL
  flexbatterAA(n=3);
}

module flexbatterAAAx3(){ // AUTO_MAKE_STL
  flexbatterAAA(n=3);
}

module flexbatter18650x4(){ // AUTO_MAKE_STL
  flexbatter18650(n=4);
}

module flexbatterAAx4(){ // AUTO_MAKE_STL
  flexbatterAA(n=4);
}

module flexbatterAAAx4(){ // AUTO_MAKE_STL
  flexbatterAAA(n=4);
}

// uncomment as needed:

//flexbatterCR123A(n=2);
//rotate([0,0,0])translate([0,0,-9])flexbatter18650(n=1);
//translate([0,40,0])rotate([90,0,0])translate([0,0,-9])flexbatter18650(n=1);
//translate([0,80,0])rotate([180,0,0])translate([0,0,-9])flexbatter18650(n=1);
//translate([0,120,0])rotate([-90,0,0])translate([0,0,-9])flexbatter18650(n=1);
//translate([0,33,0])flexbatter18650(n=2);
//translate([0,90,0])flexbatter18650(n=3);
//translate([-90,33,0])flexbatter18650(n=4);
//translate([0,28,0])flexbatterAA(n=1);
//translate([0,50,0])flexbatterAAA(n=1);
//flexbatterC(n=1);
//flexbatterD(n=1);
