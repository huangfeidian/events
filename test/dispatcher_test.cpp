
#include <iostream>
#include <dispatcher.h>
using namespace std;
using namespace spiritsaway::utility::events;

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

	std::cout << "begin typed dispatcher" << std::endl;
	typed_dispatcher<std::string, std::string> cur_typed_dispatcher;
	auto typed_handler_1 = cur_typed_dispatcher.add_listener(&temp_a::callback_5, &a);
	auto typed_handler_2 = cur_typed_dispatcher.add_listener(&callback_4);
	auto typed_handler_3 = cur_typed_dispatcher.add_listener(&callback_3);
	cur_typed_dispatcher.dispatch(std::string("1"), string("hehe"));

	std::cout << "typed dispatch finish" << std::endl;
	cur_typed_dispatcher.remove_listener(typed_handler_2);
	cur_typed_dispatcher.dispatch(std::string("1"), string("hehe"));
	return 1;
}

