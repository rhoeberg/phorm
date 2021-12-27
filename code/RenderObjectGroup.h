/*
  We need some kind of renderobject overide type which takes
  a renderobject handle but adds extra position, rotation and scale 
  offsets
  
  this way we can easily reuse the same object but in different positions
  
 */

#pragma once

#include "scene.h"

struct RenderObjectGroup
{
	// could this contain renderobject instances instead of
	// sceneobjects to avoid infinite nesting?
	VMArray<RenderObjectInstance> renderObjects;
	vec3 pos;
	vec3 scale;
	vec3 rot;

	RenderObjectGroup() {
		renderObjects = VMArray<RenderObjectInstance>();
	}
};
