#pragma once
#include "dispatcher.h"
#include <iostream>
namespace spiritsaway::event_util
{
    template < typename... Args>
    class state_machine;
    template < typename... Args>
    class state
    {
    public:
        state_machine<Args...>& _owner;
		spiritsaway::event_util::dispatcher<Args...> _dispatcher;


		virtual void on_create()
		{
			std::cout << "create state " << name() << std::endl;
		}
		virtual void on_enter()
		{
			std::cout << "enter state " << name() << std::endl;
		}
		virtual void on_exit()
		{
			std::cout << "exit state " << name() << std::endl;

		}
		virtual std::string name() const
		{
			return "invalid";
		}
		bool change_to(const std::string& dest_state)
		{
			return _owner.change_to(dest_state);
		}
		template <typename K, typename T>
		void process_event(const K& event, const T& data)
		{
			_dispatcher.dispatch(event, data);
		}
        template <typename K, typename T>
		void notify_owner(const K& event, const T& data)
		{
			_owner.local_dispatch(event, data);
		}
    public:
        state(state_machine<Args...>& in_owner)
        : _owner(in_owner)
        {

        }
    };
    template <typename... Args>
    class state_machine
    {
    protected:
		const std::string default_state_name;
        spiritsaway::event_util::dispatcher<Args...> _dispatcher;
        std::unordered_map<std::string, std::unique_ptr<state<Args...>>> _states;
        state<Args...>* _cur_state = nullptr;
	public:

        bool change_to(const std::string& state_name)
        {
            auto state_iter = _states.find(state_name);
            if(state_iter == _states.end())
            {
                return false;
            }
            if(_cur_state)
            {
                _cur_state->on_exit();
            }
            _cur_state = state_iter->second.get();
            _cur_state->on_enter();
			return true;
        }
        state_machine(const std::string& in_default_state)
			: default_state_name(in_default_state)
        {
        }
		virtual void reset()
		{
			change_to(default_state_name);
		}
		template <typename K, typename V>
		void process_event(const K& event, const V& data)
		{
			if (!_cur_state)
			{
				return;
			}
			_cur_state->process_event(event, data);
		}
		template <typename K, typename V>
		void local_dispatch(const K& event, const V& data)
		{
			_dispatcher.dispatch(event, data);
		}
		std::string active_state() const
		{
			if (!_cur_state)
			{
				return "";
			}
			return _cur_state->name();
		}
    protected:
        
        template <typename T, typename... kwargs>
        void add_state(kwargs&&... arguments)
        {
            auto cur_state = std::make_unique<T>(*this, std::forward<kwargs>(arguments)...);
			cur_state->on_create();
            _states[cur_state->name()] = std::move(cur_state);
        }
    };


}