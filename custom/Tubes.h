#pragma once

struct Tube
{
	vec3 end;
	vec3 color;
	float thickness;

	Tube(vec3 _end, vec3 _color, float _thickness) {
		end = _end;
		color = _color;
		thickness = _thickness;
	}
};

struct Tubes
{
	PArray<Tube> points;
	i32 lastDir;
};

global ObjectContainer<Tubes> myTubes;

void TubeOperation(Node *self)
{
	if(!self->initialized) {
		Tubes tubes = {};
		tubes.points = PArray<Tube>();
		tubes.lastDir = 0;
		self->extraHandle = myTubes.Insert(tubes);
		self->initialized = true;
	}

	Tubes *tubes = myTubes.Get(self->extraHandle); 
	if(!tubes)
		return;

	vec3 directions[6] = {
		vec3(1, 0, 0),
		vec3(-1, 0, 0),
		vec3(0, 1, 0),
		vec3(0, -1, 0),
		vec3(0, 0, 1),
		vec3(0, 0, -1),
	};

	vec3 colors[6] = {
		vec3(0.6f, 0.4f, 0.0f),
		vec3(0.2f, 0.7f, 0.0f),
		vec3(0.2f, 0.1f, 0.4f),
		vec3(0.3f, 0.9f, 0.1f),
		vec3(0.5f, 0.3f, 0.2f),
		vec3(0.4f, 0.2f, 0.9f),
	};

	vec3 col = self->params[1].Vec3();
	float speed = self->params[0].Double();
	float thickness = self->params[2].Double();

	if(tubes->points.Count() == 0) {
		tubes->points.Insert(Tube(vec3(0, 0, 0), col, thickness));
	}
	else {
		i32 r = RandInt(0, 6);
		vec3 lastPoint = tubes->points[tubes->points.Count() - 1].end;
		Tube newTube = Tube(lastPoint + (directions[r] * speed), col, thickness);
		tubes->points.Insert(newTube);
	}

	for(i32 i = 0; i < tubes->points.Count(); i++) {
		if(i == 0)
			continue;

		vec3 lastPoint = tubes->points[i - 1].end;
		vec3 currentPoint = tubes->points[i].end;
		ImDraw3DSetColor(tubes->points[i].color);
		ImDraw3DCylinder(lastPoint, currentPoint, tubes->points[i].thickness, 20);
	}

	if(tubes->points.Count() > 1000) {
		tubes->points.Clear();
	}
}

ObjectHandle CreateTubeNode(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("speed", 0.05),
		NodeParameter("color", vec3(1, 1, 1)),
		NodeParameter("thickness", 0.01),
	};

	FixedArray<NodeInput> inputs = {
	};

	ObjectHandle handle = AddNode(name.buffer, DATA_NONE, op, drawFunc, params, inputs);
	return handle;
}
