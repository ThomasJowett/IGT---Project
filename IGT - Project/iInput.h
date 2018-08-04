#pragma once

class iInput
{
public:
	virtual void MoveUp(float scale) {}
	virtual void MoveRight(float scale) {}
	virtual void LookUp(float scale) {}
	virtual void LookRight(float scale) {}
	
	virtual void AButtonDown() {}
	virtual void AButtonUp() {}
	virtual void BButtonDown() {}
	virtual void BButtonUp() {}
	virtual void XButtonDown() {}
	virtual void XButtonUp() {}
	virtual void YButtonDown() {}
	virtual void YButtonUp() {}

	virtual void RightTrigger(float scale) {}
	virtual void LeftTrigger(float scale) {}

	virtual void RightBumper() {}
	virtual void LeftBumper() {}
	
	virtual void Start() {}
	virtual void Select() {}

	virtual void Up() {}
	virtual void Down() {}
	virtual void Left() {}
	virtual void Right() {}

	virtual void MousePosition(float x, float y) {}

	virtual void MouseLeftClick() {}
	virtual void MouseLeftUnClick() {}
	virtual void MouseRightClick() {}
	virtual void MouseRightUnClick() {}

	virtual void OnPossessed(int controllerID) { mControllerID = controllerID; }
	virtual void OnUnPossesed() { mControllerID = -1; }

private:
	int mControllerID;
};
