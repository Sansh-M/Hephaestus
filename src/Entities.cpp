#include <iostream>
#include <vector>
#include "constants.hpp"

/*
Entity class, used to 
*/
class Entity {
	public:
		Entity(const Vec3& origin, float mass) 
			: origin(origin), mass(mass) {}

		std::vector<float> grav_Effect(std::vector<float>& position) {
			
		}	

		Vec3 get_pos() {
			return origin;
		}

		
	private:
		Vec3 origin;
		float mass; 
		int aerosurface; 

};

