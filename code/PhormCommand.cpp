void InitializeCommands()
{
	_commandState = new CommandState();
}

void CommandAdd(Command cmd)
{
	_commandState->commands.Insert(cmd);
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
	String name = String(self->deleteNode.node.name);
	NodeConstructor *nodeConstructor = GetNodeConstructors()->Get(name);
	if(nodeConstructor) {
		SetupNode(&self->deleteNode.node, nodeConstructor);
	}

	GetNodes()->Insert(self->deleteNode.handle, self->deleteNode.node);
}
