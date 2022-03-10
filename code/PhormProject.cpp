void InitializeProjectState()
{
	_projectState = new ProjectState();
}

void CleanupProjectState()
{
	delete _projectState;
}
