#pragma once

struct Project
{
	String assetsPath;
};

struct ProjectState
{
	Project currentProject;
};

ProjectState *_projectState;
