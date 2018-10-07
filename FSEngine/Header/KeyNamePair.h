#pragma once
#include <string>

using std::string;

struct KeyNamePair
{
	string key;
	string name;

	KeyNamePair(const string& key, const string& name)
	 : key(key), name(name)
	{
	}

	bool operator==(const KeyNamePair& other) const
	{
		return key == other.key && name == other.name;
	}
};

namespace std
{
	template <>
	struct hash<KeyNamePair>
	{
		size_t operator()(const KeyNamePair& keyNamePair) const
		{
			return hash<string>()(keyNamePair.key) ^ hash<string>()(keyNamePair.name);
		}
	};
}
