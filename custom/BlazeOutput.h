#pragma once

void BlazeOutputOp(Node *self)
{
	vec3 *output = GetVec3s()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("BlazeOutput: error getting vec3 output");
	}

	i32 partindex = self->params[0].Int();

	vec3 newPos = _blazeReaderState->pData.data[partindex];
	newPos.y = 1.0f - newPos.y;
	vec3 dir = newPos - *output;
	vec3 result = *output + (dir * 0.05f);

	/* *output = vec3(tmp.x, 1.0f - tmp.y, tmp.z); */
	*output = vec3(result.x, result.y, result.z);
}

ObjectHandle CreateBlazeOutputNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("partindex", 0),
	};

	FixedArray<NodeInput> inputs = {
	};

	ObjectHandle result =  AddNode(DATA_VEC3, params, inputs);
	Node *node = GetNode(result);
	node->update = true;
	return result;
}
