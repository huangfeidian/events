#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <memory>
namespace spiritsaway::event_util
{

	class event_data_wrapper
	{
	public:
		template <typename K>
		event_data_wrapper(const K& data, std::uint32_t data_type_id)
			: data_type(data_type_id)
			, data_ptr(&data)
		{

		}
		std::uint32_t data_type;
		const void* data_ptr;
	};
	template <typename T>
	class listen_handler
	{
	public:
		listen_handler(std::uint32_t handler_id, std::uint32_t event_id, std::uint32_t event_type_id)
			:handler_idx(handler_id)
			, event_type_idx(event_type_id)
			, event_idx(event_id)
		{

		}
		std::uint32_t handler_idx;
		std::uint32_t event_type_idx;
		std::uint32_t event_idx;

		void reset()
		{
			handler_idx = 0;
			event_type_idx = 0;
			event_idx = 0;
		}
	};

	template <typename K>
	class dispatcher_impl
	{
	private:
		struct event_desc
		{
			std::uint16_t event_id;
			bool during_dispatch = false; // to stop recursive dispatch
		};
		std::unordered_map<K, event_desc> event_descs; // map every event to one index
		std::vector< std::unordered_map<std::uint16_t, std::unordered_set<std::uint32_t>>> event_id_to_callbacks;// event callbacks for event_id

		std::unordered_map< std::uint32_t, std::unique_ptr<std::function<void(const K&, const event_data_wrapper&)>>> handler_to_callbacks;
		std::vector<std::uint32_t> recycle_callback_idxes;
		std::uint32_t next_callback_idxes;
	public:
		dispatcher_impl()
		{
			event_id_to_callbacks.push_back({});
		}
	private:
		std::uint32_t get_next_callback_idx()
		{
			if (recycle_callback_idxes.empty())
			{
				return next_callback_idxes++;
			}
			auto result = recycle_callback_idxes.back();
			recycle_callback_idxes.pop_back();
			return result;
		}
		void invoke_callback(std::uint32_t callback_idx, const K& event, const event_data_wrapper& event_data)
		{
			auto cur_callback_iter = handler_to_callbacks.find(callback_idx);
			if (cur_callback_iter == handler_to_callbacks.end())
			{
				return;
			}
			cur_callback_iter->second->operator()(event, event_data);
		}
	public:
		template <typename V>
		void dispatch(const K& event, const V& data, std::uint32_t cur_data_type_id)
		{

			auto cur_event_desc_iter = event_descs.find(event);
			if (cur_event_desc_iter == event_descs.end())
			{
				// this event is not registered
				return;
			}
			if (cur_event_desc_iter->second.during_dispatch)
			{
				// recursive dispatch
				return;
			}
			auto cur_event_id = cur_event_desc_iter->second.event_id;
			const auto& cur_event_callbacks = event_id_to_callbacks[cur_event_id];

			auto cur_event_callback_iter = cur_event_callbacks.find(cur_data_type_id);
			if (cur_event_callback_iter == cur_event_callbacks.end())
			{
				return;
			}

			cur_event_desc_iter->second.during_dispatch = true;
			std::vector<std::uint32_t> cur_callbacks = std::vector<std::uint32_t>(cur_event_callback_iter->second.begin(), cur_event_callback_iter->second.end());
			auto cur_data_wrapper = event_data_wrapper(data, cur_data_type_id);
			for (auto one_callback : cur_callbacks)
			{
				invoke_callback(one_callback, event, cur_data_wrapper);
			}
			cur_event_desc_iter->second.during_dispatch = false;
		}
		template <typename V>
		listen_handler<K> add_listener(const K& event, std::function<void(const K&, const V&)> cur_callback, std::uint32_t cur_event_type_idx)
		{

			auto cur_callback_idx = get_next_callback_idx();
			auto temp_lambda = [=](const K& event, const event_data_wrapper& data)
			{
				if (data.data_type != cur_event_type_idx)
				{
					return;
				}
				return cur_callback(event, *reinterpret_cast<const V*>(data.data_ptr));
			};
			handler_to_callbacks[cur_callback_idx] = std::make_unique< std::function<void(const K&, const event_data_wrapper&)>>(temp_lambda);
			std::uint16_t cur_event_id = 0;
			auto event_id_iter = event_descs.find(event);
			if (event_id_iter == event_descs.end())
			{
				cur_event_id = event_id_to_callbacks.size();
				event_descs[event] = event_desc{ cur_event_id, false };
				event_id_to_callbacks.push_back({});
			}
			else
			{
				cur_event_id = event_id_iter->second.event_id;
			}
			event_id_to_callbacks[cur_event_id][cur_event_type_idx].insert(cur_callback_idx);
			return listen_handler<K>{ cur_callback_idx, cur_event_type_idx, cur_event_id};
		}
		void remove_listener(listen_handler<K> cur_event_handler)
		{
			auto cur_iter = handler_to_callbacks.find(cur_event_handler.handler_idx);
			if (cur_iter == handler_to_callbacks.end())
			{
				return;
			}
			handler_to_callbacks.erase(cur_iter);
			if (cur_event_handler.event_idx >= event_id_to_callbacks.size())
			{
				return;
			}
			event_id_to_callbacks[cur_event_handler.event_idx][cur_event_handler.event_type_idx].erase(cur_event_handler.handler_idx);
			recycle_callback_idxes.push_back(cur_event_handler.handler_idx);
		}



	};
	template <typename... args>
	class dispatcher
	{
	private:
		std::tuple<dispatcher_impl<args>...> dispatcher_impls;
		std::uint32_t last_type_id = 0;
	private:
		template <typename K>
		dispatcher_impl<K>& dispatcher_for()
		{
			return std::get<dispatcher_impl<K>>(dispatcher_impls);
		}
		template <class K>
		std::uint32_t get_type_id()
		{
			static const std::uint32_t id = last_type_id++;
			return id;
		}

	public:
		dispatcher()
		{

		}
		template <typename K, typename V>
		listen_handler<K> add_listener(const K& event, std::function<void(const K&, const V&)> cur_callback)
		{
			return dispatcher_for<K>().add_listener(event, cur_callback, get_type_id<V>());
		}
		template <typename K, typename V>
		listen_handler<K> add_listener(const K& event, void(*cur_callback)(const K&, const V&))
		{
			std::function<void(const K&, const V&)> temp_func(cur_callback);
			return dispatcher_for<K>().add_listener(event, temp_func, get_type_id<V>());
		}
		template <typename K, typename V, typename T>
		listen_handler<K> add_listener(const K& event, void(T::*cur_callback)(const K&, const V&), T* self)
		{
			auto temp_lambda = [=](const K& key, const V& data)
			{
				return (self->*cur_callback)(key, data);
			};
			std::function<void(const K&, const V&)> temp_func(temp_lambda);
			return dispatcher_for<K>().add_listener(event, temp_func, get_type_id<V>());
		}

		template <typename K>
		void remove_listener(listen_handler<K>& handler)
		{
			dispatcher_for<K>().remove_listener(handler);
			handler.reset();
		}
		template <typename K, typename V>
		void dispatch(const K& event, const V& data)
		{
			return dispatcher_for<K>().dispatch(event, data, get_type_id<V>());
		}
		
	};
}
