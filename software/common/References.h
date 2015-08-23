#ifndef REFERENCES_H_
#define REFERENCES_H_

#include "Export.h"

class References {
public:
	SOUNDGLOVE_API References();

	SOUNDGLOVE_API bool any() const;

	SOUNDGLOVE_API void acquire();
	SOUNDGLOVE_API void release();

private:
	int count_;
};

#endif /* REFERENCES_H_ */
