/*
  
  First draft of a command system.
  
  UNDELETE
  to allow for a "undelete" command we probably need to change something
  about how nodes are currently created and given id's
  
  right now undoing a delete command could give troubles if the handle of the
  deleted node has been reused for a new node.
  in this case the old node would need a new handle, but this would mess
  up any connections the old node had + we would not be certain that
  any other nodes it was connected to had also been deleted and replaced.
  this seems like a headache to figure out a way to keep these 
  connections intact.
  
  *node handle reference table*
  a possible change that could make this smoother would be if old handles where
  never reused but instead all handles where stored in a reference table to the index position in the node container. 
  
  example:
  | handle | "actual" handle in container |
  ------------------------------------------
  | 0      | 0                            |
  | 1      | 1                            |
  | 2      | NULL                         |
  | 3      | 2                            |
  
  in this example a node with index 2 have been deleted and later we have added
  a new node and given it slot 2 in the node container. BUT instead of refering
  directly to the handle in the objectcontainer we use a abstracted hande that
  points to it in a seperate handle abstraction table. Everywhere we need to 
  store node handles we instead store these abstracted handles and always take
  the route through the table
  abstracthandel -> table -> realhandle 
  


  BETTER STACK CONTAINER / UNDO LIMIT
  PArray for our command stack gives us the problem that we dont have a 
  good way to limit the stack size. 
  When reaching the maximim undo limit of our stack we would want to keep adding
  new commands to the top and removing the last command. With a PArray the
  only way to do that would be to remove the first element and shift ALL the 
  elements one up which would potentially be very slow. 
  We could use memcpy for the shift but once the stack is full we would have to
  do it after every single command, which seems very wastefull.
  *LinkedList*
  Instead we could use a linked list and just remove first element and set
  root to the second element and add one new command to the end.





 */

enum CommandType {
	CMD_MOVE_NODE,
	CMD_DISCONNECT_INPUT,
	CMD_DISCONNECT_PARAM,
};

struct Command;

struct CMDMoveNode
{
	vec2 offset;
	ObjectHandle handle;
	CMDMoveNode(vec2 _offset, ObjectHandle _handle) {
		offset = _offset;
		handle = _handle;
	}

	static void Undo(Command *self);
};

struct CMDDisconnectInput
{
	i32 ctx;
	ObjectHandle inputHandle;
	ObjectHandle outputHandle;

	CMDDisconnectInput() {}

	static void Undo(Command *self);
};

typedef void(*CommandUndo)(Command *self);

struct Command
{
	CommandType type;
	CommandUndo undo;
	union
	{
		CMDMoveNode moveNode;
		CMDDisconnectInput disconnectInput;
	};

	Command(CMDMoveNode _moveNode)
	{
		type = CMD_MOVE_NODE;
		moveNode = _moveNode;
		undo = CMDMoveNode::Undo;
	}

	Command(CMDDisconnectInput _disconnectInput)
	{
		type = CMD_DISCONNECT_INPUT;
		disconnectInput = _disconnectInput;
		undo = CMDDisconnectInput::Undo;
	}
};

struct CommandState
{
	PArray<Command> commands;
};

void InitializeCommands();
void CommandUndoLast(Command cmd);

global CommandState *_commandState;
