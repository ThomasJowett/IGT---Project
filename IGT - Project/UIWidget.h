#pragma once
#include "GameObject.h"
#include "Messaging.h"
#include "Collider.h"
#include "Settings.h"

enum WidgetStatus
{
	NORMAL = 0,
	HOVERED,
	PRESSED
};

enum class WidgetEvent {ON_HOVERED, ON_UNHOVERED, ON_CLICKED, ON_DRAGGED, ON_PRESSED};

struct WidgetEventData
{
	int ID;
	int value;
};

class UIWidget :
	public GameObject, public Subject<WidgetEvent, WidgetEventData> , public Observer<SettingsEvent, Vector2D>
{
public:
	UIWidget(const char* name, Vector2D anchorPoint, Vector2D offset);
	virtual ~UIWidget();

	virtual bool OnPressed();
	virtual bool OnReleased();
	virtual bool OnHovered();
	virtual bool OnUnHovered();
	virtual void OnDragged(Vector2D mousePosition);

	WidgetEventData mWidgetData;

	bool ContainsMouse(Vector2D mousePosition);

	bool GetIsFocusable() const { return mIsFocusable; }
	void SetIsFocusable(bool isFocusable) { mIsFocusable = isFocusable; }
	
	Box2D* GetCollisionBox() { return mCollisionBox; }
	Box2D* SetCollisionBox(Box2D* box) { mCollisionBox = box; }

	void OnNotify(SettingsEvent event, Vector2D data)override;

	Vector2D GetOffset() const { return mOffset; };
protected:
	Box2D * mCollisionBox = nullptr;
private:
	
	bool mIsFocusable;

	Vector2D mAnchorPoint;
	Vector2D mOffset;

	WidgetStatus mWidgetStatus = NORMAL;

	void CalculateScreenPosition(Vector2D resolution);
};