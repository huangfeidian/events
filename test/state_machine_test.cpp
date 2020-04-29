#include "state_machine.h"
using namespace spiritsaway::event_util;
using namespace std;
enum class fsm_event_t
{
	event_set = 0,
	event_reset
};
struct event_set
{
	int value;
};
struct event_reset
{

};


class idle_state : public state< fsm_event_t>
{

public:
	using parent = state< fsm_event_t>;
	using parent::parent;
	void on_create()
	{
		_dispatcher.add_listener<fsm_event_t, event_set>(fsm_event_t::event_set, &idle_state::on_event, this);
	}
	void on_enter()
	{
		notify_owner(fsm_event_t::event_reset, event_reset{});
	}
	std::string name() const override
	{
		return "idle";
	}
	void on_event(const fsm_event_t& event, const event_set& data)
	{
		notify_owner(event, data);
		change_to("busy");
	}

};

class busy_state : public state< fsm_event_t>
{

public:
	using parent = state< fsm_event_t>;
	using parent::parent;
	void on_create()
	{
		_dispatcher.add_listener(fsm_event_t::event_reset, &busy_state::on_reset_event, this);
		_dispatcher.add_listener(fsm_event_t::event_set, &busy_state::on_set_event, this);

	}

	std::string name() const override
	{
		return "busy";
	}
	void on_set_event(const fsm_event_t& event, const event_set& data)
	{
		notify_owner(event, data);
	}
	void on_reset_event(const fsm_event_t& event, const event_reset& data)
	{
		change_to("idle");
	}

};
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