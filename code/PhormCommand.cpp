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
		switch(cmd->type) {
			case CMD_MOVE_NODE: {
				CommandMoveNode moveNodeCmd = _commandState->moveNodeCmds[cmd->typeHandle];
				Node *node = GetNode(moveNodeCmd.handle);
				if(node) {
					node->rect.pos -= moveNodeCmd.offset;
				}
				break;
			}
			case CMD_DISCONNECT_INPUT: {
				CommandDisconnectInput cmdType = _commandState->disconnectInputCmds[cmd->typeHandle];
				Node *inputNode = GetNode(cmdType.inputNodeHandle);
				Node *outputNode = GetNode(cmdType.outputNodeHandle);
				if(inputNode && outputNode) {
					if(cmdType.ctx <  inputNode->inputs.Count()) {
						inputNode->inputs[cmdType.ctx].handle = cmdType.outputNodeHandle;
					}
				}
				break;
			}
		}
	}
}

void CleanupCommands()
{
	delete(_commandState);
}
