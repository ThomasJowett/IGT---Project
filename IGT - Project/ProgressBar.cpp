#include "ProgressBar.h"
#include "Texture2D.h"

ProgressBar::ProgressBar(const char * name, Vector2D anchorPoint, Vector2D offset, const char * path, Vector2D size, float initialValue)
	:UIWidget(name, anchorPoint, offset)
{
	mTextureID = Texture2D::GetTexture2D(path);
	mSize = size;
	mIncrementStep = 1 / mSize.x;
	
	SetIsFocusable(false);

	SetPercent(initialValue);
}

void ProgressBar::SetPercent(float value)
{
	if (value > 1.0f)
		value = 1.0f;
	else if (value < 0.0f)
		value = 0.0f;

	if (value > mCurrentValue + mIncrementStep || value < mCurrentValue - mIncrementStep)
	{
		mCurrentValue = value;
		UpdateBar();
	}
}

void ProgressBar::Render(Shader * shader)
{
	UIWidget::Render(shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	mMesh->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ProgressBar::UpdateBar()
{
	if (mMesh) delete mMesh;

	float width = mSize.x * mCurrentValue;
	float halfWidth = mSize.x / 2;
	float halfHeight = mSize.y / 2;

	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	Vertex vertices[] =
	{
		Vertex(Vector3D(-halfWidth,-halfHeight,0), Vector2D(0,1)),
		Vertex(Vector3D(-halfWidth + width,-halfHeight,0), Vector2D(mCurrentValue, 1)),
		Vertex(Vector3D(-halfWidth + width,halfHeight,0), Vector2D(mCurrentValue,0)),
		Vertex(Vector3D(-halfWidth,halfHeight,0), Vector2D(0,0))
	};

	mMesh = new Mesh(vertices, 4, indices, 6);
}
