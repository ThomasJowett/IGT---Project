#pragma once

#include <vector>
#include <algorithm>

template<class EventType, class DataType>
class Observer 
{
public:
	virtual void OnNotify(EventType, DataType) = 0;
};

template<class EventType, class DataType>
class Subject
{
public:
	Subject() = default;
	~Subject()
	{
		mObservers.clear();
	}
	void Notify(EventType event, DataType data)
	{
		for (Observer<EventType, DataType>* observer : mObservers)
		{
			observer->OnNotify(event, data);
		}
	}
	void AddObserver(Observer<EventType, DataType>* observer)
	{
		mObservers.push_back(observer);
	}

	virtual void RemoveObserver(Observer<EventType, DataType>* observer)
	{
		mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
	}

private:
	std::vector<Observer<EventType, DataType>*> mObservers;
};