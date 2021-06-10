class sphere {
	public:
		// see: https://en.wikipedia.org/wiki/Phong_reflection_model
		double ks;  // specular reflection constant
		color ia;  // ambient lighting (RGB val as vec3d)
		double kd;  // diffuse reflection constant
		double ka;  // ambient reflection constant
		double alpha;  // shininess constant for material
		point3d center;  // xyz center of sphere
		double radius;  // sphere radius

		// initialization
		sphere(double ks, color ia, double kd, double ka, double alpha, point3d center, double radius) : ks(ks), ia(ia), kd(kd), ka(ka), alpha(alpha), center(center), radius(radius) {};
};
