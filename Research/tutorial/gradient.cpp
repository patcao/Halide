#include <iostream>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

extern "C" void *halide_malloc(void *ctx, size_t);
extern "C" void halide_free(void *ctx, void *ptr);
extern "C" int halide_debug_to_file(void *ctx, const char *filename, void *data, int, int, int, int, int, int);
extern "C" int halide_start_clock(void *ctx);
extern "C" int64_t halide_current_time_ns(void *ctx);
extern "C" uint64_t halide_profiling_timer(void *ctx);
extern "C" int halide_printf(void *ctx, const char *fmt, ...);

#ifdef _WIN32
extern "C" float roundf(float);
extern "C" double round(double);
#else
inline float asinh_f32(float x) {return asinhf(x);}
inline float acosh_f32(float x) {return acoshf(x);}
inline float atanh_f32(float x) {return atanhf(x);}
inline double asinh_f64(double x) {return asinh(x);}
inline double acosh_f64(double x) {return acosh(x);}
inline double atanh_f64(double x) {return atanh(x);}
#endif
inline float sqrt_f32(float x) {return sqrtf(x);}
inline float sin_f32(float x) {return sinf(x);}
inline float asin_f32(float x) {return asinf(x);}
inline float cos_f32(float x) {return cosf(x);}
inline float acos_f32(float x) {return acosf(x);}
inline float tan_f32(float x) {return tanf(x);}
inline float atan_f32(float x) {return atanf(x);}
inline float sinh_f32(float x) {return sinhf(x);}
inline float cosh_f32(float x) {return coshf(x);}
inline float tanh_f32(float x) {return tanhf(x);}
inline float hypot_f32(float x, float y) {return hypotf(x, y);}
inline float exp_f32(float x) {return expf(x);}
inline float log_f32(float x) {return logf(x);}
inline float pow_f32(float x, float y) {return powf(x, y);}
inline float floor_f32(float x) {return floorf(x);}
inline float ceil_f32(float x) {return ceilf(x);}
inline float round_f32(float x) {return roundf(x);}

inline double sqrt_f64(double x) {return sqrt(x);}
inline double sin_f64(double x) {return sin(x);}
inline double asin_f64(double x) {return asin(x);}
inline double cos_f64(double x) {return cos(x);}
inline double acos_f64(double x) {return acos(x);}
inline double tan_f64(double x) {return tan(x);}
inline double atan_f64(double x) {return atan(x);}
inline double sinh_f64(double x) {return sinh(x);}
inline double cosh_f64(double x) {return cosh(x);}
inline double tanh_f64(double x) {return tanh(x);}
inline double hypot_f64(double x, double y) {return hypot(x, y);}
inline double exp_f64(double x) {return exp(x);}
inline double log_f64(double x) {return log(x);}
inline double pow_f64(double x, double y) {return pow(x, y);}
inline double floor_f64(double x) {return floor(x);}
inline double ceil_f64(double x) {return ceil(x);}
inline double round_f64(double x) {return round(x);}

inline float maxval_f32() {return FLT_MAX;}
inline float minval_f32() {return -FLT_MAX;}
inline double maxval_f64() {return DBL_MAX;}
inline double minval_f64() {return -DBL_MAX;}
inline uint8_t maxval_u8() {return 0xff;}
inline uint8_t minval_u8() {return 0;}
inline uint16_t maxval_u16() {return 0xffff;}
inline uint16_t minval_u16() {return 0;}
inline uint32_t maxval_u32() {return 0xffffffff;}
inline uint32_t minval_u32() {return 0;}
inline uint64_t maxval_u64() {return 0xffffffffffffffff;}
inline uint64_t minval_u64() {return 0;}
inline int8_t maxval_s8() {return 0x7f;}
inline int8_t minval_s8() {return 0x80;}
inline int16_t maxval_s16() {return 0x7fff;}
inline int16_t minval_s16() {return 0x8000;}
inline int32_t maxval_s32() {return 0x7fffffff;}
inline int32_t minval_s32() {return 0x80000000;}
inline int64_t maxval_s64() {return 0x7fffffffffffffff;}
inline int64_t minval_s64() {return 0x8000000000000000;}

inline int8_t abs_i8(int8_t a) {return a >= 0 ? a : -a;}
inline int16_t abs_i16(int16_t a) {return a >= 0 ? a : -a;}
inline int32_t abs_i32(int32_t a) {return a >= 0 ? a : -a;}
inline int64_t abs_i64(int64_t a) {return a >= 0 ? a : -a;}
inline float abs_f32(float a) {return fabsf(a);}
inline double abs_f64(double a) {return fabs(a);}

inline float nan_f32() {return NAN;}
inline float neg_inf_f32() {return -INFINITY;}
inline float inf_f32() {return INFINITY;}
inline float float_from_bits(uint32_t bits) {
 union {
  uint32_t as_uint;
  float as_float;
 } u;
 u.as_uint = bits;
 return u.as_float;
}

template<typename T> T max(T a, T b) {if (a > b) return a; return b;}
template<typename T> T min(T a, T b) {if (a < b) return a; return b;}
template<typename T> T mod(T a, T b) {T result = a % b; if (result < 0) result += b; return result;}
template<typename T> T sdiv(T a, T b) {return (a - mod(a, b))/b;}
template<typename A, typename B> A reinterpret(B b) {A a; memcpy(&a, &b, sizeof(a)); return a;}

#ifndef BUFFER_T_DEFINED
#define BUFFER_T_DEFINED
#include <stdint.h>
typedef struct buffer_t {
    uint64_t dev;
    uint8_t* host;
    int32_t extent[4];
    int32_t stride[4];
    int32_t min[4];
    int32_t elem_size;
    bool host_dirty;
    bool dev_dirty;
} buffer_t;
#endif
bool halide_rewrite_buffer(buffer_t *b, int32_t elem_size,
                           int32_t min0, int32_t extent0, int32_t stride0,
                           int32_t min1, int32_t extent1, int32_t stride1,
                           int32_t min2, int32_t extent2, int32_t stride2,
                           int32_t min3, int32_t extent3, int32_t stride3) {
 b->min[0] = min0;
 b->min[1] = min1;
 b->min[2] = min2;
 b->min[3] = min3;
 b->extent[0] = extent0;
 b->extent[1] = extent1;
 b->extent[2] = extent2;
 b->extent[3] = extent3;
 b->stride[0] = stride0;
 b->stride[1] = stride1;
 b->stride[2] = stride2;
 b->stride[3] = stride3;
 return true;
}


extern "C" int gradient(buffer_t *_gradient_buffer) {
int32_t *_gradient = (int32_t *)(_gradient_buffer->host);
const bool _gradient_host_and_dev_are_null = (_gradient_buffer->host == NULL) && (_gradient_buffer->dev == 0);
(void)_gradient_host_and_dev_are_null;
const int32_t _gradient_min_0 = _gradient_buffer->min[0];
(void)_gradient_min_0;
const int32_t _gradient_min_1 = _gradient_buffer->min[1];
(void)_gradient_min_1;
const int32_t _gradient_min_2 = _gradient_buffer->min[2];
(void)_gradient_min_2;
const int32_t _gradient_min_3 = _gradient_buffer->min[3];
(void)_gradient_min_3;
const int32_t _gradient_extent_0 = _gradient_buffer->extent[0];
(void)_gradient_extent_0;
const int32_t _gradient_extent_1 = _gradient_buffer->extent[1];
(void)_gradient_extent_1;
const int32_t _gradient_extent_2 = _gradient_buffer->extent[2];
(void)_gradient_extent_2;
const int32_t _gradient_extent_3 = _gradient_buffer->extent[3];
(void)_gradient_extent_3;
const int32_t _gradient_stride_0 = _gradient_buffer->stride[0];
(void)_gradient_stride_0;
const int32_t _gradient_stride_1 = _gradient_buffer->stride[1];
(void)_gradient_stride_1;
const int32_t _gradient_stride_2 = _gradient_buffer->stride[2];
(void)_gradient_stride_2;
const int32_t _gradient_stride_3 = _gradient_buffer->stride[3];
(void)_gradient_stride_3;
const int32_t _gradient_elem_size = _gradient_buffer->elem_size;
if (_gradient_host_and_dev_are_null)
{
 bool _0 = halide_rewrite_buffer(_gradient_buffer, 4, _gradient_min_0, _gradient_extent_0, 1, _gradient_min_1, _gradient_extent_1, _gradient_extent_0, 0, 0, 0, 0, 0, 0);
 (void)_0;
} // if _gradient_host_and_dev_are_null
bool _1 = !(_gradient_host_and_dev_are_null);
if (_1)
{
 bool _2 = _gradient_elem_size == 4;
 if (!_2) {
  halide_printf(NULL, "Output buffer gradient has type int32, but elem_size of the buffer_t passed in is %d instead of 4\n", _gradient_elem_size);
  return -1;
 }
 bool _3 = _gradient_stride_0 == 1;
 if (!_3) {
  halide_printf(NULL, "Static constraint violated: gradient.stride.0 == 1\n");
  return -1;
 }
 int64_t _4 = (int64_t)(_gradient_extent_0);
 int64_t _5 = (int64_t)(_gradient_extent_1);
 int64_t _6 = (int64_t)(2147483647);
 bool _7 = _4 <= _6;
 if (!_7) {
  halide_printf(NULL, "Total allocation for buffer gradient exceeds 2^31 - 1\n");
  return -1;
 }
 int64_t _8 = (int64_t)(_gradient_stride_1);
 int64_t _9 = _5 * _8;
 bool _10 = _9 <= _6;
 if (!_10) {
  halide_printf(NULL, "Total allocation for buffer gradient exceeds 2^31 - 1\n");
  return -1;
 }
 int64_t _11 = _5 * _4;
 bool _12 = _11 <= _6;
 if (!_12) {
  halide_printf(NULL, "Product of extents for buffer gradient exceeds 2^31 - 1\n");
  return -1;
 }
 // produce gradient
 for (int _gradient_s0_y = _gradient_min_1; _gradient_s0_y < _gradient_min_1 + _gradient_extent_1; _gradient_s0_y++)
 {
  for (int _gradient_s0_x = _gradient_min_0; _gradient_s0_x < _gradient_min_0 + _gradient_extent_0; _gradient_s0_x++)
  {
   int32_t _13 = _gradient_s0_x - _gradient_min_0;
   int32_t _14 = _gradient_s0_y - _gradient_min_1;
   int32_t _15 = _14 * _gradient_stride_1;
   int32_t _16 = _13 + _15;
   int32_t _17 = _gradient_s0_x + _gradient_s0_y;
   _gradient[_16] = _17;
  } // for _gradient_s0_x
 } // for _gradient_s0_y
 // consume gradient
} // if _1
return 0;
}
