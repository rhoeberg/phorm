void InitializeCommands()
{
	_commandState = new CommandState();
}

void CommandAdd(Command cmd)
{
	_commandState->commands.Insert(cmd);
}

void CommandAddDeleteNode(ObjectHandle handle, Node node)
{
	i32 i = _commandState->commands.InsertNew();
	_commandState->commands[i].type = CMD_DeleteNode;
	_commandState->commands[i].undo = CMDDeleteNode::Undo;
	new(&_commandState->commands[i].deleteNode) CMDDeleteNode(handle, node);
}

void CommandAddDeleteNode(PArray<ObjectHandle> handles, PArray<Node> nodes)
{
	i32 i = _commandState->commands.InsertNew();
	_commandState->commands[i].type = CMD_DeleteNode;
	_commandState->commands[i].undo = CMDDeleteNode::Undo;
	new(&_commandState->commands[i].deleteNode) CMDDeleteNode(handles, nodes);
}

void CommandUndoLast()
{
	Command *cmd = _commandState->commands.PopLast();
	if(cmd) {
		cmd->undo(cmd);
	}
}

void CleanupCommands()
{
	delete(_commandState);
}


void CMDMoveNode::Undo(Command *self)
{
	Node *node = GetNode(self->moveNode.handle);
	if(node) {
		node->rect.pos -= self->moveNode.offset;
	}
}

void CMDDisconnectInput::Undo(Command *self)
{
	Node *inputNode = GetNode(self->disconnectInput.inputHandle);
	Node *outputNode = GetNode(self->disconnectInput.outputHandle);
	if(inputNode && outputNode) {
		if(self->disconnectInput.ctx <  inputNode->inputs.Count()) {
			inputNode->inputs[self->disconnectInput.ctx].handle = self->disconnectInput.outputHandle;
		}
	}
}

void CMDAddNode::Undo(Command *self)
{
	DeleteNode(self->addNode.handle);
}

// TODO (rhoe) NEXT, hook back inputs / params
void CMDDeleteNode::Undo(Command *self)
{
	CMDDeleteNode *cmd = &self->deleteNode;

	for(i32 i = 0; i < cmd->handles.Count(); i++) {
		String name = String(cmd->nodes[i].name);
		// TODO (rhoe) why is node constructor returning something when
		//             empty string is used as key???
		///            must be a problem with hashmap
		NodeConstructor *nodeConstructor = GetNodeConstructors()->Get(name);
		if(nodeConstructor) {
			SetupNode(&cmd->nodes[i], nodeConstructor);
		}

		GetNodes()->Insert(cmd->handles[i], cmd->nodes[i]);
	}
}
