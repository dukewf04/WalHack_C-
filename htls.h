#pragma once

#pragma region xSDK
#ifdef _DEBUG
#define ILINE _inline
#else
#define ILINE __forceinline
#endif

typedef unsigned int EntityId;

enum type_zero { ZERO };
enum type_min { VMIN };
enum type_max { VMAX };
enum type_identity { IDENTITY };

struct Vec3;
struct Matrix3x3;
class  QuatT;
class  Quat;
struct Matrix3x4;

struct ray_hit { int Unknown001[0x100]; };

struct ray_hit_cached { int Unknown001[0x100]; };
BOOL  bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}
DWORD  FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}
class IPhysicalEntity {
public:
};
enum ESilhouettesParams
{
	eAllMap = 0x10u,
	eLimitDistance = 0xFFFFFFEFu,
	eMin = 0x80000
};

enum GameModes
{
	Mode_Unknown = 0,
	Mode_TeamDeathmatch,
	Mode_Capture,
	Mode_CoopSurvival,
	Mode_PlantTheBomb,
	Mode_Storm,
	Mode_FreeForAll,
	Mode_Destruction,
	Mode_Domination
};
template< typename cData >
cData vFun_Call(PVOID BaseClass, DWORD vOFFSET)
{
	PDWORD* vPointer = (PDWORD*)BaseClass;
	PDWORD vFunction = *vPointer;
	DWORD dwAddress = vFunction[vOFFSET];
	return (cData)(dwAddress);
}

VOID cry_sincos(FLOAT angle, FLOAT* pSin, FLOAT* pCos) {
	*pSin = (sin(angle));
	*pCos = (cos(angle));
}
VOID sincos(FLOAT angle, FLOAT* pSin, FLOAT* pCos) {
	cry_sincos(angle, pSin, pCos);
}

void MEMwrite(void *adr, void *ptr, int size)
{
	DWORD OldProtection;
	VirtualProtect(adr, size, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy(adr, ptr, size);
	VirtualProtect(adr, size, OldProtection, &OldProtection);
}

struct ScreenArgs {
	FLOAT  world_x;
	FLOAT  world_y;
	FLOAT  world_z;
	FLOAT* screen_x;
	FLOAT* screen_y;
	FLOAT* screen_z;
};
struct Vec3 {
	Vec3(type_zero) : x(0), y(0), z(0) {}
	Vec3()
	{
		Vec3(0.f, 0.f, 0.f);
	}
	Vec3(FLOAT x, FLOAT y, FLOAT z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	FLOAT     length() const { return sqrt(x*x + y * y + z * z); }
	FLOAT     Distance(Vec3 &VecB)
	{
		Vec3 Out = *this - VecB;
		return sqrt(Out.x*Out.x + Out.y*Out.y + Out.z*Out.z);
	}
	Vec3 PZD(Vec3 &VecB)
	{
		Vec3 Out = *this - VecB;
		return Out / sqrt(Out.x*Out.x + Out.y*Out.y + Out.z*Out.z);
	}

	Vec3& normalize()
	{
		FLOAT len2 = x * x + y * y + z * z;
		if (len2 > 1e-20f)
		{
			FLOAT rlen = (float)1.0 / sqrt(len2);
			x *= rlen; y *= rlen; z *= rlen;
		}
		else { x = 0; y = 0; z = 1; }
		return *this;

	}
	Vec3&    FastNormalize()
	{
		FLOAT l = length();
		l = 1.f / l;
		this->x *= l;
		this->y *= l;
		this->z *= l;
		return *this;
	}
	Vec3 operator / (FLOAT fValue) { return Vec3(x / fValue, y / fValue, z / fValue); }
	Vec3&  operator /= (FLOAT fValue)
	{
		*this = *this / fValue;
		return *this;
	}
	Vec3   operator /  (FLOAT fValue) const
	{
		Vec3 vOut;
		vOut.x = this->x / fValue;
		vOut.y = this->y / fValue;
		vOut.z = this->z / fValue;
		return vOut;
	}
	BOOL      operator != (Vec3 &vec)
	{
		return !(vec.x != x || vec.y != y || vec.z != z);
	}
	Vec3   operator /  (Vec3 &vec) const
	{
		return Vec3(vec.x / x, vec.y / y, vec.z / z);
	}
	Vec3   operator +  (Vec3 &vec) const
	{
		return Vec3(vec.x + x, vec.y + y, vec.z + z);
	}
	Vec3   operator -  (Vec3 &vec) const
	{
		return Vec3(vec.x - x, vec.y - y, vec.z - z);
	}

	FLOAT x, y, z;
};

struct Matrix3x3 {
	Matrix3x3(const Vec3& vx, const Vec3& vy, const Vec3& vz)
	{
		m00 = (vx.x);
		m01 = (vy.x);
		m02 = (vz.x);

		m10 = (vx.y);
		m11 = (vy.y);
		m12 = (vz.y);

		m20 = (vx.z);
		m21 = (vy.z);
		m22 = (vz.z);
	}
	Matrix3x3()
	{
		m00 = m01 = m02 = m10 = m11 = m12 = m20 = m21 = m22 = 0.f;
	};
	VOID SetRotationVDir(const Vec3& vdir)
	{

		m00 = 1;
		m01 = 0;
		m02 = 0;
		m10 = 0;
		m11 = 0;
		m12 = -vdir.z;
		m20 = 0;
		m21 = vdir.z;
		m22 = 0;

		FLOAT l = sqrt(vdir.x*vdir.x + vdir.y*vdir.y);
		if (l > 0.0001)
		{
			FLOAT xl = -vdir.x / l;
			FLOAT yl = vdir.y / l;

			m00 = (yl);
			m01 = (vdir.x);
			m02 = (xl*vdir.z);

			m10 = (xl);
			m11 = (vdir.y);
			m12 = (-vdir.z*yl);

			m20 = 0;
			m21 = (vdir.z);
			m22 = (l);
		}
	}

	FLOAT m00, m01, m02;
	FLOAT m10, m11, m12;
	FLOAT m20, m21, m22;
};

class  Quat {
public:
	Quat(const Matrix3x3& m)
	{
		FLOAT s, p, tr = m.m00 + m.m11 + m.m22;
		w = 1, v.x = 0, v.y = 0, v.z = 0;

		if (tr > 0)
			s = sqrt(tr + 1.0f), p = 0.5f / s, w = s * 0.5f, v.x = (m.m21 - m.m12)*p, v.y = (m.m02 - m.m20)*p, v.z = (m.m10 - m.m01)*p;
		else if ((m.m00 >= m.m11) && (m.m00 >= m.m22))
			s = sqrt(m.m00 - m.m11 - m.m22 + 1.0f), p = 0.5f / s, w = (m.m21 - m.m12)*p, v.x = s * 0.5f, v.y = (m.m10 + m.m01)*p, v.z = (m.m20 + m.m02)*p;
		else if ((m.m11 >= m.m00) && (m.m11 >= m.m22))
			s = sqrt(m.m11 - m.m22 - m.m00 + 1.0f), p = 0.5f / s, w = (m.m02 - m.m20)*p, v.x = (m.m01 + m.m10)*p, v.y = s * 0.5f, v.z = (m.m21 + m.m12)*p;
		else if ((m.m22 >= m.m00) && (m.m22 >= m.m11))
			s = sqrt(m.m22 - m.m00 - m.m11 + 1.0f), p = 0.5f / s, w = (m.m10 - m.m01)*p, v.x = (m.m02 + m.m20)*p, v.y = (m.m12 + m.m21)*p, v.z = s * 0.5f;
	}
	Quat() {}

	friend FLOAT operator | (const Quat& q, const Quat& p)
	{
		return (q.v.x*p.v.x + q.v.y*p.v.y + q.v.z*p.v.z + q.w*p.w);
	}
	friend Quat  operator - (const Quat &q, const Quat &p)
	{
		Quat ret;
		ret.w = q.w - p.w;

		ret.v.x = q.v.x - p.v.x;
		ret.v.y = q.v.y - p.v.y;
		ret.v.z = q.v.z - p.v.z;

		return ret;
	}

	VOID        SetRotationVDir(const Vec3& vdir)
	{
		w = (0.70710676908493042f);
		v.x = (vdir.z*0.70710676908493042f);
		v.y = (0.0f);
		v.z = (0.0f);

		FLOAT l = sqrt(vdir.x*vdir.x + vdir.y*vdir.y);

		if (l > (0.00001))
		{
			Vec3 hv;

			hv.x = vdir.x / l;
			hv.y = vdir.y / l + 1.0f;
			hv.z = l + 1.0f;

			FLOAT r = sqrt(hv.x*hv.x + hv.y*hv.y);
			FLOAT s = sqrt(hv.z*hv.z + vdir.z*vdir.z);
			FLOAT hacos0 = 0.0;
			FLOAT hasin0 = -1.0;

			if (r > (0.00001)) { hacos0 = hv.y / r; hasin0 = -hv.x / r; }

			FLOAT hacos1 = hv.z / s;
			FLOAT hasin1 = vdir.z / s;

			w = (hacos0*hacos1);
			v.x = (hacos0*hasin1);
			v.y = (hasin0*hasin1);
			v.z = (hasin0*hacos1);
		}
	}
	static Quat CreateRotationVDir(const Vec3& vdir)
	{
		Quat q;
		q.SetRotationVDir(vdir);
		return q;
	}

	VOID Normalize(VOID)
	{
		FLOAT d = sqrt(w*w + v.x*v.x + v.y*v.y + v.z*v.z);

		w *= d;

		v.x *= d;
		v.y *= d;
		v.z *= d;
	}
	VOID SetNlerp(const Quat &p, const Quat &tq, FLOAT t)
	{
		Quat q = tq;

		//assert(p.IsValid());
		//assert(q.IsValid());

		if ((p | q) < 0)
		{
			FLOAT qx = -q.v.x;
			FLOAT qy = -q.v.y;
			FLOAT qz = -q.v.z;

			q.v.x = qx;
			q.v.y = qy;
			q.v.z = qz;
		}

		v.x = p.v.x*(1.0f - t) + q.v.x*t;
		v.y = p.v.y*(1.0f - t) + q.v.y*t;
		v.z = p.v.z*(1.0f - t) + q.v.z*t;

		w = p.w  *(1.0f - t) + q.w  *t;

		Normalize();
	}
	VOID SetSlerp(const Quat &tp, const Quat &tq, FLOAT t)
	{
		//assert(tp.IsValid());

		Quat p, q;
		p = tp;
		q = tq;
		Quat q2;

		FLOAT cosine = (p | q);

		if (cosine < 0.0f)
		{
			FLOAT qx = -q.v.x;
			FLOAT qy = -q.v.y;
			FLOAT qz = -q.v.z;

			cosine = -cosine;

			q.v.x = qx;
			q.v.y = qy;
			q.v.z = qz;
		}

		if (cosine > 0.9999f)
		{
			SetNlerp(p, q, t);
			return;
		}

		q2.w = q.w - p.w*cosine;
		q2.v.x = q.v.x - p.v.x*cosine;
		q2.v.y = q.v.y - p.v.y*cosine;
		q2.v.z = q.v.z - p.v.z*cosine;

		FLOAT sine = sqrt(q2 | q2);
		FLOAT s, c;

		sincos(atan2(sine, cosine)*t, &s, &c);

		w = (p.w*c + q2.w*s / sine);
		v.x = (p.v.x*c + q2.v.x*s / sine);
		v.y = (p.v.y*c + q2.v.y*s / sine);
		v.z = (p.v.z*c + q2.v.z*s / sine);
	}

	static Quat CreateSlerp(const Quat &p, const Quat &tq, FLOAT t)
	{
		Quat d;
		d.SetSlerp(p, tq, t);
		return d;
	}



	Vec3 v;
	FLOAT w;
};
class  QuatT {
public:
	QuatT() {}

	Quat q;
	Vec3 t;
};
struct Matrix3x4 {
	Matrix3x4() {};
	Matrix3x4(const QuatT& q)
	{
		Vec3 v2;
		v2.x = (q.q.v.x) + (q.q.v.x);

		FLOAT  xx = 1 - v2.x*q.q.v.x;
		FLOAT  yy = v2.y*q.q.v.y;
		FLOAT  xw = v2.x*q.q.w;
		FLOAT  xy = v2.y*q.q.v.x;
		FLOAT  yz = v2.z*q.q.v.y;
		FLOAT  yw = v2.y*q.q.w;
		FLOAT  xz = v2.z*q.q.v.x;
		FLOAT  zz = v2.z*q.q.v.z;
		FLOAT  zw = v2.z*q.q.w;
		m00 = FLOAT(1 - yy - zz);
		m01 = FLOAT(xy - zw);
		m02 = FLOAT(xz + yw);
		m03 = FLOAT(q.t.x);
		m10 = FLOAT(xy + zw);
		m11 = FLOAT(xx - zz);
		m12 = FLOAT(yz - xw);
		m13 = FLOAT(q.t.y);
		m20 = FLOAT(xz - yw);
		m21 = FLOAT(yz + xw);
		m22 = FLOAT(xx - yy);
		m23 = FLOAT(q.t.z);
	}

	inline Vec3        GetTranslation()
	{
		Vec3 mf;
		mf.x = m03;
		mf.y = m13;
		mf.z = m23;
		return Vec3(mf);
	}
	friend Matrix3x4 operator * (const Matrix3x4& l, const Matrix3x4& r)
	{
		Matrix3x4 m = r;
		m.m00 = l.m00*r.m00 + l.m01*r.m10 + l.m02*r.m20;
		m.m10 = l.m10*r.m00 + l.m11*r.m10 + l.m12*r.m20;
		m.m20 = l.m20*r.m00 + l.m21*r.m10 + l.m22*r.m20;
		m.m01 = l.m00*r.m01 + l.m01*r.m11 + l.m02*r.m21;
		m.m11 = l.m10*r.m01 + l.m11*r.m11 + l.m12*r.m21;
		m.m21 = l.m20*r.m01 + l.m21*r.m11 + l.m22*r.m21;
		m.m02 = l.m00*r.m02 + l.m01*r.m12 + l.m02*r.m22;
		m.m12 = l.m10*r.m02 + l.m11*r.m12 + l.m12*r.m22;
		m.m22 = l.m20*r.m02 + l.m21*r.m12 + l.m22*r.m22;
		m.m03 = l.m00*r.m03 + l.m01*r.m13 + l.m02*r.m23 + l.m03;
		m.m13 = l.m10*r.m03 + l.m11*r.m13 + l.m12*r.m23 + l.m13;
		m.m23 = l.m20*r.m03 + l.m21*r.m13 + l.m22*r.m23 + l.m23;
		return m;
	}

	FLOAT m00, m01, m02, m03;
	FLOAT m10, m11, m12, m13;
	FLOAT m20, m21, m22, m23;
};

/*struct AABB {
Vec3 min;
Vec3 max;
ILINE  AABB() {
}
};*/
#pragma endregion