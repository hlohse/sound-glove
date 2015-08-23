#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "Export.h"
#include "Vector.h"
#include "Matrix.h"
#include <stdint.h>

#pragma pack(push)
#pragma pack(1)

struct Calibration {
	// Unaligned total size including each member
	static const int serialized_size = 82;

	uint16_t has_inverted_placement;
	Vector<int16_t> g_offset;
	Vector<float> g_scale;
	Vector<int16_t> a_offset;
	Vector<float> a_scale;
	Vector<int16_t> m_offset;
	Matrix m_rotation;
	int16_t temperature;

	SOUNDGLOVE_API Calibration();
	Calibration(const uint16_t has_inverted_placement,
		 const Vector<int16_t>& g_offset,
		 const Vector<float>& g_scale,
		 const Vector<int16_t>& a_offset,
		 const Vector<float>& a_scale,
		 const Vector<int16_t>& m_offset,
		 const Matrix& m_rotation,
		 const int16_t temperature);

	bool hasInvertedPlacement() const;
};

#pragma pack(pop)

#endif /* CALIBRATION_H_ */
