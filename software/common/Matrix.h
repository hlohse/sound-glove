#ifndef MATRIX_H_
#define MATRIX_H_

// Use defines to avoid VLA memory pool allocations
#define MATRIX_ROWS    3
#define MATRIX_COLUMNS 3

#include "Export.h"
#include "Platform.h"

/* Ignore "warning C4351: new behaviour: elements of array 'name' will be
* default initialized".
*/
#ifdef OS_WINDOWS
#pragma warning( push )
#pragma warning( disable : 4351 )
#endif


struct Matrix {
	float values[MATRIX_ROWS * MATRIX_COLUMNS];

SOUNDGLOVE_API Matrix()
:	values()
{
}

SOUNDGLOVE_API float get(const int row, const int column) const
{
	if (row < 0 || row >= MATRIX_ROWS || column < 0 || column >= MATRIX_COLUMNS) {
		return 0;
	}
	return values[row * MATRIX_ROWS + column];
}

SOUNDGLOVE_API void set(const int row, const int column, const float value)
{
	if (!(row < 0 || row >= MATRIX_ROWS || column < 0 || column >= MATRIX_COLUMNS)) {
		values[row * MATRIX_ROWS + column] = value;
	}
}

};

#endif /* MATRIX_H_ */
