#pragma once

class iInput
{
public:
	virtual void MoveUp(float scale) = 0;
	virtual void MoveRight(float scale) = 0;
	virtual void LookUp(float scale) = 0;
	virtual void LookRight(float scale) = 0;
	
	virtual void AButton() = 0;
	virtual void BButton() = 0;
	virtual void XButton() = 0;
	virtual void YButton() = 0;

	virtual void RightTrigger(float scale) = 0;
	virtual void LeftTrigger(float scale) = 0;

	virtual void RightBumper() = 0;
	virtual void LeftBumper() = 0;
	
	virtual void Start() = 0;
	virtual void Select() = 0;

	virtual void Up() = 0;
	virtual void Down() = 0;
	virtual void Left() = 0;
	virtual void Right() = 0;

	virtual void MousePosition(int x, int y) = 0;
};
