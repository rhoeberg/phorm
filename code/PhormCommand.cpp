void InitializeCommands()
{
	_commandState = (CommandState*)malloc(sizeof(CommandState));
	new(&_commandState->commands) PArray<MoveNodeCommand>();
}
