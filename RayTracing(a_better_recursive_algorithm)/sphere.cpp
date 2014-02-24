#include "math.h"

#include "sphere.h"

Sphere::Sphere(Material mat, Point centre, float radius) : GObject()
{
	material() = mat;
	
	Centre = centre;
	Radius = radius;
}

ostream& operator<<(ostream& s, Sphere sphere)
{
	s << "\nSphere:" << "\n"
	  << "Centre: " << sphere.Centre << " "
	  << "Radius: " << sphere.Radius << "\n"
	  << "Material: " << sphere.material();
	  
	return s;
}


istream& operator>>(istream& s,Sphere& sphere)
{
	s >> sphere.Centre >> sphere.Radius >> sphere.material();
	return s;
}

void Sphere::print(ostream &s)
{
	s << *this;
}

void Sphere::read(istream& s)
{
	s >> *this;
}

bool Sphere::intersect(Ray ray, float& t, Colour& colour)
//ok not an elegant rendition, but making use of what I already have
{
	/*_invisible_*/
	float a,b,c,r;
	float dx,dy,dz;
	float A,B,C,d;
	float x1,y1,z1,x2,y2,z2;
	float t1,t2;
	
	/*get the centre*/
	a = Centre.x();
	b = Centre.y();
	c = Centre.z();
	r = Radius;
	
	/*coordinates of the ray*/
	x1 = ray.origin().x();
	y1 = ray.origin().y();
	z1 = ray.origin().z();

	/*coordinates at the other end*/
	x2 = x1 + ray.direction().x();
	y2 = y1 + ray.direction().y();
	z2 = z1 + ray.direction().z();
	
	/*translate to the origin*/
	x1 = x1-a; y1 = y1-b; z1 = z1-c;
	x2 = x2-a; y2 = y2-b; z2 = z2-c;
	
	/*set up and solve the quadratic equation*/
	dx = x2-x1;
	dy = y2-y1;
	dz = z2-z1;
	
	A = dx*dx + dy*dy + dz*dz;
	B = x1*dx + y1*dy + z1*dz;
	C = x1*x1 + y1*y1 + z1*z1- r*r;
	
	/*there is no intersection if this happens*/
	if((d=(B*B-A*C)) < 0.0) return false;

	d = (float)sqrt((double)d);
	t1 = (-B-d)/A;
	t2 = (-B+d)/A;

	/*want the smallest of these*/
	t = t1;
	
	/*return the colour*/
	colour = material().ambient(); //note that this is simplification

	return true;
	/*_invisible_*/
}

	

