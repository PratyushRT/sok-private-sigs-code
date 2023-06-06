#pragma once
/**
	emulate mpz_class
*/
#include <mcl/bint.hpp>

#ifndef CYBOZU_DONT_USE_EXCEPTION
#include <cybozu/exception.hpp>
#endif
#include <cybozu/xorshift.hpp>
#ifndef CYBOZU_DONT_USE_STRING
#include <iostream>
#endif
#include <mcl/array.hpp>
#include <mcl/util.hpp>
#include <mcl/randgen.hpp>
#include <mcl/conversion.hpp>
#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <string.h>

namespace mcl {

namespace vint {

class FixedBuffer {
	static const size_t N = maxUnitSize * 2 + 1;
	size_t size_;
	Unit v_[N];
public:
	FixedBuffer()
		: size_(0)
	{
	}
	FixedBuffer(const FixedBuffer& rhs)
	{
		operator=(rhs);
	}
	FixedBuffer& operator=(const FixedBuffer& rhs)
	{
		size_ = rhs.size_;
#if defined(__GNUC__) && !defined(__EMSCRIPTEN__) && !defined(__clang__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
		for (size_t i = 0; i < size_; i++) {
			v_[i] = rhs.v_[i];
		}
#if defined(__GNUC__) && !defined(__EMSCRIPTEN__) && !defined(__clang__)
	#pragma GCC diagnostic pop
#endif
		return *this;
	}
	void clear() { size_ = 0; }
	void alloc(bool *pb, size_t n)
	{
		if (n > N) {
			*pb = false;
			return;
		}
		size_ = n;
		*pb = true;
	}
	void swap(FixedBuffer& rhs)
	{
		FixedBuffer *p1 = this;
		FixedBuffer *p2 = &rhs;
		if (p1->size_ < p2->size_) {
			fp::swap_(p1, p2);
		}
		assert(p1->size_ >= p2->size_);
		for (size_t i = 0; i < p2->size_; i++) {
			fp::swap_(p1->v_[i], p2->v_[i]);
		}
		for (size_t i = p2->size_; i < p1->size_; i++) {
			p2->v_[i] = p1->v_[i];
		}
		fp::swap_(p1->size_, p2->size_);
	}
	// to avoid warning of gcc
	void verify(size_t n) const
	{
		assert(n <= N);
		(void)n;
	}
	const Unit& operator[](size_t n) const { verify(n); return v_[n]; }
	Unit& operator[](size_t n) { verify(n); return v_[n]; }
};

} // vint

/**
	signed integer with variable length
*/
class Vint {
public:
	typedef vint::FixedBuffer Buffer;
	static const size_t UnitBitSize = sizeof(Unit) * 8;
	static const int invalidVar = -2147483647 - 1; // abs(invalidVar) is not defined
private:
	Buffer buf_;
	size_t size_;
	bool isNeg_;
	void trim(size_t n)
	{
		assert(n > 0);
		int i = (int)n - 1;
		for (; i > 0; i--) {
			if (buf_[i]) {
				size_ = i + 1;
				return;
			}
		}
		size_ = 1;
		// zero
		if (buf_[0] == 0) {
			isNeg_ = false;
		}
	}
	static int ucompare(const Buffer& x, size_t xn, const Buffer& y, size_t yn)
	{
		if (xn == yn) return bint::cmpN(&x[0], &y[0], xn);
		return xn > yn ? 1 : -1;
	}
	static void uadd(Vint& z, const Buffer& x, size_t xn, const Buffer& y, size_t yn)
	{
		const Unit *px = &x[0];
		const Unit *py = &y[0];
		if (yn > xn) {
			fp::swap_(xn, yn);
			fp::swap_(px, py);
		}
		assert(xn >= yn);
		bool b;
		// &x[0] and &y[0] will not change if z == x or z == y because they are FixedBuffer
		z.buf_.alloc(&b, xn + 1);
		assert(b);
		if (!b) {
			z.clear();
			return;
		}
		Unit *dst = &z.buf_[0];
		Unit c = bint::addN(dst, px, py, yn);
		if (xn > yn) {
			size_t n = xn - yn;
			if (dst != px) bint::copyN(dst + yn, px + yn, n);
			c = bint::addUnit(dst + yn, n, c);
		}
		dst[xn] = c;
		z.trim(xn + 1);
	}
	static void uadd1(Vint& z, const Buffer& x, size_t xn, Unit y)
	{
		size_t zn = xn + 1;
		bool b;
		z.buf_.alloc(&b, zn);
		assert(b);
		if (!b) {
			z.clear();
			return;
		}
		if (&z.buf_[0] != &x[0]) bint::copyN(&z.buf_[0], &x[0], xn);
		z.buf_[zn - 1] = bint::addUnit(&z.buf_[0], xn, y);
		z.trim(zn);
	}
	static void usub1(Vint& z, const Buffer& x, size_t xn, Unit y)
	{
		size_t zn = xn;
		bool b;
		z.buf_.alloc(&b, zn);
		assert(b);
		if (!b) {
			z.clear();
			return;
		}
		Unit *dst = &z.buf_[0];
		const Unit *src = &x[0];
		if (dst != src) bint::copyN(dst, src, xn);
		Unit c = bint::subUnit(dst, xn, y);
		(void)c;
		assert(!c);
		z.trim(zn);
	}
	static void usub(Vint& z, const Buffer& x, size_t xn, const Buffer& y, size_t yn)
	{
		assert(xn >= yn);
		bool b;
		z.buf_.alloc(&b, xn);
		assert(b);
		if (!b) {
			z.clear();
			return;
		}
		Unit c = bint::subN(&z.buf_[0], &x[0], &y[0], yn);
		if (xn > yn) {
			size_t n = xn - yn;
			Unit *dst = &z.buf_[yn];
			const Unit *src = &x[yn];
			if (dst != src) bint::copyN(dst, src, n);
			c = bint::subUnit(dst, n, c);
		}
		assert(!c);
		z.trim(xn);
	}
	static void _add(Vint& z, const Vint& x, bool xNeg, const Vint& y, bool yNeg)
	{
		if ((xNeg ^ yNeg) == 0) {
			// same sign
			uadd(z, x.buf_, x.size(), y.buf_, y.size());
			z.isNeg_ = xNeg;
			return;
		}
		int r = ucompare(x.buf_, x.size(), y.buf_, y.size());
		if (r >= 0) {
			usub(z, x.buf_, x.size(), y.buf_, y.size());
			z.isNeg_ = xNeg;
		} else {
			usub(z, y.buf_, y.size(), x.buf_, x.size());
			z.isNeg_ = yNeg;
		}
	}
	static void _adds1(Vint& z, const Vint& x, int y, bool yNeg)
	{
		assert(y >= 0);
		if ((x.isNeg_ ^ yNeg) == 0) {
			// same sign
			uadd1(z, x.buf_, x.size(), y);
			z.isNeg_ = yNeg;
			return;
		}
		if (x.size() > 1 || x.buf_[0] >= (Unit)y) {
			usub1(z, x.buf_, x.size(), y);
			z.isNeg_ = x.isNeg_;
		} else {
			z = y - x.buf_[0];
			z.isNeg_ = yNeg;
		}
	}
	static void _addu1(Vint& z, const Vint& x, Unit y, bool yNeg)
	{
		if ((x.isNeg_ ^ yNeg) == 0) {
			// same sign
			uadd1(z, x.buf_, x.size(), y);
			z.isNeg_ = yNeg;
			return;
		}
		if (x.size() > 1 || x.buf_[0] >= y) {
			usub1(z, x.buf_, x.size(), y);
			z.isNeg_ = x.isNeg_;
		} else {
			z = y - x.buf_[0];
			z.isNeg_ = yNeg;
		}
	}
	/**
		@param q [out] x / y if q != 0
		@param r [out] x % y
	*/
	static void udiv(Vint* q, Vint& r, const Buffer& x, size_t xn, const Buffer& y, size_t yn)
	{
		assert(q != &r);
		if (xn < yn) {
			r.buf_ = x;
			r.trim(xn);
			if (q) q->clear();
			return;
		}
		size_t qn = xn - yn + 1;
		bool b;
		if (q) {
			q->buf_.alloc(&b, qn);
			assert(b); (void)b;
		}
		Unit *xx = (Unit*)CYBOZU_ALLOCA(sizeof(Unit) * xn);
		bint::copyN(xx, &x[0], xn);
		Unit *qq = q ? &q->buf_[0] : 0;
		size_t rn = bint::div(qq, qn, xx, xn, &y[0], yn);
		r.buf_.alloc(&b, rn);
		assert(b); (void)b;
		bint::copyN(&r.buf_[0], xx, rn);
		if (q) {
			q->trim(qn);
		}
		r.trim(rn);
	}
	/*
		@param x [inout] x <- d
		@retval s for x = 2^s d where d is odd
	*/
	static uint32_t countTrailingZero(Vint& x)
	{
		uint32_t s = 0;
		while (x.isEven()) {
			x >>= 1;
			s++;
		}
		return s;
	}
	struct MulMod {
		const Vint *pm;
		void operator()(Vint& z, const Vint& x, const Vint& y) const
		{
			Vint::mul(z, x, y);
			z %= *pm;
		}
	};
	struct SqrMod {
		const Vint *pm;
		void operator()(Vint& y, const Vint& x) const
		{
			Vint::sqr(y, x);
			y %= *pm;
		}
	};
	// z = x^y
	template<class Mul, class Sqr>
	static void powT(Vint& z, const Vint& x, const Unit *y, size_t n, const Mul& mul, const Sqr& sqr)
	{
		while (n > 0) {
			if (y[n - 1]) break;
			n--;
		}
		if (n == 0) n = 1;
		if (n == 1) {
			switch (y[0]) {
			case 0:
				z = 1;
				return;
			case 1:
				z = x;
				return;
			case 2:
				sqr(z, x);
				return;
			case 3:
				{
					Vint t;
					sqr(t, x);
					mul(z, t, x);
				}
				return;
			case 4:
				sqr(z, x);
				sqr(z, z);
				return;
			}
		}
		const size_t w = 4; // don't change
		const size_t m = sizeof(Unit) * 8 / w;
		const size_t tblSize = (1 << w) - 1;
		Vint tbl[tblSize];
		tbl[0] = x;
		for (size_t i = 1; i < tblSize; i++) {
			mul(tbl[i], tbl[i - 1], x);
		}
		Unit *yy = 0;
		if (y == &z.buf_[0]) { // keep original y(=z)
			yy = (Unit *)CYBOZU_ALLOCA(MCL_SIZEOF_UNIT * n);
			bint::copyN(yy, y, n);
			y = yy;
		}
		z = 1;
		for (size_t i = 0; i < n; i++) {
			Unit v = y[n - 1 - i];
			for (size_t j = 0; j < m; j++) {
				for (size_t k = 0; k < w; k++) {
					sqr(z, z);
				}
				Unit idx = (v >> ((m - 1 - j) * w)) & tblSize;
				if (idx) mul(z, z, tbl[idx - 1]);
			}
		}
	}

public:
	Vint(int x = 0)
		: size_(0)
	{
		*this = x;
	}
	Vint(Unit x)
		: size_(0)
	{
		*this = x;
	}
	Vint(const Vint& rhs)
		: buf_(rhs.buf_)
		, size_(rhs.size_)
		, isNeg_(rhs.isNeg_)
	{
	}
	Vint& operator=(int x)
	{
		assert(x != invalidVar);
		isNeg_ = x < 0;
		bool b;
		buf_.alloc(&b, 1);
		assert(b); (void)b;
		buf_[0] = fp::abs_(x);
		size_ = 1;
		return *this;
	}
	Vint& operator=(Unit x)
	{
		isNeg_ = false;
		bool b;
		buf_.alloc(&b, 1);
		assert(b); (void)b;
		buf_[0] = x;
		size_ = 1;
		return *this;
	}
	Vint& operator=(const Vint& rhs)
	{
		buf_ = rhs.buf_;
		size_ = rhs.size_;
		isNeg_ = rhs.isNeg_;
		return *this;
	}
	void swap(Vint& rhs)
#if CYBOZU_CPP_VERSION >= CYBOZU_CPP_VERSION_CPP11
		noexcept
#endif
	{
		fp::swap_(buf_, rhs.buf_);
		fp::swap_(size_, rhs.size_);
		fp::swap_(isNeg_, rhs.isNeg_);
	}
	void dump(const char *msg = "") const
	{
		bint::dump(&buf_[0], size_, msg);
	}
	/*
		set positive value
		@note x is treated as a little endian
	*/
	template<class S>
	void setArray(bool *pb, const S *x, size_t size)
	{
		isNeg_ = false;
		if (size == 0) {
			clear();
			*pb = true;
			return;
		}
		size_t unitSize = (sizeof(S) * size + sizeof(Unit) - 1) / sizeof(Unit);
		buf_.alloc(pb, unitSize);
		if (!*pb) return;
		bool b = fp::convertArrayAsLE(&buf_[0], unitSize, x, size);
		assert(b);
		(void)b;
		trim(unitSize);
	}
	/*
		set [0, max) randomly
	*/
	void setRand(bool *pb, const Vint& max, fp::RandGen rg = fp::RandGen())
	{
		assert(max > 0);
		if (rg.isZero()) rg = fp::RandGen::get();
		size_t n = max.size();
		buf_.alloc(pb, n);
		if (!*pb) return;
		rg.read(pb, &buf_[0], n * sizeof(buf_[0]));
		if (!*pb) return;
		trim(n);
		*this %= max;
	}
	/*
		get abs value
		buf_[0, size) = x
		buf_[size, maxSize) with zero
		@note assume little endian system
	*/
	void getArray(bool *pb, Unit *x, size_t maxSize) const
	{
		size_t n = size();
		if (n > maxSize) {
			*pb = false;
			return;
		}
		bint::copyN(x, &buf_[0], n);
		bint::clearN(x + n, maxSize - n);
		*pb = true;
	}
	void clear() { *this = 0; }
	template<class OutputStream>
	void save(bool *pb, OutputStream& os, int base = 10) const
	{
		if (isNeg_) cybozu::writeChar(pb, os, '-');
		char buf[1024];
		size_t n = mcl::fp::arrayToStr(buf, sizeof(buf), &buf_[0], size_, base, false);
		if (n == 0) {
			*pb = false;
			return;
		}
		cybozu::write(pb, os, buf + sizeof(buf) - n, n);
	}
	/*
		set buf with string terminated by '\0'
		return strlen(buf) if success else 0
	*/
	size_t getStr(char *buf, size_t bufSize, int base = 10) const
	{
		cybozu::MemoryOutputStream os(buf, bufSize);
		bool b;
		save(&b, os, base);
		const size_t n = os.getPos();
		if (!b || n == bufSize) return 0;
		buf[n] = '\0';
		return n;
	}
	/*
		return bitSize(abs(*this))
		@note return 1 if zero
	*/
	size_t getBitSize() const
	{
		if (isZero()) return 1;
		size_t n = size();
		Unit v = buf_[n - 1];
		assert(v);
		return (n - 1) * sizeof(Unit) * 8 + 1 + cybozu::bsr<Unit>(v);
	}
	// ignore sign
	bool testBit(size_t i) const
	{
		size_t q = i / UnitBitSize;
		size_t r = i % UnitBitSize;
		assert(q <= size());
		Unit mask = Unit(1) << r;
		return (buf_[q] & mask) != 0;
	}
	void setBit(size_t i, bool v = true)
	{
		size_t q = i / UnitBitSize;
		size_t r = i % UnitBitSize;
		assert(q <= size());
		bool b;
		buf_.alloc(&b, q + 1);
		assert(b);
		if (!b) {
			clear();
			return;
		}
		Unit mask = Unit(1) << r;
		if (v) {
			buf_[q] |= mask;
		} else {
			buf_[q] &= ~mask;
			trim(q + 1);
		}
	}
	/*
		@param str [in] number string
		@note "0x..."   => base = 16
		      "0b..."   => base = 2
		      otherwise => base = 10
	*/
	void setStr(bool *pb, const char *str, int base = 0)
	{
		// allow twice size of MCL_MAX_BIT_SIZE because of multiplication
		const size_t maxN = (MCL_MAX_BIT_SIZE * 2 + UnitBitSize - 1) / UnitBitSize;
		buf_.alloc(pb, maxN);
		if (!*pb) return;
		*pb = false;
		isNeg_ = false;
		size_t len = strlen(str);
		size_t n = fp::strToArray(&isNeg_, &buf_[0], maxN, str, len, base);
		if (n == 0) return;
		trim(n);
		*pb = true;
	}
	static int compare(const Vint& x, const Vint& y)
	{
		if (x.isNeg_ ^ y.isNeg_) {
			if (x.isZero() && y.isZero()) return 0;
			return x.isNeg_ ? -1 : 1;
		} else {
			// same sign
			int c = ucompare(x.buf_, x.size(), y.buf_, y.size());
			if (x.isNeg_) {
				return -c;
			}
			return c;
		}
	}
	static int compares1(const Vint& x, int y)
	{
		assert(y != invalidVar);
		if (x.isNeg_ ^ (y < 0)) {
			if (x.isZero() && y == 0) return 0;
			return x.isNeg_ ? -1 : 1;
		} else {
			// same sign
			Unit y0 = fp::abs_(y);
			int c = (x.size() > 1) ? 1 : bint::cmpT<1>(&x.buf_[0], &y0);
			if (x.isNeg_) {
				return -c;
			}
			return c;
		}
	}
	static int compareu1(const Vint& x, uint32_t y)
	{
		if (x.isNeg_) return -1;
		if (x.size() > 1) return 1;
		Unit x0 = x.buf_[0];
		return x0 > y ? 1 : x0 == y ? 0 : -1;
	}
	size_t size() const { return size_; }
	bool isZero() const { return size() == 1 && buf_[0] == 0; }
	bool isNegative() const { return !isZero() && isNeg_; }
	uint32_t getLow32bit() const { return (uint32_t)buf_[0]; }
	bool isOdd() const { return (buf_[0] & 1) == 1; }
	bool isEven() const { return !isOdd(); }
	const Unit *getUnit() const { return &buf_[0]; }
	size_t getUnitSize() const { return size_; }
	static void add(Vint& z, const Vint& x, const Vint& y)
	{
		_add(z, x, x.isNeg_, y, y.isNeg_);
	}
	static void sub(Vint& z, const Vint& x, const Vint& y)
	{
		_add(z, x, x.isNeg_, y, !y.isNeg_);
	}
	static void mul(Vint& z, const Vint& x, const Vint& y)
	{
		const size_t xn = x.size();
		const size_t yn = y.size();
		size_t zn = xn + yn;
		bool b;
		z.buf_.alloc(&b, zn);
		assert(b); (void)b;
		bint::mulNM(&z.buf_[0], &x.buf_[0], xn, &y.buf_[0], yn);
		z.trim(zn);
		z.isNeg_ = x.isNeg_ ^ y.isNeg_;
	}
	static void sqr(Vint& y, const Vint& x)
	{
		mul(y, x, x);
	}
	static void addu1(Vint& z, const Vint& x, Unit y)
	{
		_addu1(z, x, y, false);
	}
	static void subu1(Vint& z, const Vint& x, Unit y)
	{
		_addu1(z, x, y, true);
	}
	static void mulu1(Vint& z, const Vint& x, Unit y)
	{
		size_t xn = x.size();
		size_t zn = xn + 1;
		bool b;
		z.buf_.alloc(&b, zn);
		assert(b);
		if (!b) {
			z.clear();
			return;
		}
		z.buf_[zn - 1] = bint::mulUnitN(&z.buf_[0], &x.buf_[0], y, xn);
		z.isNeg_ = x.isNeg_;
		z.trim(zn);
	}
	static void divu1(Vint& q, const Vint& x, Unit y)
	{
		udivModu1(&q, x, y);
	}
	static void modu1(Vint& r, const Vint& x, Unit y)
	{
		bool xNeg = x.isNeg_;
		r = udivModu1(0, x, y);
		r.isNeg_ = xNeg;
	}
	static void adds1(Vint& z, const Vint& x, int y)
	{
		assert(y != invalidVar);
		_adds1(z, x, fp::abs_(y), y < 0);
	}
	static void subs1(Vint& z, const Vint& x, int y)
	{
		assert(y != invalidVar);
		_adds1(z, x, fp::abs_(y), !(y < 0));
	}
	static void muls1(Vint& z, const Vint& x, int y)
	{
		assert(y != invalidVar);
		mulu1(z, x, fp::abs_(y));
		z.isNeg_ ^= (y < 0);
	}
	/*
		@param q [out] q = x / y if q is not zero
		@param x [in]
		@param y [in] must be not zero
		return x % y
	*/
	static int divMods1(Vint *q, const Vint& x, int y)
	{
		assert(y != invalidVar);
		bool xNeg = x.isNeg_;
		bool yNeg = y < 0;
		Unit absY = fp::abs_(y);
		size_t xn = x.size();
		int r;
		if (q) {
			q->isNeg_ = xNeg ^ yNeg;
			bool b;
			q->buf_.alloc(&b, xn);
			assert(b);
			if (!b) {
				q->clear();
				return 0;
			}
			r = (int)bint::divUnit(&q->buf_[0], &x.buf_[0], xn, absY);
			q->trim(xn);
		} else {
			r = (int)bint::modUnit(&x.buf_[0], xn, absY);
		}
		return xNeg ? -r : r;
	}
	/*
		like C
		  13 /  5 =  2 ...  3
		  13 / -5 = -2 ...  3
		 -13 /  5 = -2 ... -3
		 -13 / -5 =  2 ... -3
	*/
	static void divMod(Vint *q, Vint& r, const Vint& x, const Vint& y)
	{
		bool xNeg = x.isNeg_;
		bool qsign = xNeg ^ y.isNeg_;
		udiv(q, r, x.buf_, x.size(), y.buf_, y.size());
		r.isNeg_ = xNeg;
		if (q) q->isNeg_ = qsign;
	}
	static void div(Vint& q, const Vint& x, const Vint& y)
	{
		Vint r;
		divMod(&q, r, x, y);
	}
	static void mod(Vint& r, const Vint& x, const Vint& y)
	{
		divMod(0, r, x, y);
	}
	static void divs1(Vint& q, const Vint& x, int y)
	{
		divMods1(&q, x, y);
	}
	static void mods1(Vint& r, const Vint& x, int y)
	{
		bool xNeg = x.isNeg_;
		r = divMods1(0, x, y);
		r.isNeg_ = xNeg;
	}
	static Unit udivModu1(Vint *q, const Vint& x, Unit y)
	{
		assert(!x.isNeg_);
		size_t xn = x.size();
		if (q) {
			bool b;
			q->buf_.alloc(&b, xn);
			assert(b);
			if (!b) {
				q->clear();
				return 0;
			}
		}
		Unit r = bint::divUnit(q ? &q->buf_[0] : 0, &x.buf_[0], xn, y);
		if (q) {
			q->trim(xn);
			q->isNeg_ = false;
		}
		return r;
	}
	/*
		like Python
		 13 /  5 =  2 ...  3
		 13 / -5 = -3 ... -2
		-13 /  5 = -3 ...  2
		-13 / -5 =  2 ... -3
	*/
	static void quotRem(Vint *q, Vint& r, const Vint& x, const Vint& y)
	{
		assert(q != &r);
		Vint yy = y;
		bool yNeg = y.isNeg_;
		bool qsign = x.isNeg_ ^ yNeg;
		udiv(q, r, x.buf_, x.size(), yy.buf_, yy.size());
		r.isNeg_ = yNeg;
		if (q) q->isNeg_ = qsign;
		if (!r.isZero() && qsign) {
			if (q) {
				uadd1(*q, q->buf_, q->size(), 1);
			}
			usub(r, yy.buf_, yy.size(), r.buf_, r.size());
		}
	}
	template<class InputStream>
	void load(bool *pb, InputStream& is, int ioMode)
	{
		*pb = false;
		char buf[1024];
		size_t n = fp::local::loadWord(buf, sizeof(buf), is);
		if (n == 0) return;
		const size_t maxN = 384 / (sizeof(MCL_SIZEOF_UNIT) * 8);
		buf_.alloc(pb, maxN);
		if (!*pb) return;
		isNeg_ = false;
		n = fp::strToArray(&isNeg_, &buf_[0], maxN, buf, n, ioMode);
		if (n == 0) return;
		trim(n);
		*pb = true;
	}
	// logical left shift (copy sign)
	static void shl(Vint& y, const Vint& x, size_t shiftBit)
	{
		assert(shiftBit <= MCL_MAX_BIT_SIZE * 2); // many be too big
		size_t xn = x.size();
		size_t yn = xn + (shiftBit + UnitBitSize - 1) / UnitBitSize;
		bool b;
		y.buf_.alloc(&b, yn);
		assert(b); (void)b;
		bint::shiftLeft(&y.buf_[0], &x.buf_[0], shiftBit, xn);
		y.isNeg_ = x.isNeg_;
		y.trim(yn);
	}
	// logical right shift (copy sign)
	static void shr(Vint& y, const Vint& x, size_t shiftBit)
	{
		assert(shiftBit <= MCL_MAX_BIT_SIZE * 2); // many be too big
		size_t xn = x.size();
		if (xn * UnitBitSize <= shiftBit) {
			y.clear();
			return;
		}
		size_t yn = xn - shiftBit / UnitBitSize;
		bool b;
		y.buf_.alloc(&b, yn);
		assert(b); (void)b;
		bint::shiftRight(&y.buf_[0], &x.buf_[0], shiftBit, xn);
		y.isNeg_ = x.isNeg_;
		y.trim(yn);
	}
	static void neg(Vint& y, const Vint& x)
	{
		if (&y != &x) { y = x; }
		y.isNeg_ = !x.isNeg_;
	}
	static void abs(Vint& y, const Vint& x)
	{
		if (&y != &x) { y = x; }
		y.isNeg_ = false;
	}
	static Vint abs(const Vint& x)
	{
		Vint y = x;
		abs(y, x);
		return y;
	}
	// accept only non-negative value
	static void orBit(Vint& z, const Vint& x, const Vint& y)
	{
		assert(!x.isNeg_ && !y.isNeg_);
		const Vint *px = &x, *py = &y;
		if (x.size() < y.size()) {
			fp::swap_(px, py);
		}
		size_t xn = px->size();
		size_t yn = py->size();
		assert(xn >= yn);
		bool b;
		z.buf_.alloc(&b, xn);
		assert(b);
		if (!b) {
			z.clear();
		}
		for (size_t i = 0; i < yn; i++) {
			z.buf_[i] = x.buf_[i] | y.buf_[i];
		}
		bint::copyN(&z.buf_[0] + yn, &px->buf_[0] + yn, xn - yn);
		z.trim(xn);
	}
	static void andBit(Vint& z, const Vint& x, const Vint& y)
	{
		assert(!x.isNeg_ && !y.isNeg_);
		const Vint *px = &x, *py = &y;
		if (x.size() < y.size()) {
			fp::swap_(px, py);
		}
		size_t yn = py->size();
		assert(px->size() >= yn);
		bool b;
		z.buf_.alloc(&b, yn);
		assert(b);
		if (!b) {
			z.clear();
			return;
		}
		for (size_t i = 0; i < yn; i++) {
			z.buf_[i] = x.buf_[i] & y.buf_[i];
		}
		z.trim(yn);
	}
	static void orBitu1(Vint& z, const Vint& x, Unit y)
	{
		assert(!x.isNeg_);
		z = x;
		z.buf_[0] |= y;
	}
	static void andBitu1(Vint& z, const Vint& x, Unit y)
	{
		assert(!x.isNeg_);
		bool b;
		z.buf_.alloc(&b, 1);
		assert(b); (void)b;
		z.buf_[0] = x.buf_[0] & y;
		z.size_ = 1;
		z.isNeg_ = false;
	}
	/*
		REMARK y >= 0;
	*/
	static void pow(Vint& z, const Vint& x, const Vint& y)
	{
		assert(!y.isNeg_);
		powT(z, x, &y.buf_[0], y.size(), mul, sqr);
	}
	/*
		REMARK y >= 0;
	*/
	static void pow(Vint& z, const Vint& x, int64_t y)
	{
		assert(y >= 0);
#if MCL_SIZEOF_UNIT == 8
		Unit ua = fp::abs_(y);
		powT(z, x, &ua, 1, mul, sqr);
#else
		uint64_t ua = fp::abs_(y);
		Unit u[2] = { uint32_t(ua), uint32_t(ua >> 32) };
		size_t un = u[1] ? 2 : 1;
		powT(z, x, u, un, mul, sqr);
#endif
	}
	/*
		z = x ^ y mod m
		REMARK y >= 0;
	*/
	static void powMod(Vint& z, const Vint& x, const Vint& y, const Vint& m)
	{
		assert(!y.isNeg_);
		MulMod mulMod;
		SqrMod sqrMod;
		mulMod.pm = &m;
		sqrMod.pm = &m;
		powT(z, x, &y.buf_[0], y.size(), mulMod, sqrMod);
	}
	/*
		inverse mod
		y = 1/x mod m
		REMARK x != 0 and m != 0;
	*/
	static void invMod(Vint& y, const Vint& x, const Vint& m)
	{
		assert(!x.isZero() && !m.isZero());
#if 0
		Vint u = x;
		Vint v = m;
		Vint x1 = 1, x2 = 0;
		Vint t;
		while (u != 1 && v != 1) {
			while (u.isEven()) {
				u >>= 1;
				if (x1.isOdd()) {
					x1 += m;
				}
				x1 >>= 1;
			}
			while (v.isEven()) {
				v >>= 1;
				if (x2.isOdd()) {
					x2 += m;
				}
				x2 >>= 1;
			}
			if (u >= v) {
				u -= v;
				x1 -= x2;
				if (x1 < 0) {
					x1 += m;
				}
			} else {
				v -= u;
				x2 -= x1;
				if (x2 < 0) {
					x2 += m;
				}
			}
		}
		if (u == 1) {
			y = x1;
		} else {
			y = x2;
		}
#else
		if (x == 1) {
			y = 1;
			return;
		}
		Vint a = 1;
		Vint t;
		Vint q;
		divMod(&q, t, m, x);
		Vint s = x;
		Vint b = -q;

		for (;;) {
			divMod(&q, s, s, t);
			if (s.isZero()) {
				if (b.isNeg_) {
					b += m;
				}
				y = b;
				return;
			}
			a -= b * q;

			divMod(&q, t, t, s);
			if (t.isZero()) {
				if (a.isNeg_) {
					a += m;
				}
				y = a;
				return;
			}
			b -= a * q;
		}
#endif
	}
	/*
		Miller-Rabin
	*/
	static bool isPrime(bool *pb, const Vint& n, int tryNum = 32)
	{
		*pb = true;
		if (n <= 1) return false;
		if (n == 2 || n == 3) return true;
		if (n.isEven()) return false;
		cybozu::XorShift rg;
		const Vint nm1 = n - 1;
		Vint d = nm1;
		uint32_t r = countTrailingZero(d);
		// n - 1 = 2^r d
		Vint a, x;
		for (int i = 0; i < tryNum; i++) {
			a.setRand(pb, n - 3, rg);
			if (!*pb) return false;
			a += 2; // a in [2, n - 2]
			powMod(x, a, d, n);
			if (x == 1 || x == nm1) {
				continue;
			}
			for (uint32_t j = 1; j < r; j++) {
				sqr(x, x);
				x %= n;
				if (x == 1) return false;
				if (x == nm1) goto NEXT_LOOP;
			}
			return false;
		NEXT_LOOP:;
		}
		return true;
	}
	bool isPrime(bool *pb, int tryNum = 32) const
	{
		return isPrime(pb, *this, tryNum);
	}
	static void gcd(Vint& z, Vint x, Vint y)
	{
		Vint t;
		for (;;) {
			if (y.isZero()) {
				z = x;
				return;
			}
			t = x;
			x = y;
			mod(y, t, y);
		}
	}
	static Vint gcd(const Vint& x, const Vint& y)
	{
		Vint z;
		gcd(z, x, y);
		return z;
	}
	static void lcm(Vint& z, const Vint& x, const Vint& y)
	{
		Vint c;
		gcd(c, x, y);
		div(c, x, c);
		mul(z, c, y);
	}
	static Vint lcm(const Vint& x, const Vint& y)
	{
		Vint z;
		lcm(z, x, y);
		return z;
	}
	/*
		 1 if m is quadratic residue modulo n (i.e., there exists an x s.t. x^2 = m mod n)
		 0 if m = 0 mod n
		-1 otherwise
		@note return legendre_symbol(m, p) for m and odd prime p
	*/
	static int jacobi(Vint m, Vint n)
	{
		assert(n.isOdd());
		if (n == 1) return 1;
		if (m < 0 || m > n) {
			quotRem(0, m, m, n); // m = m mod n
		}
		if (m.isZero()) return 0;
		if (m == 1) return 1;
		if (gcd(m, n) != 1) return 0;

		int j = 1;
		Vint t;
		goto START;
		while (m != 1) {
			if ((m.getLow32bit() % 4) == 3 && (n.getLow32bit() % 4) == 3) {
				j = -j;
			}
			mod(t, n, m);
			n = m;
			m = t;
		START:
			int s = countTrailingZero(m);
			uint32_t nmod8 = n.getLow32bit() % 8;
			if ((s % 2) && (nmod8 == 3 || nmod8 == 5)) {
				j = -j;
			}
		}
		return j;
	}
#ifndef CYBOZU_DONT_USE_STRING
	explicit Vint(const std::string& str)
		: size_(0)
	{
		setStr(str);
	}
	void getStr(std::string& s, int base = 10) const
	{
		s.clear();
		cybozu::StringOutputStream os(s);
		save(os, base);
	}
	std::string getStr(int base = 10) const
	{
		std::string s;
		getStr(s, base);
		return s;
	}
	inline friend std::ostream& operator<<(std::ostream& os, const Vint& x)
	{
		return os << x.getStr(os.flags() & std::ios_base::hex ? 16 : 10);
	}
	inline friend std::istream& operator>>(std::istream& is, Vint& x)
	{
		x.load(is);
		return is;
	}
#endif
#ifndef CYBOZU_DONT_USE_EXCEPTION
	void setStr(const std::string& str, int base = 0)
	{
		bool b;
		setStr(&b, str.c_str(), base);
		if (!b) throw cybozu::Exception("Vint:setStr") << str;
	}
	void setRand(const Vint& max, fp::RandGen rg = fp::RandGen())
	{
		bool b;
		setRand(&b, max, rg);
		if (!b) throw cybozu::Exception("Vint:setRand");
	}
	void getArray(Unit *x, size_t maxSize) const
	{
		bool b;
		getArray(&b, x, maxSize);
		if (!b) throw cybozu::Exception("Vint:getArray");
	}
	template<class InputStream>
	void load(InputStream& is, int ioMode = 0)
	{
		bool b;
		load(&b, is, ioMode);
		if (!b) throw cybozu::Exception("Vint:load");
	}
	template<class OutputStream>
	void save(OutputStream& os, int base = 10) const
	{
		bool b;
		save(&b, os, base);
		if (!b) throw cybozu::Exception("Vint:save");
	}
	static bool isPrime(const Vint& n, int tryNum = 32)
	{
		bool b;
		bool ret = isPrime(&b, n, tryNum);
		if (!b) throw cybozu::Exception("Vint:isPrime");
		return ret;
	}
	bool isPrime(int tryNum = 32) const
	{
		bool b;
		bool ret = isPrime(&b, *this, tryNum);
		if (!b) throw cybozu::Exception("Vint:isPrime");
		return ret;
	}
	template<class S>
	void setArray(const S *x, size_t size)
	{
		bool b;
		setArray(&b, x, size);
		if (!b) throw cybozu::Exception("Vint:setArray");
	}
#endif
	Vint& operator++() { adds1(*this, *this, 1); return *this; }
	Vint& operator--() { subs1(*this, *this, 1); return *this; }
	Vint operator++(int) { Vint c = *this; adds1(*this, *this, 1); return c; }
	Vint operator--(int) { Vint c = *this; subs1(*this, *this, 1); return c; }
	friend bool operator<(const Vint& x, const Vint& y) { return compare(x, y) < 0; }
	friend bool operator>=(const Vint& x, const Vint& y) { return !operator<(x, y); }
	friend bool operator>(const Vint& x, const Vint& y) { return compare(x, y) > 0; }
	friend bool operator<=(const Vint& x, const Vint& y) { return !operator>(x, y); }
	friend bool operator==(const Vint& x, const Vint& y) { return compare(x, y) == 0; }
	friend bool operator!=(const Vint& x, const Vint& y) { return !operator==(x, y); }

	friend bool operator<(const Vint& x, int y) { return compares1(x, y) < 0; }
	friend bool operator>=(const Vint& x, int y) { return !operator<(x, y); }
	friend bool operator>(const Vint& x, int y) { return compares1(x, y) > 0; }
	friend bool operator<=(const Vint& x, int y) { return !operator>(x, y); }
	friend bool operator==(const Vint& x, int y) { return compares1(x, y) == 0; }
	friend bool operator!=(const Vint& x, int y) { return !operator==(x, y); }

	friend bool operator<(const Vint& x, uint32_t y) { return compareu1(x, y) < 0; }
	friend bool operator>=(const Vint& x, uint32_t y) { return !operator<(x, y); }
	friend bool operator>(const Vint& x, uint32_t y) { return compareu1(x, y) > 0; }
	friend bool operator<=(const Vint& x, uint32_t y) { return !operator>(x, y); }
	friend bool operator==(const Vint& x, uint32_t y) { return compareu1(x, y) == 0; }
	friend bool operator!=(const Vint& x, uint32_t y) { return !operator==(x, y); }

	Vint& operator+=(const Vint& rhs) { add(*this, *this, rhs); return *this; }
	Vint& operator-=(const Vint& rhs) { sub(*this, *this, rhs); return *this; }
	Vint& operator*=(const Vint& rhs) { mul(*this, *this, rhs); return *this; }
	Vint& operator/=(const Vint& rhs) { div(*this, *this, rhs); return *this; }
	Vint& operator%=(const Vint& rhs) { mod(*this, *this, rhs); return *this; }
	Vint& operator&=(const Vint& rhs) { andBit(*this, *this, rhs); return *this; }
	Vint& operator|=(const Vint& rhs) { orBit(*this, *this, rhs); return *this; }

	Vint& operator+=(int rhs) { adds1(*this, *this, rhs); return *this; }
	Vint& operator-=(int rhs) { subs1(*this, *this, rhs); return *this; }
	Vint& operator*=(int rhs) { muls1(*this, *this, rhs); return *this; }
	Vint& operator/=(int rhs) { divs1(*this, *this, rhs); return *this; }
	Vint& operator%=(int rhs) { mods1(*this, *this, rhs); return *this; }
	Vint& operator+=(Unit rhs) { addu1(*this, *this, rhs); return *this; }
	Vint& operator-=(Unit rhs) { subu1(*this, *this, rhs); return *this; }
	Vint& operator*=(Unit rhs) { mulu1(*this, *this, rhs); return *this; }
	Vint& operator/=(Unit rhs) { divu1(*this, *this, rhs); return *this; }
	Vint& operator%=(Unit rhs) { modu1(*this, *this, rhs); return *this; }

	Vint& operator&=(Unit rhs) { andBitu1(*this, *this, rhs); return *this; }
	Vint& operator|=(Unit rhs) { orBitu1(*this, *this, rhs); return *this; }

	friend Vint operator+(const Vint& a, const Vint& b) { Vint c; add(c, a, b); return c; }
	friend Vint operator-(const Vint& a, const Vint& b) { Vint c; sub(c, a, b); return c; }
	friend Vint operator*(const Vint& a, const Vint& b) { Vint c; mul(c, a, b); return c; }
	friend Vint operator/(const Vint& a, const Vint& b) { Vint c; div(c, a, b); return c; }
	friend Vint operator%(const Vint& a, const Vint& b) { Vint c; mod(c, a, b); return c; }
	friend Vint operator&(const Vint& a, const Vint& b) { Vint c; andBit(c, a, b); return c; }
	friend Vint operator|(const Vint& a, const Vint& b) { Vint c; orBit(c, a, b); return c; }

	friend Vint operator+(const Vint& a, int b) { Vint c; adds1(c, a, b); return c; }
	friend Vint operator-(const Vint& a, int b) { Vint c; subs1(c, a, b); return c; }
	friend Vint operator*(const Vint& a, int b) { Vint c; muls1(c, a, b); return c; }
	friend Vint operator/(const Vint& a, int b) { Vint c; divs1(c, a, b); return c; }
	friend Vint operator%(const Vint& a, int b) { Vint c; mods1(c, a, b); return c; }
	friend Vint operator+(const Vint& a, Unit b) { Vint c; addu1(c, a, b); return c; }
	friend Vint operator-(const Vint& a, Unit b) { Vint c; subu1(c, a, b); return c; }
	friend Vint operator*(const Vint& a, Unit b) { Vint c; mulu1(c, a, b); return c; }
	friend Vint operator/(const Vint& a, Unit b) { Vint c; divu1(c, a, b); return c; }
	friend Vint operator%(const Vint& a, Unit b) { Vint c; modu1(c, a, b); return c; }

	friend Vint operator&(const Vint& a, Unit b) { Vint c; andBitu1(c, a, b); return c; }
	friend Vint operator|(const Vint& a, Unit b) { Vint c; orBitu1(c, a, b); return c; }

	Vint operator-() const { Vint c; neg(c, *this); return c; }
	Vint& operator<<=(size_t n) { shl(*this, *this, n); return *this; }
	Vint& operator>>=(size_t n) { shr(*this, *this, n); return *this; }
	Vint operator<<(size_t n) const { Vint c = *this; c <<= n; return c; }
	Vint operator>>(size_t n) const { Vint c = *this; c >>= n; return c; }
};

} // mcl

//typedef mcl::Vint mpz_class;
