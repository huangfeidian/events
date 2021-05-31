#pragma once
#include "dispatcher.h"
#include <iostream>
namespace spiritsaway::utility::events
{
	template <typename Owner, typename... Args>
	class state_machine;
	template <typename Owner, typename... Args>
	class state
	{
	public:
		state_machine<Owner, Args...>& m_statem;
		dispatcher<Args...> m_dispatcher;


		virtual void on_create()
		{
		}
		virtual void on_enter()
		{
		}
		virtual void on_exit()
		{

		}
		virtual std::string name() const
		{
			return "invalid";
		}
		std::string static_name()
		{
			return "invalid";
		}
		bool change_to(const std::string& dest_state)
		{
			return m_statem.change_to(dest_state);
		}
		template <typename K, typename T>
		void process_event(const K& event, const T& data)
		{
			m_dispatcher.dispatch(event, data);
		}
		template <typename K, typename T>
		void notify_statem(const K& event, const T& data)
		{
			m_statem.local_dispatch(event, data);
		}
	public:
		state(state_machine<Owner, Args...>& in_statem)
		: m_statem(in_statem)
		{

		}
	protected:
		Owner* owner()
		{
			return m_statem.owner();
		}
	};
	template <typename Owner, typename... Args>
	class state_machine
	{
	protected:
		const std::string default_state_name;
		dispatcher<Args...> m_dispatcher;
		std::unordered_map<std::string, std::unique_ptr<state<Owner, Args...>>> m_states;
		state<Owner, Args...>* m_cur_state = nullptr;
		Owner* const m_owner;
	public:

		bool change_to(const std::string& state_name)
		{
			auto state_iter = m_states.find(state_name);
			if(state_iter == m_states.end())
			{
				return false;
			}
			if(m_cur_state)
			{
				m_cur_state->on_exit();
			}
			m_cur_state = state_iter->second.get();
			m_cur_state->on_enter();
			return true;
		}
		state_machine(const std::string& in_default_state, Owner* in_owner)
			: default_state_name(in_default_state)
			, m_owner(in_owner)
		{
		}
		Owner* owner()
		{
			return m_owner;
		}
		virtual void reset()
		{
			change_to(default_state_name);
		}
		template <typename K, typename V>
		void process_event(const K& event, const V& data)
		{
			if (!m_cur_state)
			{
				return;
			}
			m_cur_state->process_event(event, data);
		}
		template <typename K, typename V>
		void local_dispatch(const K& event, const V& data)
		{
			m_dispatcher.dispatch(event, data);
		}
		std::string active_state() const
		{
			if (!m_cur_state)
			{
				return "";
			}
			return m_cur_state->name();
		}
	protected:
		
		template <typename T, typename... kwargs>
		void add_state(kwargs&&... arguments)
		{
			auto cur_state = std::make_unique<T>(*this, std::forward<kwargs>(arguments)...);
			cur_state->on_create();
			m_states[cur_state->name()] = std::move(cur_state);
		}
	public:
		template <typename T>
		T* get_state()
		{
			auto state_iter = m_states.find(T::static_name());
			if(state_iter == m_states.end())
			{
				return nullptr;
			}
			return dynamic_cast<T*>(state_iter->second.get());
		}
	};


}