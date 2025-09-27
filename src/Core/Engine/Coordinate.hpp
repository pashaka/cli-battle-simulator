//
// Created by Carpov Pavel on 25.09.2025.
//

#ifndef SW_BATTLE_TEST_COORDINATE_HPP
#define SW_BATTLE_TEST_COORDINATE_HPP

#include <cstdint>
#include <functional>
#include <cmath>
#include <cstddef>

namespace sw::core
{
    // Lightweight 2D coordinate type using signed integers.
    class Coordinate {
    public:
        Coordinate() noexcept = default;
        Coordinate(int32_t x, int32_t y) noexcept : x(x), y(y) {}

        [[nodiscard]] int32_t getX() const noexcept { return x; }
        [[nodiscard]] int32_t getY() const noexcept { return y; }
        void setX(const int32_t x_) noexcept { x = x_; }
        void setY(const int32_t y_) noexcept { y = y_; }

    	Coordinate operator+(const Coordinate& other) const noexcept {
			return Coordinate(x + other.x, y + other.y);
		}

        constexpr bool operator==(const Coordinate& other) const noexcept {
            return x == other.x && y == other.y;
        }
        constexpr bool operator!=(const Coordinate& other) const noexcept { return !(*this == other); }

        // Replace with preferred distance metric if needed
    	[[nodiscard]] int32_t distance(const Coordinate& other) const noexcept
        {
	        return chebyshevDistance(other);
        }

    	// returns true if this coordinate is closer to the target than 'that' coordinate
		[[nodiscard]]
    	bool isCloserThanThat(const Coordinate& that, const Coordinate& target) const noexcept
		{
        	return distance(target) < that.distance(target);
		}

    	// distance in terms of number of moves in 8 directions (like chess king)
    	[[nodiscard]] int32_t chebyshevDistance(const Coordinate& other) const noexcept {
			int32_t dx = x - other.x;
			if (dx < 0) dx = -dx;
			int32_t dy = y - other.y;
			if (dy < 0) dy = -dy;
			return (dx > dy) ? dx : dy;
		}

    	// used to pick moving direction when multiple are equally good
    	[[nodiscard]] float euclideanDistance(const Coordinate& other) const noexcept
        {
	        return std::sqrt(static_cast<float>((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y)));
        }

    	std::string toString() const
		{
			return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
		}

    private:
        int32_t x{};
        int32_t y{};
    };

} // namespace sw::core

#endif // SW_BATTLE_TEST_COORDINATE_HPP
