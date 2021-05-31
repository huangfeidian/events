#include <state_machine.h>
#include <cstdlib>
#include <chrono>
using namespace spiritsaway::utility::events;

struct event_data
{
};
struct statem_owner;
enum class fsm_event_t
{
    e1,
    e2,
    e3,
    e4,
    e5,
    e6,
    e7,
    e8,
    e9,
    e10,
    e11,
    e12,
    e13,
    e14,
    e15,
    e16,
    e17,
    e18,
    e19,
    e20,
    e21,
    e22,
    e23,
    e24,
    e25,
    e26,
    e27,
    e28,
    e29,
    e30,
    e31,
    e32,
    e33,
    e34,
    e35,
    e36,
    e37,
    e38,
    e39,
    e40,
    e41,
    e42,
    e43,
    e44,
    e45,
    e46,
    e47,
    e48,
    e49,
    e50,
};
#define STATE(A, B)                                                                                      \
    class s##A : public state<statem_owner, fsm_event_t>                                                               \
    {                                                                                                    \
    public:                                                                                              \
        using parent = state<statem_owner, fsm_event_t>;                                                 \
        using parent::parent;                                                                            \
        void on_create()                                                                                 \
        {                                                                                                \
            m_dispatcher.add_listener<fsm_event_t, event_data>(fsm_event_t::e##A, &s##A::on_event, this); \
        }                                                                                                \
        void on_event(const fsm_event_t &event, const event_data &data)                                  \
        {                                                                                                \
            change_to("s" #B);                                                                           \
        }                                                                                                \
        std::string name() const override                                                                \
        {                                                                                                \
            return "s" #A;                                                                               \
        }                                                                                                \
        static std::string static_name()                                                                 \
        {                                                                                                \
            return "s" #A;                                                                               \
        }                                                                                                \
    };

STATE(1, 2)
STATE(2, 3)
STATE(3, 4)
STATE(4, 5)
STATE(5, 6)
STATE(6, 7)
STATE(7, 8)
STATE(8, 9)
STATE(9, 10)
STATE(10, 11)

STATE(11, 12)
STATE(12, 13)
STATE(13, 14)
STATE(14, 15)
STATE(15, 16)
STATE(16, 17)
STATE(17, 18)
STATE(18, 19)
STATE(19, 20)
STATE(20, 21)

STATE(21, 22)
STATE(22, 23)
STATE(23, 24)
STATE(24, 25)
STATE(25, 26)
STATE(26, 27)
STATE(27, 28)
STATE(28, 29)
STATE(29, 30)
STATE(30, 31)

STATE(31, 32)
STATE(32, 33)
STATE(33, 34)
STATE(34, 35)
STATE(35, 36)
STATE(36, 37)
STATE(37, 38)
STATE(38, 39)
STATE(39, 40)
STATE(40, 41)

STATE(41, 2)
STATE(42, 3)
STATE(43, 4)
STATE(44, 5)
STATE(45, 6)
STATE(46, 7)
STATE(47, 8)
STATE(48, 9)
STATE(49, 50)
STATE(50, 1)

class test_fsm : public state_machine<statem_owner, fsm_event_t>
{
public:
    test_fsm(statem_owner* in_owner)
		: state_machine<statem_owner, fsm_event_t>("s1", in_owner)
    {
        add_state<s1>();
        add_state<s2>();
        add_state<s3>();
        add_state<s4>();
        add_state<s5>();
        add_state<s6>();
        add_state<s7>();
        add_state<s8>();
        add_state<s9>();
        add_state<s10>();

        add_state<s11>();
        add_state<s12>();
        add_state<s13>();
        add_state<s14>();
        add_state<s15>();
        add_state<s16>();
        add_state<s17>();
        add_state<s18>();
        add_state<s19>();
        add_state<s20>();

        add_state<s21>();
        add_state<s22>();
        add_state<s23>();
        add_state<s24>();
        add_state<s25>();
        add_state<s26>();
        add_state<s27>();
        add_state<s28>();
        add_state<s29>();
        add_state<s30>();

        add_state<s31>();
        add_state<s32>();
        add_state<s33>();
        add_state<s34>();
        add_state<s35>();
        add_state<s36>();
        add_state<s37>();
        add_state<s38>();
        add_state<s39>();
        add_state<s40>();

        add_state<s41>();
        add_state<s42>();
        add_state<s43>();
        add_state<s44>();
        add_state<s45>();
        add_state<s46>();
        add_state<s47>();
        add_state<s48>();
        add_state<s49>();
        add_state<s50>();

        change_to("s1");
    }
};
struct statem_owner
{
    test_fsm statem;
    statem_owner()
        : statem(this)
    {

    }
};
int main()
{
    auto start = std::chrono::steady_clock::now();
    statem_owner the_owner;
    auto& sm = the_owner.statem;
    for (int i = 0; i < 1000 * 1000; i++)
    {
        if (rand() % 2)
            sm.process_event(fsm_event_t::e1, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e2, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e3, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e4, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e5, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e6, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e7, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e8, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e9, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e10, event_data{});

        if (rand() % 2)
            sm.process_event(fsm_event_t::e11, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e12, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e13, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e14, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e15, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e16, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e17, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e18, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e19, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e20, event_data{});

        if (rand() % 2)
            sm.process_event(fsm_event_t::e21, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e22, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e23, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e24, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e25, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e26, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e27, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e28, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e29, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e30, event_data{});

        if (rand() % 2)
            sm.process_event(fsm_event_t::e31, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e32, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e33, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e34, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e35, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e36, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e37, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e38, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e39, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e40, event_data{});

        if (rand() % 2)
            sm.process_event(fsm_event_t::e41, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e42, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e43, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e44, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e45, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e46, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e47, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e48, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e49, event_data{});
        if (rand() % 2)
            sm.process_event(fsm_event_t::e50, event_data{});
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() * 1000 << "Ms\n";
}
