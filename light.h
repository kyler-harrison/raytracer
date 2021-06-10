class light {
	public:
		point3d position;
		color is;  // specular color of light I think
		color id;  // color of diffuse part of light I think

	// initialization
	light(point3d position, color is, color id) : position(position), is(is), id(id) {};
};
