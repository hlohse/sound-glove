#include "Calibration.h"

Calibration::Calibration()
:	has_inverted_placement(false),
	g_offset(),
	g_scale(),
	a_offset(),
	a_scale(),
	m_offset(),
	m_rotation(),
	temperature(0)
{
}


Calibration::Calibration(const uint16_t has_inverted_placement,
	       const Vector<int16_t>& g_offset,
	       const Vector<float>& g_scale,
	       const Vector<int16_t>& a_offset,
	       const Vector<float>& a_scale,
	       const Vector<int16_t>& m_offset,
	       const Matrix& m_rotation,
	       const int16_t temperature)
:	has_inverted_placement(has_inverted_placement),
	g_offset(g_offset),
	g_scale(g_scale),
	a_offset(a_offset),
	a_scale(a_scale),
	m_offset(m_offset),
	m_rotation(m_rotation),
	temperature(temperature)
{
}

bool Calibration::hasInvertedPlacement() const
{
	return has_inverted_placement != 0;
}
