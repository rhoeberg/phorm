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
  




  UNIONS
  TODO (rhoe) invistigate using unions instead of seperate arrays with type data




 */

enum CommandType {
	CMD_MOVE_NODE,
	CMD_DISCONNECT_INPUT,
	CMD_DISCONNECT_PARAM,
};

struct CommandMoveNode
{
	vec2 offset;
	ObjectHandle handle;
	CommandMoveNode(vec2 _offset, ObjectHandle _handle) {
		offset = _offset;
		handle = _handle;
	}
};

struct CommandDisconnectInput
{
	i32 ctx;
	ObjectHandle inputNodeHandle;
	ObjectHandle outputNodeHandle;
};

struct Command
{
	CommandType type;
	i32 typeHandle;

	Command(CommandType _type, i32 _typeHandle = -1)
	{
		type  = _type;
		typeHandle  = _typeHandle;
	}
};

struct CommandState
{
	i32 lastCmd;
	PArray<Command> commands;

	PArray<CommandMoveNode> moveNodeCmds;
	PArray<CommandDisconnectInput> disconnectInputCmds;
};

void InitializeCommands();
void CommandUndoLast(Command cmd);

global CommandState *_commandState;
