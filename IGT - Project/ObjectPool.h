#ifndef _OBJECTPOOL_H
#define _OBJECTPOOL_H

#include <list>

template<class T>
class ObjectPool
{
public:
	ObjectPool(T* prefab)
		:mPrefab(prefab){}
	~ObjectPool()
	{
		CleanUp();
	}

	T* GetObject()
	{
		if (mObjects.empty())
		{
			return new T(*mPrefab);
			//TODO:make it so that this returns a deep copy rather than the same thing
		}
		else
		{
			T* newObject = mObjects.front();
			mObjects.pop_front();
			return newObject;
		}
	}

	void ReturnObject(T* object)
	{
		mObjects.push_back(object);
	}

	void CleanUp()
	{
		for (T* object : mObjects)
			delete object;

		mObjects.clear();
	}

	void PreLoadObjects(int numberToLoad)
	{
		for (int i = 0; i < numberToLoad; i++)
		{
			ReturnObject(GetObject());
		}
	}

private:
	std::list<T*> mObjects;
	T* mPrefab;
};
#endif // !_OBJECTPOOL_H