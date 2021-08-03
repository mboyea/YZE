#ifndef RANDOM_H
#define RANDOM_H
#pragma once

namespace Math {
	// return a random int between min and max
	// re-seeded every millisecond
	int Random(int min, int max);
}

#endif // !RANDOM_H