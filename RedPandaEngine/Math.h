#pragma once
namespace Math {
	template<typename T>
	struct Vec2 {
		Vec2() {
			x = 0;
			y - 0;
		}
		Vec2(T _x, T _y) {
			x = _x;
			y - _y;
		}
		T x, y;
	};
	template<typename T>
	struct Vec3 {
		Vec3() {
			x = 0;
			y - 0;
			z = 0;
		}
		Vec3(T _x, T _y, T _z) {
			x = _x;
			y - _y;
			z = _z;
		}
		T x, y,z;
	};
	template<typename T>
	struct Vec4 {
		T x, y,z,w;
	};
}