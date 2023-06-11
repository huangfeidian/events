#pragma once
#include <functional>
#include "dispatcher.h"

namespace spiritsaway::utility
{
	struct event_entry
	{
		std::uint32_t category;
		std::uint32_t command;
		bool operator==(const event_entry& other) const
		{
			return category == other.category && command == other.command;
		}
	};
	template <typename E>
	class category_dispatcher
	{
	protected:
		utility::dispatcher<utility::event_entry, E, std::string> m_dispatcher;
	public:

		template <typename C, typename... Args>
		auto dispatch_event(E event_cat, C event_cmd, const Args&...  args)
		{
			return m_dispatcher.dispatch(event_cat, args...) + m_dispatcher.dispatch(utility::event_entry{ std::uint32_t(event_cat), std::uint32_t(event_cmd) }, args...);
		}
		template<typename... Args>
		auto dispatch_event(const std::string& event, const Args&... args)
		{
			return m_dispatcher.dispatch(event, args...);
		}

		template <typename C, typename V>
		utility::listen_handler<utility::event_entry>  add_event_listener(E event_cat, C event_cmd, std::function<void(const utility::event_entry&, const V&)> cur_callback)
		{
			return m_dispatcher.template add_listener<utility::event_entry, V>(utility::event_entry{ std::uint32_t(event_cat), std::uint32_t(event_cmd) }, cur_callback);
		}

		template <typename V>
		utility::listen_handler<E>  add_event_listener(E event_cat, std::function<void(const E&, const V&)> cur_callback)
		{
			return m_dispatcher.template add_listener<E, V>(event_cat, cur_callback);
		}



		template <typename V>
		utility::listen_handler<std::string>  add_event_listener(const std::string& cur_event, std::function<void(const std::string&, const V&)> cur_callback)
		{
			return m_dispatcher.template add_listener<std::string, V>(cur_event, cur_callback);
		}
		bool remove_event_listener(utility::listen_handler<utility::event_entry> cur_handler)
		{
			return m_dispatcher.remove_listener(cur_handler);
		}
		bool remove_event_listener(utility::listen_handler<E> cur_handler)
		{
			return m_dispatcher.remove_listener(cur_handler);
		}
		bool remove_event_listener(utility::listen_handler<std::string> cur_handler)
		{
			return m_dispatcher.remove_listener(cur_handler);
		}
	protected:
		void clear_listeners()
		{
			m_dispatcher.clear();
		}
	public:
		virtual  ~category_dispatcher()
		{
			clear_listeners();
		}
	};
}
namespace std
{
	template <>
	struct hash<spiritsaway::utility::event_entry>
	{
		std::size_t operator()(const spiritsaway::utility::event_entry& in_event) const
		{
			return (std::uint64_t(in_event.category) << 32) + in_event.command;
		}
	};
}