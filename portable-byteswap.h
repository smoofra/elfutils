
#if defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define bswap_16(x) OSSwapInt16(x)
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)
#define htobe64(x) htonll(x)
#define be64toh(x) ntohll(x)
#define be32toh(x) ntohl(x)
#if BYTE_ORDER == LITTLE_ENDIAN
#define le16toh(x) x
#define le32toh(x) x
#define le64toh(x) x
#endif

#else

#include <byteswap.h>

#endif