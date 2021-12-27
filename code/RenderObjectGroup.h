/*
  We need some kind of renderobject overide type which takes
  a renderobject handle but adds extra position, rotation and scale 
  offsets
  
  this way we can easily reuse the same object but in different positions
  
 */

#pragma

struct RenderObjectInstance
{
	ObjectHandle handle;
	vec3 pos;
	vec3 scale;
	vec3 rot;
};

struct RenderObjectGroup
{
	VMArray<ObjectHandle> renderObjects;
	vec3 pos;
	vec3 scale;
	vec3 rot;

	RenderObjectGroup() {
		renderObjects = VMArray<ObjectHandle>();
	}
};
