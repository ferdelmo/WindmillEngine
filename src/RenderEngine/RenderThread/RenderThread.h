#pragma once


class RenderThread
{
private:
	static RenderThread* _instance;

	RenderThread();
public:
	static RenderThread& GetInstance();
};

