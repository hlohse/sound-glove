#include "HeadingAnswer.h"
#include "Platform.h"

HeadingAnswer::HeadingAnswer()
:	Command(Command::type_heading_answer)
{
}

void HeadingAnswer::execute(Context& context)
{
#ifdef PLATFORM_DESKTOP
	context.glove->initializeHeading(a, m);
#endif
}

void HeadingAnswer::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)a[0].values, sizeof(a[0].values));
	serializer.serialize((uint8_t*)a[1].values, sizeof(a[1].values));
	serializer.serialize((uint8_t*)a[2].values, sizeof(a[2].values));
	serializer.serialize((uint8_t*)m[0].values, sizeof(m[0].values));
	serializer.serialize((uint8_t*)m[1].values, sizeof(m[1].values));
	serializer.serialize((uint8_t*)m[2].values, sizeof(m[2].values));
}

void HeadingAnswer::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*) a[0].values, sizeof(a[0].values));
	deserializer.deserialize((uint8_t*) a[1].values, sizeof(a[1].values));
	deserializer.deserialize((uint8_t*) a[2].values, sizeof(a[2].values));
	deserializer.deserialize((uint8_t*) m[0].values, sizeof(m[0].values));
	deserializer.deserialize((uint8_t*) m[1].values, sizeof(m[1].values));
	deserializer.deserialize((uint8_t*) m[2].values, sizeof(m[2].values));
}

int HeadingAnswer::maxDataSize() const
{
	return max_data_size;
}
