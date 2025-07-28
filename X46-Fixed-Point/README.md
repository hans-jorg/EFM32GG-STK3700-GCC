46 Using Fixed-Point Arithmetic
===============================

## Introduction

When there is a need to work with fractional values, it is possible to use scaling to represent them
with integer values. For example, when working with money, it will be easy when the units are cents.

When using integer arithmetic, it is possible to avoid the need of a Floating Point Unit (FPU) and
use cheaper devices.

In C, there are integer with different sizes, 8, 16, 32, 64 and sometimes 128 bits.

> Altough it is not specified by the C standard, most compilers and processors use arithmetic right
shift for signed types and logical right shift for unsigned types. There is no problem with left
shifts because they are equal.


## Notation

The notation used in the proposal for the C commitee uses **[s][I].F**, where s is the sign;
**I** the number of bits of the integer part and **F** the number of bits of the fractional part.

Another notation was introduced by Texas Instruments for their Digital Signal Processor (DSP). It is
called the Q notation. A format is described by **[U]Q[I]F**, where **I** is the number of bits
of the integer part and **F**, of the fractional part. **U** is used to specify an unsigned format.

A short cut is *QF* means **F** the number of fractional bits and it is a signed number. But
it does not specifies the number of bits of the integer part. It is actually only the position
of the binary point.

> The ARM variation of this notation explicitly considers the sign bit counted in the integer part.

### Using integer types


### Using (non-standard) Fixed-Point Types

There is a proposal for fixed point arithmetic in C, but is not currently approved [2].

| Type                              | Size    |  Q notation     | Constant suffix   |
|-----------------------------------|---------|-----------------|-------------------|
| signed short _Fract               |  s.7    |     Q.7         |        hr         |
| signed short _Accum               |  s4.7   |     Q.7         |        hk         |
| signed _Fract                     |  s.15   |     Q.15        |        r          |
| signed _Accum                     |  s4.15  |     Q4.15       |        k          |
| signed long _Fract                |  s.23   |     Q.23        |        lr         |
| signed long _Accum                |  s4.23  |     Q4.23       |        lk         |
| unsigned short _Fract             |  . 7    |     UQ.7        |        uhr        |
| unsigned short _Accum             |  4.7    |     UQ.7        |        uhk        |
| unsigned _Fract                   |  .15    |     UQ.15       |        ur         |
| unsigned _Accum                   |  4.15   |     UQ4.15      |        uk         |
| unsigned long _Fract              |  .23    |     UQ.23       |        ulr        |
| unsigned long _Accum              |   4.23  |     UQ4.23      |        ulk        |

There is an addition keyword, **__Sat** that specifies saturarion arithmetic for a specific data.


There is a header file **stdfix.h**, that defines some symbols like **fract**, **accum** and
**sat** for __Fract, _Accum and _Sat, respectively.

The increment and decrement operators are defined for these fixed-point types.

>> The processing of fixed point data can be modified by certain pragmas:

* FX_FULL_PRECISION:
*

There are some functions defined for these data type.

* abs**f**
* round**f**:
* count1s**f**: largest integer greater than parameter
* bits**f**: returns integer representation of data
* strfo**f**: converts string to fixed point data

There is additions format specifiers for printf/fprintf:

* r: fract types
* R: unsigned fract types
* k: accum types
* K: unsigned accum types.

They can be modified by **h** for short types and **l** for long types.


when **f** is hr,r,lr,hk,k or lk.


### Using processor specific instructions


### Using libraries


## Annex A - Half Precision Floating Point

For ARM processors, GCC supports halt precision floating point values and processing.

They are defined by the **__fp16** keyword and mimics the IEEE 754 floating point format.

The use 16 bits with the following format:

* Sign bit
* 10-bits mantissa (fractional)
* 5-bit exponent

There are two incompatible representations:

* **ieee*: Normalizaed values in the range 2^(-14) to 65504. Specified by the **mfp16-format=ieee**
 compilation flag.
* **arm**: Similar but does not support infinities and NaNs. The range is 2^(-14) to 131008.
Specified by the **mfp16-format=alternative** compilation flag.

For ARM processors with VFP and NEON floating point processors, the compiler uses the FPU when
instructed (**mfpu=neon-fp16** compilation flag).

There are intrinsics defined in the *arm-fp16.h** header file.

> For portability, it is recommended to use the *_Float16** type

> There is yeat another 16-bit floating point type: Brain Floating Point, BFloat16 (bf16). It is
 used in NVIDIA GPUs, among others. It has sign, 8-bit exponents and 7-bit mantissa (with hidden 1)


## Annex B - Conversions between _fp16 and float.


From [8].

    typedef unsigned short ushort;
    typedef unsigned int uint;

    uint as_uint(const float x) {
        return *(uint*)&x;
    }
    float as_float(const uint x) {
        return *(float*)&x;
    }

    float half_to_float(const ushort x) { // IEEE-754 16-bit floating-point format (without infinity): 1-5-10, exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
        const uint e = (x&0x7C00)>>10; // exponent
        const uint m = (x&0x03FF)<<13; // mantissa
        const uint v = as_uint((float)m)>>23; // evil log2 bit hack to count leading zeros in denormalized format
        return as_float((x&0x8000)<<16 | (e!=0)*((e+112)<<23|m) | ((e==0)&(m!=0))*((v-37)<<23|((m<<(150-v))&0x007FE000))); // sign : normalized : denormalized
    }
    ushort float_to_half(const float x) { // IEEE-754 16-bit floating-point format (without infinity): 1-5-10, exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
        const uint b = as_uint(x)+0x00001000; // round-to-nearest-even: add last bit after truncated mantissa
        const uint e = (b&0x7F800000)>>23; // exponent
        const uint m = b&0x007FFFFF; // mantissa; in line below: 0x007FF000 = 0x00800000-0x00001000 = decimal indicator flag - initial rounding
        return (b&0x80000000)>>16 | (e>112)*((((e-112)<<10)&0x7C00)|m>>13) | ((e<113)&(e>101))*((((0x007FF000+m)>>(125-e))+1)>>1) | (e>143)*0x7FFF; // sign : normalized : denormalized : saturate
}

## References

1. [Fixed-point arithmetic](https://en.wikipedia.org/wiki/Fixed-point_arithmetic)
2. [Randy Yates-Fixed-Point Arithmetic: An Introduction](http://www.digitalsignallabs.com/downloads/fp.pdf)
2. [ARM](https://developer.arm.com/documentation/dai0033/latest/)
3. [A Fixed-Point Introduction by Example](https://www.dsprelated.com/showarticle/139.php)
2. [Q (number format)](https://en.wikipedia.org/wiki/Q_(number_format))
3. Texas Instruments. TMS320C64x DSP Library Programmer’s Reference. 2003.
2. [GCC Fixed-Point Types](https://gcc.gnu.org/onlinedocs/gcc/Fixed-Point.html)
3. [ISO/IEC DTR 18037.2](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1005.pdf)
6. [GCC Half precision floating point](https://gcc.gnu.org/onlinedocs/gcc/Half-Precision.html)
7. [2-bit to 16-bit Floating Point Conversions](https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion/60047308#60047308)
8. [bfloat16 floating-point format](https://en.wikipedia.org/wiki/Bfloat16_floating-point_format)
