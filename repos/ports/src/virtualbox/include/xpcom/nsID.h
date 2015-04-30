#ifndef nsID_h__
#define nsID_h__

typedef struct { char x [sizeof(RTUUID)]; } nsID; /* differs from original */

typedef nsID nsCID;

#define NS_DEFINE_STATIC_IID_ACCESSOR(x)

#endif
