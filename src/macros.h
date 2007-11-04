/* $Id$ */

/** @file macros.h */

#ifndef MACROS_H
#define MACROS_H

/**
 * Fetch n bits from x, started at bit s.
 *
 * This macro can be used to fetch n bits from the value x. The
 * s value set the startposition to read. The startposition is
 * count from the LSB and starts at 0. The result starts at a
 * LSB, as this isn't just an and-bitmask but also some
 * bit-shifting operations. GB(0xFF, 2, 1) will so
 * return 0x01 (0000 0001) instead of
 * 0x04 (0000 0100).
 *
 * @param x The value to read some bits.
 * @param s The startposition to read some bits.
 * @param n The number of bits to read.
 * @return The selected bits, aligned to a LSB.
 */
#define GB(x, s, n) (((x) >> (s)) & ((1U << (n)) - 1))

/** Set n bits from x starting at bit s to d
 *
 * This macro sets n bits from x which started as bit s to the value of
 * d. The parameters x, s and n works the same as the parameters of
 * #GB. The result is saved in x again. Unused bits in the window
 * provided by n are set to 0 if the value of b isn't "big" enough.
 * This is not a bug, its a feature.
 *
 * @note Parameter x must be a variable as the result is saved there.
 * @note To avoid unexpecting results the value of b should not use more
 *       space as the provided space of n bits (log2)
 * @param x The variable to change some bits
 * @param s The startposition for the new bits
 * @param n The size/window for the new bits
 * @param d The actually new bits to save in the defined position.
 * @return The new value of x
 */
#define SB(x, s, n, d) ((x) = ((x) & ~(((1U << (n)) - 1) << (s))) | ((d) << (s)))

/** Add i to n bits of x starting at bit s.
 *
 * This add the value of i on n bits of x starting at bit s. The parameters x,
 * s, i are similar to #GB besides x must be a variable as the result are
 * saved there. An overflow does not affect the following bits of the given
 * bit window and is simply ignored.
 *
 * @note Parameter x must be a variable as the result is saved there.
 * @param x The variable to add some bits at some position
 * @param s The startposition of the addition
 * @param n The size/window for the addition
 * @param i The value to add at the given startposition in the given window.
 * @return The new value of x
 */
#define AB(x, s, n, i) ((x) = ((x) & ~(((1U << (n)) - 1) << (s))) | (((x) + ((i) << (s))) & (((1U << (n)) - 1) << (s))))

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

/**
 * Returns the maximum of two values.
 *
 * This function returns the greater value of two given values.
 * If they are equal the value of a is returned.
 *
 * @param a The first value
 * @param b The second value
 * @return The greater value or a if equals
 */
template<typename T> static inline T max(const T a, const T b)
{
	return a >= b ? a : b;
}

/**
 * Returns the minimum of two values.
 *
 * This function returns the smaller value of two given values.
 * If they are equal the value of b is returned.
 *
 * @param a The first value
 * @param b The second value
 * @return The smaller value or b if equals
 */
template<typename T> static inline T min(const T a, const T b)
{
	return a < b ? a : b;
}

/**
 * Returns the minimum of two integer.
 *
 * This function returns the smaller value of two given integers.
 *
 * @param a The first integer
 * @param b The second integer
 * @return The smaller value
 */
static inline int min(const int a, const int b)
{
	return a <= b ? a : b;
}

/**
 * Returns the minimum of two unsigned integers.
 *
 * This function returns the smaller value of two given unsigned integers.
 *
 * @param a The first unsigned integer
 * @param b The second unsigned integer
 * @return The smaller value
 */
static inline uint minu(const uint a, const uint b)
{
	return a <= b ? a : b;
}

/**
 * Clamp an integer between an interval.
 *
 * This function returns a value which is between the given interval of
 * min and max. If the given value is in this interval the value itself
 * is returned otherwise the border of the interval is returned, according
 * which side of the interval was 'left'.
 *
 * @note The min value must be less or equal of max or you get some
 *       unexpected results.
 * @param a The value to clamp/truncate.
 * @param min The minimum of the interval.
 * @param max the maximum of the interval.
 * @returns A value between min and max which is closest to a.
 * @see clampu(uint, uint, uint)
 */
static inline int clamp(const int a, const int min, const int max)
{
	if (a <= min) return min;
	if (a >= max) return max;
	return a;
}

/**
 * Clamp an unsigned integer between an interval.
 *
 * This function returns a value which is between the given interval of
 * min and max. If the given value is in this interval the value itself
 * is returned otherwise the border of the interval is returned, according
 * which side of the interval was 'left'.
 *
 * @note The min value must be less or equal of max or you get some
 *       unexpected results.
 * @param a The value to clamp/truncate.
 * @param min The minimum of the interval.
 * @param max the maximum of the interval.
 * @returns A value between min and max which is closest to a.
 * @see clamp(int, int, int)
 */
static inline uint clampu(const uint a, const uint min, const uint max)
{
	if (a <= min) return min;
	if (a >= max) return max;
	return a;
}

/**
 * Reduce a signed 64-bit int to a signed 32-bit one
 *
 * This function clamps a 64-bit integer to a 32-bit integer.
 * If the 64-bit value is smaller than the smallest 32-bit integer
 * value 0x80000000 this value is returned (the left one bit is the sign bit).
 * If the 64-bit value is greater than the greatest 32-bit integer value 0x7FFFFFFF
 * this value is returned. In all other cases the 64-bit value 'fits' in a
 * 32-bits integer field and so the value is casted to int32 and returned.
 *
 * @param a The 64-bit value to clamps
 * @return The 64-bit value reduced to a 32-bit value
 * @see clamp(int, int, int)
 */
static inline int32 ClampToI32(const int64 a)
{
	if (a <= (int32)0x80000000) return 0x80000000;
	if (a >= (int32)0x7FFFFFFF) return 0x7FFFFFFF;
	return (int32)a;
}

/**
 * Multiply two integer values and shift the results to right.
 *
 * This function multiplies two integer values. The result is
 * shifted by the amount of shift to right.
 *
 * @param a The first integer
 * @param b The second integer
 * @param shift The amount to shift the value to right.
 * @return The shifted result
 */
static inline int32 BIGMULSS(const int32 a, const int32 b, const uint8 shift)
{
	return (int32)((int64)a * (int64)b >> shift);
}

/**
 * Multiply two unsigned integers and shift the results to right.
 *
 * This function multiplies two unsigned integers. The result is
 * shifted by the amount of shift to right.
 *
 * @param a The first unsigned integer
 * @param b The second unsigned integer
 * @param shift The amount to shift the value to right.
 * @return The shifted result
 */
static inline uint32 BIGMULUS(const uint32 a, const uint32 b, const uint8 shift)
{
	return (uint32)((uint64)a * (uint64)b >> shift);
}


/**
 * Checks if a value is between a window started at some base point.
 *
 * This macro checks if the value x is between the value of base
 * and base+size. If x equals base this returns true. If x equals
 * base+size this returns false.
 *
 * @param x The value to check
 * @param base The base value of the interval
 * @param size The size of the interval
 * @return True if the value is in the interval, false else.
 */
/* OPT: optimized into an unsigned comparison */
//#define IS_INSIDE_1D(x, base, size) ((x) >= (base) && (x) < (base) + (size))
#define IS_INSIDE_1D(x, base, size) ( (uint)((x) - (base)) < ((uint)(size)) )

/**
 * Checks if a bit in a value is set.
 *
 * This function checks if a bit inside a value is set or not.
 * The y value specific the position of the bit, started at the
 * LSB and count from 0.
 *
 * @param x The value to check
 * @param y The position of the bit to check, started from the LSB
 * @return True if the bit is set, false else.
 */
template<typename T> static inline bool HASBIT(const T x, const uint8 y)
{
	return (x & ((T)1U << y)) != 0;
}

/**
 * Set a bit in a variable.
 *
 * This function sets a bit in a variable. The variable is changed
 * and the value is also returned. Parameter y defines the bit and
 * starts at the LSB with 0.
 *
 * @param x The variable to set a bit
 * @param y The bit position to set
 * @return The new value of the old value with the bit set
 */
template<typename T> static inline T SETBIT(T& x, const uint8 y)
{
	return x |= (T)1U << y;
}

/**
 * Clears a bit in a variable.
 *
 * This function clears a bit in a variable. The variable is
 * changed and the value is also returned. Parameter y defines the bit
 * to clear and starts at the LSB with 0.
 *
 * @param x The variable to clear the bit
 * @param y The bit position to clear
 * @return The new value of the old value with the bit cleared
 */
template<typename T> static inline T CLRBIT(T& x, const uint8 y)
{
	return x &= ~((T)1U << y);
}

/**
 * Toggles a bit in a variable.
 *
 * This function toggles a bit in a variable. The variable is
 * changed and the value is also returned. Parameter y defines the bit
 * to toggle and starts at the LSB with 0.
 *
 * @param x The varliable to toggle the bit
 * @param y The bit position to toggle
 * @return The new value of the old value with the bit toggled
 */
template<typename T> static inline T TOGGLEBIT(T& x, const uint8 y)
{
	return x ^= (T)1U << y;
}


/* checking more bits. Maybe unneccessary, but easy to use */
/**
 * Check several bits in a value.
 *
 * This macro checks if a value contains at least one bit of an other
 * value.
 *
 * @param x The first value
 * @param y The second value
 * @return True if at least one bit is set in both values, false else.
 */
#define HASBITS(x, y) ((x) & (y))

/**
 * Sets several bits in a variable.
 *
 * This macro sets several bits in a variable. The bits to set are provided
 * by a value. The new value is also returned.
 *
 * @param x The variable to set some bits
 * @param y The value with set bits for setting them in the variable
 * @return The new value of x
 */
#define SETBITS(x, y) ((x) |= (y))

/**
 * Clears several bits in a variable.
 *
 * This macro clears several bits in a variable. The bits to clear are
 * provided by a value. The new value is also returned.
 *
 * @param x The variable to clear some bits
 * @param y The value with set bits for clearing them in the variable
 * @return The new value of x
 */
#define CLRBITS(x, y) ((x) &= ~(y))

#define GENERAL_SPRITE_COLOR(color) ((color) + PALETTE_RECOLOR_START)
#define PLAYER_SPRITE_COLOR(owner) (GENERAL_SPRITE_COLOR(_player_colors[owner]))

/**
 * Whether a sprite comes from the original graphics files or a new grf file
 * (either supplied by OpenTTD or supplied by the user).
 *
 * @param sprite The sprite to check
 * @return True if it is a new sprite, or false if it is original.
 */
#define IS_CUSTOM_SPRITE(sprite) ((sprite) >= SPR_SIGNALS_BASE)

extern const byte _ffb_64[128];

/**
 * Returns the first occure of a bit in a 6-bit value (from right).
 *
 * Returns the position of the first bit that is not zero, counted from the
 * LSB. Ie, 110100 returns 2, 000001 returns 0, etc. When x == 0 returns
 * 0.
 *
 * @param x The 6-bit value to check the first zero-bit
 * @return The first position of a bit started from the LSB or 0 if x is 0.
 */
#define FIND_FIRST_BIT(x) _ffb_64[(x)]

/**
 * Returns a value with the first occured of a bit set to zero.
 *
 * Returns x with the first bit from LSB that is not zero set
 * to zero. So, 110100 returns 110000, 000001 returns 000000, etc.
 *
 * @param x The value to returned a new value
 * @return The value which the first bit is set to zero
 */
#define KILL_FIRST_BIT(x) _ffb_64[(x) + 64]

/**
 * Finds the position of the first bit in an integer.
 *
 * This function returns the position of the first bit set in the
 * integer. It does only check the bits of the bitmask
 * 0x3F3F (0011111100111111) and checks only the
 * bits of the bitmask 0x3F00 if and only if the
 * lower part 0x00FF is 0. This results the bits at 0x00C0 must
 * be also zero to check the bits at 0x3F00.
 *
 * @param value The value to check the first bits
 * @return The position of the first bit which is set
 * @see FIND_FIRST_BIT
 */
static inline int FindFirstBit2x64(int value)
{
/*
	int i = 0;
	if ( (byte) value == 0) {
		i += 8;
		value >>= 8;
	}
	return i + FIND_FIRST_BIT(value & 0x3F);

Faster ( or at least cleaner ) implementation below?
*/
	if (GB(value, 0, 8) == 0) {
		return FIND_FIRST_BIT(GB(value, 8, 6)) + 8;
	} else {
		return FIND_FIRST_BIT(GB(value, 0, 6));
	}
}

/**
 * Clear the first bit in an integer.
 *
 * This function returns a value where the first bit (from LSB)
 * is cleared. This function checks, similar to FindFirstBit2x64,
 * the bits at 0x3F3F.
 *
 * @param value The value to clear the first bit
 * @return The new value with the first bit cleared
 * @see KILL_FIRST_BIT
 * @see FindFirstBit2x64
 */
static inline int KillFirstBit2x64(int value)
{
	if (GB(value, 0, 8) == 0) {
		return KILL_FIRST_BIT(GB(value, 8, 6)) << 8;
	} else {
		return value & (KILL_FIRST_BIT(GB(value, 0, 6)) | 0x3F00);
	}
}

/**
 * Counts the number of set bits in a variable.
 *
 * @param value the value to count the number of bits in.
 * @return the number of bits.
 */
template<typename T> static inline uint COUNTBITS(T value)
{
	register uint num;

	/* This loop is only called once for every bit set by clearing the lowest
	 * bit in each loop. The number of bits is therefore equal to the number of
	 * times the loop was called. It was found at the following website:
	 * http://graphics.stanford.edu/~seander/bithacks.html */

	for (num = 0; value != 0; num++) {
		value &= (T)(value - 1);
	}

	return num;
}

/**
 * Checks if a byte is in an interval.
 *
 * This macro returns true if a byte value is in the interval of [min, max).
 *
 * @param a The byte value to check
 * @param min The minimum of the interval
 * @param max The maximum of the interval
 * @see IS_INSIDE_1D
 */
#define IS_BYTE_INSIDE(a, min, max) ((byte)((a) - (min)) < (byte)((max) - (min)))

/**
 * Checks if an int is in an interval.
 *
 * This macro returns true if a integer value is in the interval of [min, max).
 *
 * @param a The integer value to check
 * @param min The minimum of the interval
 * @param max The maximum of the interval
 * @see IS_INSIDE_1D
 */
#define IS_INT_INSIDE(a, min, max) ((uint)((a) - (min)) < (uint)((max) - (min)))

/**
 * Flips a coin with a given probability.
 *
 * This macro can be used to get true or false randomized according to a
 * given probability. The parameter a and b create a percent value with
 * (a/b). The macro returns true in (a/b) percent.
 *
 * @param a The numerator of the fraction
 * @param b The denominator of the fraction, must of course not be null
 * @return True in (a/b) percent
 */
#define CHANCE16(a, b) ((uint16)Random() <= (uint16)((65536 * (a)) / (b)))

/**
 * Flips a coin with a given probability and saves the randomize-number in a variable.
 *
 * This macro uses the same parameters as the CHANCE16 marco. The third parameter
 * must be a variable the randomize-number from Random() is saved in.
 *
 * @param a The numerator of the fraction, see CHANCE16
 * @param b The denominator of the fraction, see CHANCE16
 * @param r The variable to save the randomize-number from Random()
 * @return True in (a/b) percent
 */
#define CHANCE16R(a, b, r) ((uint16)(r = Random()) <= (uint16)((65536 * (a)) / (b)))

/**
 * Checks if a given randomize-number is below a given probability.
 *
 * This macro is used to check if the given probability by the fraction of (a/b)
 * is greater than the given randomize-number v.
 *
 * @param a The numerator of the fraction, see CHANCE16
 * @param b The denominator of the fraction, see CHANCE16
 * @param v The given randomize-number
 * @return True if v is less or equals (a/b)
 */
#define CHANCE16I(a, b, v) ((uint16)(v) <= (uint16)((65536 * (a)) / (b)))


#define for_each_bit(_i, _b)            \
	for (_i = 0; _b != 0; _i++, _b >>= 1) \
		if (_b & 1)

#define abs myabs


static inline uint16 ReadLE16Aligned(const void* x)
{
	return FROM_LE16(*(const uint16*)x);
}

static inline uint16 ReadLE16Unaligned(const void* x)
{
#ifdef OTTD_ALIGNMENT
	return ((const byte*)x)[0] | ((const byte*)x)[1] << 8;
#else
	return FROM_LE16(*(const uint16*)x);
#endif
}


/**
 * ROtate x Left/Right by n (must be >= 0)
 * @note Assumes a byte has 8 bits
 */
#define ROL(x, n) ((x) << (n) | (x) >> (sizeof(x) * 8 - (n)))
#define ROR(x, n) ((x) >> (n) | (x) << (sizeof(x) * 8 - (n)))

/**
 * Return the smallest multiple of n equal or greater than x
 * @note n must be a power of 2
 */
#define ALIGN(x, n) (((x) + (n) - 1) & ~((n) - 1))

/** return the largest value that can be entered in a variable.
 */
#define MAX_UVALUE(type) ((type)~(type)0)

#endif /* MACROS_H */
