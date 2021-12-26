void PointLightOperation(Node *self)
{
	PointLight *output = _nodeState->pointLights.Get(&self->GetDataLast());
	if(!output)
		return;

	output->pos = self->params[0].Vec3();
	output->color = self->params[1].Vec3();
}

ObjectHandle CreatePointLightNode(String name, vec2 pos, DataType dataType, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
		NodeParameter("pos", vec3(0, 0, 0)),
		NodeParameter("color", vec3(1.0f, 1.0f, 1.0f)),
		NodeParameter("label", ""),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(name.buffer, pos, dataType, op, drawFunc, params, inputs);
}
