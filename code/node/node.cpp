#include "node.h"

DataHandle Node::GetData()
{
	bool inputChanged = false;
	for(int i = 0; i < inputs.Count(); i++) {
		if(inputs[i].isset) {
			Node *inputNode = GetNode(inputs[i].handle);
			if(inputNode->changed)
				inputChanged = true;
		}
	}

	if(changed || inputChanged) {
		changed = false;
		op(this);
	}

	return dataHandle;
}


