#pragma once
#include <list>
#include <cstdio>
#include <cassert>

template <typename T>
using Iter_t = typename std::list <T> :: iterator;

template <typename SonType, typename FatherType, typename SelfType>
struct SplitList
{
	using SonIter_t = Iter_t <SonType>;
	using FatherIter_t = Iter_t <FatherType>;
	using SelfIter_t = Iter_t <SelfType>;
	
	private:
	std::list <SonType> L;
	
	public:
	FatherIter_t father;
	SelfIter_t selfptrInFather;

	static SelfIter_t create(FatherIter_t father, SelfIter_t iter)
	{
		auto object = father->insert(iter, SelfType());
		object->father = father;
		object->selfptrInFather = object;
		return object;
	}

	static SelfIter_t create(FatherIter_t father)
	{
		return create(father, father->end());
	}

	SelfIter_t next() 
	{	return std::next(selfptrInFather);	}

	SelfIter_t prev() 
	{	return std::prev(selfptrInFather);	}

	SonIter_t first() 
	{	return L.begin();	}

	typename std::list <SonType> :: const_iterator first() const
	{	return L.begin();	}

	SonIter_t end() 
	{	return L.end();	}

	typename std::list <SonType> :: const_iterator end() const
	{	return L.end();	}

	SonIter_t last()
	{	return std::prev(end());	}

	bool empty() const
	{	return L.empty();	}

	virtual int size() const
	{	return L.size();	}

	auto insert(SonIter_t it, SonType&& value)
	{	return L.insert(it, value);	}

	void erase(SonIter_t it)
	{	L.erase(it);	}

	void destroy()
	{	father->erase(selfptrInFather);	}


	void splice(SplitList& donor, SonIter_t a, SonIter_t b, SonIter_t destination, bool extractingExternal)
	{
		auto& origin = donor.L;

		if (extractingExternal)
		{
			L.splice(destination, origin, origin.begin(), a);
			L.splice(destination, origin, b, origin.end());
		}
		else
			L.splice(destination, origin, a, b);
		for (auto& it : L)
			it.father = selfptrInFather;
	}

	void splice(SplitList& donor, SonIter_t a, SonIter_t b, bool extractingExternal)
	{
		splice(donor, a, b, end(), extractingExternal);
	}

	SelfIter_t split(SonIter_t x, SonIter_t y)
	{
		auto externalIter = x == first() ? y : first();
		auto internalIter = x;
		
		while (externalIter != end() and internalIter != y)
		{
			externalIter++;
			internalIter++;
			if (externalIter == x)
				externalIter = y;
		}
			
		bool internalLonger = internalIter != y;
		// external goes before, internal after

		auto newIter = internalLonger ? selfptrInFather : next();
		newIter = create(father, newIter);
		newIter->splice(*this, x, y, internalLonger);

		return newIter;
	}

	SelfIter_t split(SonIter_t x)
	{	return split(x, L.end());	}

	bool isLeftMarginal() const
	{
		assert(!father->empty());
		return selfptrInFather == father->first();
	}

	bool isRightMarginal() const
	{
		assert(!father->empty());
		return selfptrInFather == father->last();
	}

	bool isMarginal() const
	{
		return isLeftMarginal() or isRightMarginal();
	}

	bool earlierThan(SplitList& other) const
	{
		auto x = other.selfptrInFather;

		if (father != other.father or selfptrInFather == x)
			return false;

		auto forw = selfptrInFather;
		auto back = selfptrInFather;

		while (forw != father->last() and back != father->first())
			if (++forw == x or --back == x)
				return forw == x;

		return back == father->first();
	}
};
