#include "RenderThread.h"


RenderThread::RenderThread() {

}


RenderThread* RenderThread::_instance = nullptr;

RenderThread& RenderThread::GetInstance() {
	if (_instance == nullptr) {
		_instance = new RenderThread();
	}

	return *_instance;
}