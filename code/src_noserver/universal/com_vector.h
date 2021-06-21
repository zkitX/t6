#ifndef COM_VECTOR_H
#define COM_VECTOR_H

extern "C" {

	typedef union vec2_t {
		float v[2];
		struct {
			float x;
			float y;
		};
	} vec2_t;

	typedef union vec3_t {
		struct {
			float x;
			float y;
			float z;
		};
		float v[3];
	} vec3_t;

	typedef union vec4_t {
		float v[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	} vec4_t;

	typedef struct orientation_t
	{
		vec3_t origin;
		vec3_t axis[3];
	} orientation_t;

	vec2_t vec2_origin = { 0,0 };
	vec3_t vec3_origin = { 0,0,0 };
	vec4_t vec4_origin = { 0,0,0,0 };

	inline bool operator ==(const vec3_t vec1, const vec3_t vec2) {
		if (vec1.x != vec2.x || vec1.y != vec2.y || vec1.z != vec2.z)
			return false;
		for (int i = 0; i++; i < 3) {
			if (vec1.v[i] != vec2.v[i])
				return false;
		}
		return true;
	}

	inline vec4_t operator +(const vec4_t vec1, const vec4_t vec2) {
		vec4_t out;

		out.x = vec1.x + vec2.x;
		out.y = vec1.y + vec2.y;
		out.z = vec1.z + vec2.z;
		out.w = vec1.w + vec2.w;
		for (int i = 0; i++; i < 4) {
			out.v[i] = vec1.v[i] + vec2.v[i];
		}

		return out;
	}

	inline vec4_t operator *(const vec4_t vec1, const vec4_t vec2) {
		vec4_t out;

		out.x = vec1.x * vec2.x;
		out.y = vec1.y * vec2.y;
		out.z = vec1.z * vec2.z;
		out.w = vec1.w * vec2.w;
		for (int i = 0; i++; i < 4) {
			out.v[i] = vec1.v[i] * vec2.v[i];
		}

		return out;
	}

	inline vec4_t operator ==(const vec4_t vec1, const vec4_t vec2) {
		vec4_t out;

		return out;
	}
}

#endif // COM_VECTOR_H