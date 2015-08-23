#ifndef ISERIALIZEABLE_H_
#define ISERIALIZEABLE_H_

#include "Serialization.h"
#include "Context.h"

class ISerializeable {
public:
    virtual Serialization serialize(Context& context) const = 0;
	virtual void deserialize(Serialization& serialization, Context& context) = 0;
};

#endif /* ISERIALIZEABLE_H_ */
