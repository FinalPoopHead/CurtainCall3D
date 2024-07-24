#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class MenuItem;

class UIStateContext
{
	class UIElement : public flt::GameObject
	{
		friend UIStateContext;
	public:
		UIElement(const std::string& name, MenuItem* item) : _name(name), _item(item) {}
		virtual ~UIElement() {}

		UIElement& onEnable(std::function<void()> func) { _onEnables.emplace_back(func); return *this; }
		UIElement& onDisable(std::function<void()> func) { _onDisables.emplace_back(func); return *this;}
		UIElement& onUpdate(std::function<void(float)> func) { _onUpdates.emplace_back(func); return *this; }
		UIElement& onActive(std::function<void()> func) { _onActives.emplace_back(func); return *this; }
		UIElement& onSelect(std::function<void(flt::KeyCode)> func) { _onSelects.emplace_back(func); return *this; }

		void Enable() { for (auto& onEnableFunc : _onEnables) { onEnableFunc(); } }
		void Disable() { for (auto& onDisableFunc : _onDisables) { onDisableFunc(); } }
		void Update(float deltaSecond) { for (auto& onUpdateFunc : _onUpdates) { onUpdateFunc(deltaSecond); } }
		void Active() { for (auto& onActiveFunc : _onActives) { onActiveFunc(); } }
		void Select(flt::KeyCode keyCode) { for (auto& onSelectFunc : _onSelects) { onSelectFunc(keyCode); } }

		MenuItem* GetItem() const { return _item; }

	private:
		std::string _name;
		MenuItem* _item;
		std::vector<std::function<void()>> _onEnables;
		std::vector<std::function<void()>> _onDisables;
		std::vector<std::function<void(float)>> _onUpdates;
		std::vector<std::function<void()>> _onActives;
		std::vector<std::function<void(flt::KeyCode)>> _onSelects;
	};


public:
	UIStateContext();
	virtual ~UIStateContext();

protected:
	virtual void Update(float deltaSecond);
	virtual void PostUpdate(float deltaSecond);

	virtual void OnEnable();
	virtual void OnDisable();

public:
	UIElement& AddElement(const std::string& name, MenuItem* item);
	UIElement& GetElement(const std::string& name);

private:
	std::unordered_map<std::string, UIElement> _elements;
	std::unordered_set<flt::KeyCode> _downKeyCodes;
};
