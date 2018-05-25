#pragma once

#include <vector>

template<class EventType, class DataType
class Subject
{
public:

	Subject() = default;
	~Subject()
	{
		mObservers.clear();
	}
	virtual void Notify(std::string message)
	{
		for (Observer* observer : mObservers)
		{
			observer->RecieveMessage(message);
		}
	}
	virtual void AddSubscriber(Observer<EventType, DataType>* observer)
	{
		mObservers.push_back(observer);
	}

	virtual void RemoveSubscriber(Observer<EventType, DataType>* observer)
	{
		mObservers.erase(observer);
	}

private:
	std::vector<Observer<EventType, DataType>*> mObservers;
};

template<class EventType, class DataType>
class Observer 
{
public:
	virtual void OnNotify(EventType, DataType) = 0;
};