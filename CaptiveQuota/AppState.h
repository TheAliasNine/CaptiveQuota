#pragma once

class AppState
{
protected:
	bool closed = false;
	AppState* nextState = nullptr;

public:

	virtual ~AppState() = default;

	virtual void Draw(float) = 0;
	virtual void Update(float) = 0;

	bool Closed() { return closed; }
	AppState* NextState() { return nextState; }
};