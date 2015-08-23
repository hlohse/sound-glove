#ifndef FLEX_H_
#define FLEX_H_

#include <stdint.h>

class Flex {
public:
	static Flex& instance();

	uint16_t flex() const;

private:
	Flex();

    // Disallow copies; (do not implement!)
	Flex(const Flex&);
    void operator=(const Flex&);

};

#endif /* FLEX_H_ */
