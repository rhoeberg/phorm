void InitializeProjectState()
{
	_projectState = new ProjectState();
}

Project *GetCurrentProject()
{
	return &_projectState->currentProject;
}

String GetAssetPath(String name)
{
	String result(GetCurrentProject()->projectPath);
	result.Concat("/");
	result.Concat(GetCurrentProject()->assetPath);
	result.Concat("/");
	result.Concat(name);
	return result;
}

void CleanupProjectState()
{
	delete _projectState;
}
