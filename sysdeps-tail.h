/* These macros are used to produce the declarations and inline
 * definitions for the various uint## pack/unpack msb/lsb functions */

#if defined(__GNUC__) && defined(HAS_UNALIGNED_ACCESSES) && defined(ENDIAN_LSB)
#define __INLINE_UINT_LSB 1
#undef __INLINE_UINT_MSB
#endif

#if defined(__GNUC__) && defined(HAS_UNALIGNED_ACCESSES) && defined(ENDIAN_MSB)
#define __INLINE_UINT_MSB 1
#undef __INLINE_UINT_LSB
#endif

#ifdef __INLINE_UINT_LSB
#define __UINTNN_GET_LSB_DECL(B,N) \
  static __inline__ uint##B uint##B##_get_lsb(const unsigned char b[N]) \
  { return *((const uint##B *)b); }
#define __UINTNN_PACK_LSB_DECL(B,N) \
  static __inline__ void uint##B##_pack_lsb(uint##B u, unsigned char b[N]) \
  { *((uint##B *)b) = u; }
#define __UINTNN_UNPACK_LSB_DECL(B,N) \
  static __inline__ void uint##B##_unpack_lsb(const unsigned char b[N], uint##B *u) \
  { *u = *((uint##B *)b); }
#else
#define __UINTNN_GET_LSB_DECL(B,N) \
  extern uint##B uint##B##_get_lsb(const unsigned char[N]);
#define __UINTNN_PACK_LSB_DECL(B,N) \
  extern void uint##B##_pack_lsb(uint##B, unsigned char[N]);
#define __UINTNN_UNPACK_LSB_DECL(B,N) \
  extern void uint##B##_unpack_lsb(const unsigned char[N], uint##B *);
#endif

#ifdef __INLINE_UINT_MSB
#define __UINTNN_GET_MSB_DECL(B,N) \
  static __inline__ uint##B uint##B##_get_msb(const unsigned char b[N]) \
  { return *((const uint##B *)b); }
#define __UINTNN_PACK_MSB_DECL(B,N) \
  static __inline__ void uint##B##_pack_msb(uint##B u, unsigned char b[N]) \
  { *((uint##B *)b) = u; }
#define __UINTNN_UNPACK_MSB_DECL(B,N) \
  static __inline__ void uint##B##_unpack_msb(const unsigned char b[N], uint##B *u) \
  { *u = *((uint##B *)b); }
#else
#define __UINTNN_GET_MSB_DECL(B,N) \
  extern uint##B uint##B##_get_msb(const unsigned char[N]);
#define __UINTNN_PACK_MSB_DECL(B,N) \
  extern void uint##B##_pack_msb(uint##B, unsigned char[N]);
#define __UINTNN_UNPACK_MSB_DECL(B,N) \
  extern void uint##B##_unpack_msb(const unsigned char[N], uint##B *);
#endif

#define __UINTNN_DECL(B,N) \
  __UINTNN_GET_LSB_DECL(B,N) \
  __UINTNN_PACK_LSB_DECL(B,N) \
  __UINTNN_UNPACK_LSB_DECL(B,N) \
  __UINTNN_GET_MSB_DECL(B,N) \
  __UINTNN_PACK_MSB_DECL(B,N) \
  __UINTNN_UNPACK_MSB_DECL(B,N)

