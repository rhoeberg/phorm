#pragma once

void BlazeOutputOp(Node *self)
{
	vec3 *output = GetVec3s()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("BlazeOutput: error getting vec3 output");
	}

	i32 partindex = self->params[0].Int();
	*output = _blazeReaderState.pData.data[partindex];
}

ObjectHandle CreateBlazeOutputNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("partindex", 0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_VEC3, params, inputs);
}
