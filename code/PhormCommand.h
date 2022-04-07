class Command
{
 public:
	virtual void Do() {};
	virtual void Undo() {};
};

class MoveNodeCommand : public Command
{
	vec2 offset; 
	ObjectHandle handle;

 public:
	MoveNodeCommand(vec2 _offset, ObjectHandle _handle) {
		offset = _offset;
		handle = _handle;
	}

	void Do()
	{
		Node *node = GetNode(handle);
		if(node) {
			node->rect.pos += offset;
		}
	}

	void Undo()
	{
		Node *node = GetNode(handle);
		if(node) {
			node->rect.pos -= offset;
		}
	}

};

struct CommandState
{
	PArray<MoveNodeCommand> commands;
};

global CommandState *_commandState;
