#include <iostream>
#include <vector>
#include "constants.hpp"


class Entity {
	public:
		Entity(Vec3& origin, float mass) 
			: origin(origin), mass(mass) {}

		std::vector<float> grav_Effect(std::vector<float>& position) {
			
		}	

		int atmos_effect() {
			return 0;
		}

		Vec3 get_pos() {
			return origin;
		}

		
	private:
		Vec3 origin;
		float mass; 
		int aerosurface; 

};

