#include "litscene.h"
#include <math.h>


ostream& operator<<(ostream& s, LitScene scene)
{
	scene.print(s);
	
	s << "\n\nLighting:\n";
	
	int i;
	s << "Ambient: " << scene.Ambient <<"\n";
	for(i=0;i<scene.numberOfLights();++i) s << "Light: " << i << " " << scene.lightAt(i) << "\n";
	
	return s;
}
	



istream& operator>>(istream& s, LitScene& scene)
//reading
{
	scene.read(s);
	
	//read the ambient lighting must always be there
	s >> scene.Ambient;
	
	//read how many lights
	int n; s >> n;
	
	if(n==0) return s;
	
	scene.setLighting(n);
	
	Light light; int i;		
	for(i=0;i<n;++i) {
		s >> light;
		scene.lightAt(i) = light;
	}
	return s;
}

void LitScene::setFile(char* file) {
	fileName = file;
}

static float power(float x, int n)
{
    if (n < 1) return 1.0;
    if (n == 1) return x;
    else return x * power(x, n - 1);
}

Colour LitScene::colourOnObject(GObject *object, Point p, Point eye)
{
    bool shadow = false;
	Vector  n = object->normal(p); //normal vector
    Ray shadowray;
	
	Colour colour;	

	//compute ambient component
	Colour ka = object->material().ambient();
	colour = ka*Ambient;
	

	for(int i=0; i<NLights; ++i){
		
		Light light = lightAt(i);
	    // Find the direction to the light from the point 
		Vector vL(0,0,0);
		if(light.directional()) {
			Vector tmpVector = light.vector();
			vL = vminus(vL,tmpVector);
			vL = vL.normalised();
		}
		else { 
			Point tmpPoint = light.point();
			vL = (tmpPoint - p).normalised();
		}

/* ASSIGNMENT: Is the light occluded by another object? (Your code here)*/

		shadow=false;//Initial false shadow in NLights for-loop
		shadowray.origin()=p;//Start from object point
		shadowray.direction()=light.point()-p;// End with light point, so that to control the range using 't'(0>t>1)
		//for each object
		for(int i = 0; i < numObjects(); ++i){
			float t;
			Colour col;
			if((at(i) != object) && at(i)->intersect(shadowray,t,col)){ 
				//at(i) != object to avoid at(i) being itself in colourOnObject
				//intersection found
				if((t < 1) && (t > 0.0) ){
					shadow = true;
					break; // break numObjects for-loop
				}
			}
		}
		if(shadow)continue;// Jump out one loop in the NLights for-loop. No diffuse and specular component for this object point.


		// does the light illuminate this point? 		
		float nl = n^vL; //dot product
		float nh;
		Vector e = eye-p;
		e.normalise();
		Vector h = vadd(e,vL);
		h.normalise(); // (V + L) / |L + V|)
		nh = n^h;
      
		if(nl > 0.0){
	    // if so add diffuse and specular component 
			Colour kd = object->material().diffuse();	
			Colour ks = object->material().specular();	
			Colour Intensity = light.intensity();
			Colour Diffuse = (kd * nl);
			Colour Specular = (ks * power(nh, (int) object->material().shininess()));
			Colour AddedColour = (Diffuse+Specular);	
			Colour Reflection = Intensity*AddedColour;
			colour = colour + Reflection;
		}
	
	
	//check that the colour is in the bounds 0 to 1
	colour.check();
	
	}
	return colour;
}
	
	
	
	
bool LitScene::intersect(GObject* me, Ray ray, Colour& colour, int depth)
/*returns true if the ray intersects the scene, and if so then the colour
at the intersection point. This overrides the method in Scene*/
{
	double tmin = 9999999999.9;
	bool found = false;
	GObject* object = NULL;
    Ray reflected;
    if (depth == 0) return false; /* end of recursion */


	//for each object
	for(int i = 0; i < numObjects(); ++i){
		float t;
		Colour col;
		if((at(i) != me) && at(i)->intersect(ray,t,col)){
			//intersection found
			if((t < tmin) && (t > 0.0) ){
				object = at(i);
				tmin = t;
				//colour = col;
				found = true;
			}
		} 
	}
	if(found){//an object has been found with the smallest intersection
        Colour refl_col;
		/*find the intersection point*/
		Point p = ray.pointAt((float)tmin);
		
		//now we want the colour computed at point p on the object
		colour = colourOnObject(object, p, ray.origin()); //ray.origin() is at the eye position
		
		/* EXTENSION: Recursive ray trace. Use specular
		   colour to modulate the colour returned by the recursive
		   intersect call (your code here). */

			Vector refl_n = object->normal(p); //normal vector
			Vector refl_e = ray.origin()-p; // eye vector = - ray.direction()
			refl_e.normalise();
			reflected.origin() = p;//start from object point
			float refl_ne = refl_n^refl_e;
			reflected.direction() =vminus(refl_n*2.0f*refl_ne,refl_e) ;//end with the other object -e + 2(n*e)*n)
			
			Colour refl_ks = object->material().specular();
		    //Get the specular proportion refl_ks of the object at point p
			intersect(object, reflected, refl_col, depth-1);

			colour = colour + refl_col * refl_ks;
	
			//
			//for(int i = 0; i < numObjects(); ++i){
			////for each object
			//	float refl_t;
			//	double refl_tmin = 9999999999.9;
			//	if((at(i) != object) && at(i)->intersect(reflected,refl_t,refl_col)){ //Recursive calculation of reflected light
			//		//intersection found
			//		if((refl_t < refl_tmin) && (refl_t > 0.0) ){
			//		    //There is an object been reflected
			//			refl_tmin = refl_t;
			//			Point refl_p = reflected.pointAt((float)refl_tmin);
			//			refl_col = colourOnObject(at(i), refl_p, ray.origin());	
			//			//Get specular and diffuse light 
			//			colour = colour + refl_ks*refl_col;
			//			break;
			//			//Jump out of the numObjects for-loop, so that will not include 2 objects (e.g. two points in the same sphere)
			//			//If not the reflected image will become transparent
			//			//If refl_tmin was defined outside the for-loop, there is no need to break this for-loop
			//		}
			//	}
			//}
		


		//check that the colour is in the bounds 0 to 1
        colour.check();  
		return true;
	}
	else 
	{
		return false;
	}
}
