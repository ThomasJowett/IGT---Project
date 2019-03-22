#pragma once

#include <map>

template<typename T>
struct Factory
{
	typedef std::map<std::string, T*(*)()>map_type;

	static T* CreateInstance(std::string const& name)
	{
		typename map_type::iterator it = getMap()->find(name);

		return (it == getMap()->end() ? nullptr : it->second());
	}

protected:
	static map_type *getMap()
	{
		if (!gLookUpTable)
		{
			gLookUpTable = new map_type;
		}
		return gLookUpTable;
	}
private:
	static map_type * gLookUpTable;
};