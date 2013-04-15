#ifndef KINTEX_GROUP_H
#define KINTEX_GROUP_H

#include <stack>

namespace kintex{  
	class Processor;
	
	/* Normal group, with poll mechanism */
    class Group{
    public:
		/* Method that is polled in current group, to do special actions */
		virtual bool poll(Processor &p) = 0;
		virtual ~Group() {}
	};
	
	/* Special group used to provide reading of statements */
	class StatementGroup: public Group{
    public:
        /* Default constructor */
        StatementGroup() {}
		/* Check if end of statement is reached */
		virtual void before(Processor &p) = 0;
		virtual void after(Processor &p) = 0;

		~StatementGroup(){}
    };
}

#endif
