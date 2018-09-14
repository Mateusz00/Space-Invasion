#ifndef COMMANDQUEUE_HPP
#define COMMANDQUEUE_HPP

#include <queue>
#include "Command.hpp"

class CommandQueue
{
    public:
        void     push(const Command&);
        Command	 pop();
        bool     isEmpty() const;

    private:
        std::queue<Command> mQueue;
};

#endif // COMMANDQUEUE_HPP
