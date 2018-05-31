#ifndef _ICOMPONENTS_H

#define _ICOMPONENTS_H

#include "Shader.h"

#define TO_STRING( x ) #x

#define CLASS_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool IsClassType( const std::size_t classType ) const override;                 \

#define CLASS_DEFINITION( parentclass, childclass )                                         \
const std::size_t childclass::Type = std::hash< std::string >()( TO_STRING( childclass ) ); \
bool childclass::IsClassType( const std::size_t classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}                                                                                           \

class Component
{
public:
	static const std::size_t Type;
	virtual bool IsClassType(const std::size_t classType) const 
	{
		return classType == Type;
	}

	Component() = default;
	virtual ~Component() = default;
};

class iUpdateable : public Component
{
	CLASS_DECLARATION(iUpdateable);
public:
	
	virtual void Update(float deltaTime) = 0;
};

class iRenderable : public Component
{
	CLASS_DECLARATION(iRenderable);
public:
	virtual void Render(Shader* shader) = 0;
};

#endif // !_ICOMPONENTS_H