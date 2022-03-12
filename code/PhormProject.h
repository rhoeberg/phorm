#pragma once

struct Project
{
	String assetPath;
	String projectPath;
};

struct ProjectState
{
	Project currentProject;
};

Project *GetCurrentProject();
String GetAssetPath(String name);

ProjectState *_projectState;
