#ifndef AUTOMATON_H
#define AUTOMATON_H 1

#include <string>

struct file_not_found_exception 
{
    public:
        file_not_found_exception(const std::string& message);

        const char* what() const noexcept;
    private:
        std::string _M_message;
};

struct invalid_dfa_exception 
{
    public:
        invalid_dfa_exception(const std::string& message);

        const char* what() const noexcept;
    private:
        std::string _M_messsage;
};
class dfa 
{
    public:
        dfa(const std::string& file);

        bool is_valid(const std::string& text);

        ~dfa();

    private:
        void read_file(const std::string& file);

        std::string read_transition(const std::string& line) const;

        bool is_accepting_state() const;

        void transition(char c);
    
    private:
        dfa(const dfa& other);
    
    private:
        //Alphabet
        std::string _M_alphabet;
        //States
        std::string _M_states;
        //Initial states
        std::string _M_init;
        //Accpeting state
        std::string _M_accepting;
        //State transition table
        std::string* _M_transitions;
        //The current state of transition table
        size_t _M_curr_state;
        //The largest state in the transition table
        size_t _M_num_states;
        //Capacity of state transition table
        size_t _M_cap;
};

bool automaton(std::string file, std::string text);

#endif