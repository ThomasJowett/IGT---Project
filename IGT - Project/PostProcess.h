#pragma once
#include "Commons.h"
#include <iostream>
#include "Messaging.h"
#include "Settings.h"



class FrameBuffer
	:public Observer< SettingsEvent, Vector2D>
{
private:
	GLuint mFBO;
	GLuint mTextureColour;
	GLuint mTextureDepth;
	std::vector<GLenum> drawbuffer;

public:
	FrameBuffer() { Settings::GetInstance()->AddObserver(this); }
	~FrameBuffer()
	{
		Destroy();
		Settings::GetInstance()->RemoveObserver(this);
	}

private:
	void GenerateColourTexture(unsigned int width, unsigned int height) 
	{
		glGenTextures(1, &mTextureColour);
		glBindTexture(GL_TEXTURE_2D, mTextureColour);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}

	void GenerateDepthTexture(unsigned int width, unsigned int height)
	{
		glGenTextures(1, &mTextureDepth);
		glBindTexture(GL_TEXTURE_2D, mTextureDepth);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}

public:
	void GenerateFBO(unsigned int width, unsigned int height)
	{
		glGenFramebuffers(1, &mFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		GenerateColourTexture(width, height);
		GenerateDepthTexture(width, height);

		GLuint attachment_index_colour_texture = 0;

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment_index_colour_texture, mTextureColour, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTextureDepth, 0);

		drawbuffer.push_back(GL_COLOR_ATTACHMENT0 + attachment_index_colour_texture);
		glDrawBuffers(drawbuffer.size(), &drawbuffer[0]);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "ERROR: FrameBuffer incomplete" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Destroy()
	{
		glDeleteFramebuffers(1, &mFBO);
		glDeleteTextures(1, &mTextureColour);
		glDeleteTextures(1, &mTextureDepth);
	}

	void OnNotify(SettingsEvent event, Vector2D data)override
	{
		if (event == SettingsEvent::ON_RESOLUTION_CHANGE)
		{
			Destroy();
			GenerateFBO((unsigned int)data.x, (unsigned int)data.y);
		}
	}

	GLuint GetColourTexture() { return mTextureColour; }
	GLuint GetDepthTexture() { return mTextureDepth; }

	void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, mFBO); }
	void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

};