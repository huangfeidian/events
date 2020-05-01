# dispatcher and state machine in head only c++ 17

A simple event dispatcher and for c++ 17, a state_machine is also built on the dispatcher


# dependency
None

# dispatcher interface

1. create dispatcher `dispatcher<int, std::string> cur_dispatcher;` the template argument represent for the event type the dispatcher recieve
2. add listener `auto handler_3 = cur_dispatcher.add_listener(std::string("hehe"), &callback_3);` register a event listener for specific event, return a handler for the use of remove.
3. remove listener `cur_dispatcher.remove_listener(handler_5);` user should provide correct handler
4. dispatch event `cur_dispatcher.dispatch(std::string("hehe"), string("hehe"));`, two argument, the first argument is the event, the second argument is the detail for current event, remove/add listener is allowed when dispatching. recursive dispatch is detected to stop.

# dispatcher example

```c++
void callback_1(const int& key, const int& value)
{
	cout << "callback_1 key " << key << " value " << value << endl;
}
void callback_2(int key, std::string value)
{
	cout << "callback_2 key " << key << " value " << value << endl;
}
void callback_3(const std::string& key, const std::string& value)
{
	cout << "callback_3 key " << key << " value " << value << endl;
}
void callback_4(std::string key, std::string value)
{
	cout << "callback_4 key " << key << " value " << value << endl;
}
class temp_a
{
public:
	void callback_5(const std::string& key, const std::string& value)
	{
		cout << "callback_5 key " << key << " value " << value << endl;

	}
};
int main()
{
	dispatcher<int, std::string> cur_dispatcher;
	temp_a a;
	auto handler_1 = cur_dispatcher.add_listener(1, &callback_1);
	auto handler_2 = cur_dispatcher.add_listener<int, string>(2, &callback_2);
	auto handler_3 = cur_dispatcher.add_listener(std::string("hehe"), &callback_3);
	auto handler_4 = cur_dispatcher.add_listener<string, string>(std::string("1"), &callback_4);
	auto handler_5 = cur_dispatcher.add_listener(std::string("1"), &temp_a::callback_5, &a);


	cur_dispatcher.dispatch(1, 2);
	cur_dispatcher.dispatch(2, string("hehe"));
	cur_dispatcher.dispatch(std::string("1"), string("hehe"));
	cur_dispatcher.dispatch(std::string("hehe"), string("hehe"));

	cur_dispatcher.remove_listener(handler_5);
	cur_dispatcher.remove_listener(handler_4);
	cout << "after remove listener" << endl;
	cur_dispatcher.dispatch(1, 2);
	cur_dispatcher.dispatch(2, string("hehe"));
	cur_dispatcher.dispatch(std::string("1"), string("hehe"));
	cur_dispatcher.dispatch(std::string("hehe"), string("hehe"));

	cur_dispatcher.remove_listener(handler_5);
	cur_dispatcher.remove_listener(handler_4);

}
```

## state machine interface

for every state, we define seven interface:
1. `virtual std::string name() const ` the name of current state , which should not be duplicated with in same state machine
2. `virtual void on_create()` this interface is called after this state is added to state machine
3. `virtual void on_enter()` this interface is called when state machine enter this state
4. `virtual void on_exit()` this inteface is called when state machine exit this state
5. `void process_event(const K& event, const T& data)` the state receive information from state machine
6. `void notify_owner(const K& event, const T& data)` the state send information to state machine
7. `bool change_to(const std::string& dest_state)` notify owner to change to another state

for event state machine, we define four interfaces
1. `bool change_to(const std::string& state_name)` force change to new state
2. `void process_event(const K& event, const V& data)` process information, which just call current state `process_event`
3. `void local_dispatch(const K& event, const V& data)` local dispatch, receive information for state

## state machine example

```c++
class test_fsm : public state_machine<fsm_event_t>
{
protected:
	int value = 0;

public:
	test_fsm()
		: state_machine< fsm_event_t>("idle")
	{
		add_state< idle_state>();
		add_state<busy_state>();
		_dispatcher.add_listener(fsm_event_t::event_reset, &test_fsm::on_reset_event, this);
		_dispatcher.add_listener(fsm_event_t::event_set, &test_fsm::on_set_event, this);
		change_to("idle");
	}
	void on_set_event(const fsm_event_t& event, const event_set& data)
	{
		value = data.value;
		cout<<"new value "<<value<<endl;
	}
	void on_reset_event(const fsm_event_t& event, const event_reset& data)
	{
		value = 0;
		cout<<"reset value to "<<value<<endl;
	}
};


int main()
{
	test_fsm cur_fsm;
	cur_fsm.process_event(fsm_event_t::event_reset, event_reset{});
	cur_fsm.process_event(fsm_event_t::event_set, event_set{ 1 });
	cur_fsm.process_event(fsm_event_t::event_set, event_set{ 3 });

	cur_fsm.process_event(fsm_event_t::event_reset, event_reset{});
	cur_fsm.process_event(fsm_event_t::event_set, event_set{ 2});
	cur_fsm.reset();
}
```

