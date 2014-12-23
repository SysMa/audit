
#ifndef _WC_CES_H
#define _WC_CES_H

#include "ccs.h"

#define WC_CES_T_INDEX		0x0000ff
#define WC_CES_T_TYPE		0x31ff00
#define WC_CES_T_NASCII		0x01fe00
#define WC_CES_T_8BIT		0x100000
#define WC_CES_T_MBYTE		0x200000
#define WC_CES_T_ASCII		0x000100
#define WC_CES_T_ISO_8859	0x000200
#define WC_CES_T_PRIV1		0x000400
#define WC_CES_T_ISO_2022	0x000800
#define WC_CES_T_EUC		0x001000
#define WC_CES_T_PRIV2		0x002000
#define WC_CES_T_VIET		0x004000
#define WC_CES_T_UTF		0x008000
#define WC_CES_T_WTF		0x010000
#define WC_CES_E_ISO_8859	(WC_CES_T_ISO_8859|WC_CES_T_8BIT)
#define WC_CES_E_PRIV1		(WC_CES_T_PRIV1|WC_CES_T_8BIT)
#define WC_CES_E_ISO_2022	(WC_CES_T_ISO_2022|WC_CES_T_MBYTE)
#define WC_CES_E_EUC		(WC_CES_T_EUC|WC_CES_T_8BIT|WC_CES_T_MBYTE)
#define WC_CES_E_PRIV2		(WC_CES_T_PRIV2|WC_CES_T_8BIT|WC_CES_T_MBYTE)
#define WC_CES_E_VIET		(WC_CES_T_VIET|WC_CES_T_PRIV1|WC_CES_T_8BIT)

#define WC_CES_WTF		(WC_CES_T_WTF|WC_CES_T_8BIT|WC_CES_T_MBYTE)

#define WC_CES_INDEX(c)		((c)&WC_CES_T_INDEX)
#define WC_CES_TYPE(c)		((c)&WC_CES_T_TYPE)

enum {
  WC_CES_N_US_ASCII,

  WC_CES_N_ISO_8859_1,
  WC_CES_N_ISO_8859_2,
  WC_CES_N_ISO_8859_3,
  WC_CES_N_ISO_8859_4,
  WC_CES_N_ISO_8859_5,
  WC_CES_N_ISO_8859_6,
  WC_CES_N_ISO_8859_7,
  WC_CES_N_ISO_8859_8,
  WC_CES_N_ISO_8859_9,
  WC_CES_N_ISO_8859_10,
  WC_CES_N_ISO_8859_11,
  WC_CES_N_ISO_8859_12,
  WC_CES_N_ISO_8859_13,
  WC_CES_N_ISO_8859_14,
  WC_CES_N_ISO_8859_15,
  WC_CES_N_ISO_8859_16,

  WC_CES_N_ISO_2022_JP,
  WC_CES_N_ISO_2022_JP_2,
  WC_CES_N_ISO_2022_JP_3,
  WC_CES_N_ISO_2022_CN,
  WC_CES_N_ISO_2022_KR,

  WC_CES_N_EUC_JP,
  WC_CES_N_EUC_CN,
  WC_CES_N_EUC_TW,
  WC_CES_N_EUC_KR,

  WC_CES_N_CP437,
  WC_CES_N_CP737,
  WC_CES_N_CP775,
  WC_CES_N_CP850,
  WC_CES_N_CP852,
  WC_CES_N_CP855,
  WC_CES_N_CP856,
  WC_CES_N_CP857,
  WC_CES_N_CP860,
  WC_CES_N_CP861,
  WC_CES_N_CP862,
  WC_CES_N_CP863,
  WC_CES_N_CP864,
  WC_CES_N_CP865,
  WC_CES_N_CP866,
  WC_CES_N_CP869,
  WC_CES_N_CP874,
  WC_CES_N_CP1006,
  WC_CES_N_CP1250,
  WC_CES_N_CP1251,
  WC_CES_N_CP1252,
  WC_CES_N_CP1253,
  WC_CES_N_CP1254,
  WC_CES_N_CP1255,
  WC_CES_N_CP1256,
  WC_CES_N_CP1257,
  WC_CES_N_KOI8_R,
  WC_CES_N_KOI8_U,
  WC_CES_N_NEXTSTEP,
  WC_CES_N_RAW,

  WC_CES_N_SHIFT_JIS,
  WC_CES_N_SHIFT_JISX0213,
  WC_CES_N_GBK,
  WC_CES_N_GB18030,
  WC_CES_N_HZ_GB_2312,
  WC_CES_N_BIG5,
  WC_CES_N_HKSCS,
  WC_CES_N_UHC,
  WC_CES_N_JOHAB,

  WC_CES_N_CP1258,
  WC_CES_N_TCVN_5712,
  WC_CES_N_VISCII_11,
  WC_CES_N_VPS,

  WC_CES_N_UTF_8,
  WC_CES_N_UTF_7
};

#define WC_CES_US_ASCII		(WC_CES_T_ASCII|WC_CES_N_US_ASCII)

#define WC_CES_ISO_8859_1	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_1)
#define WC_CES_ISO_8859_2	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_2)
#define WC_CES_ISO_8859_3	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_3)
#define WC_CES_ISO_8859_4	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_4)
#define WC_CES_ISO_8859_5	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_5)
#define WC_CES_ISO_8859_6	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_6)
#define WC_CES_ISO_8859_7	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_7)
#define WC_CES_ISO_8859_8	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_8)
#define WC_CES_ISO_8859_9	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_9)
#define WC_CES_ISO_8859_10	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_10)
#define WC_CES_ISO_8859_11	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_11)
#define WC_CES_TIS_620		WC_CES_ISO_8859_11
#define WC_CES_ISO_8859_12	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_12)
				/* not yet exist */
#define WC_CES_ISO_8859_13	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_13)
#define WC_CES_ISO_8859_14	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_14)
#define WC_CES_ISO_8859_15	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_15)
#define WC_CES_ISO_8859_16	(WC_CES_E_ISO_8859|WC_CES_N_ISO_8859_16)

#define WC_CES_ISO_2022_JP	(WC_CES_E_ISO_2022|WC_CES_N_ISO_2022_JP)
#define WC_CES_ISO_2022_JP_2	(WC_CES_E_ISO_2022|WC_CES_N_ISO_2022_JP_2)
#define WC_CES_ISO_2022_JP_3	(WC_CES_E_ISO_2022|WC_CES_N_ISO_2022_JP_3)
#define WC_CES_ISO_2022_CN	(WC_CES_E_ISO_2022|WC_CES_N_ISO_2022_CN)
#define WC_CES_ISO_2022_KR	(WC_CES_E_ISO_2022|WC_CES_N_ISO_2022_KR)

#define WC_CES_EUC_JP		(WC_CES_E_EUC|WC_CES_N_EUC_JP)
#define WC_CES_EUC_CN		(WC_CES_E_EUC|WC_CES_N_EUC_CN)
#define WC_CES_EUC_TW		(WC_CES_E_EUC|WC_CES_N_EUC_TW)
#define WC_CES_EUC_KR		(WC_CES_E_EUC|WC_CES_N_EUC_KR)

#define WC_CES_CP437		(WC_CES_E_PRIV1|WC_CES_N_CP437)
#define WC_CES_CP737		(WC_CES_E_PRIV1|WC_CES_N_CP737)
#define WC_CES_CP775		(WC_CES_E_PRIV1|WC_CES_N_CP775)
#define WC_CES_CP850		(WC_CES_E_PRIV1|WC_CES_N_CP850)
#define WC_CES_CP852		(WC_CES_E_PRIV1|WC_CES_N_CP852)
#define WC_CES_CP855		(WC_CES_E_PRIV1|WC_CES_N_CP855)
#define WC_CES_CP856		(WC_CES_E_PRIV1|WC_CES_N_CP856)
#define WC_CES_CP857		(WC_CES_E_PRIV1|WC_CES_N_CP857)
#define WC_CES_CP860		(WC_CES_E_PRIV1|WC_CES_N_CP860)
#define WC_CES_CP861		(WC_CES_E_PRIV1|WC_CES_N_CP861)
#define WC_CES_CP862		(WC_CES_E_PRIV1|WC_CES_N_CP862)
#define WC_CES_CP863		(WC_CES_E_PRIV1|WC_CES_N_CP863)
#define WC_CES_CP864		(WC_CES_E_PRIV1|WC_CES_N_CP864)
#define WC_CES_CP865		(WC_CES_E_PRIV1|WC_CES_N_CP865)
#define WC_CES_CP866		(WC_CES_E_PRIV1|WC_CES_N_CP866)
#define WC_CES_CP869		(WC_CES_E_PRIV1|WC_CES_N_CP869)
#define WC_CES_CP874		(WC_CES_E_PRIV1|WC_CES_N_CP874)
#define WC_CES_CP1006		(WC_CES_E_PRIV1|WC_CES_N_CP1006)
#define WC_CES_CP1250		(WC_CES_E_PRIV1|WC_CES_N_CP1250)
#define WC_CES_CP1251		(WC_CES_E_PRIV1|WC_CES_N_CP1251)
#define WC_CES_CP1252		(WC_CES_E_PRIV1|WC_CES_N_CP1252)
#define WC_CES_CP1253		(WC_CES_E_PRIV1|WC_CES_N_CP1253)
#define WC_CES_CP1254		(WC_CES_E_PRIV1|WC_CES_N_CP1254)
#define WC_CES_CP1255		(WC_CES_E_PRIV1|WC_CES_N_CP1255)
#define WC_CES_CP1256		(WC_CES_E_PRIV1|WC_CES_N_CP1256)
#define WC_CES_CP1257		(WC_CES_E_PRIV1|WC_CES_N_CP1257)
#define WC_CES_KOI8_R		(WC_CES_E_PRIV1|WC_CES_N_KOI8_R)
#define WC_CES_KOI8_U		(WC_CES_E_PRIV1|WC_CES_N_KOI8_U)
#define WC_CES_NEXTSTEP		(WC_CES_E_PRIV1|WC_CES_N_NEXTSTEP)
#define WC_CES_RAW		(WC_CES_E_PRIV1|WC_CES_N_RAW)

#define WC_CES_SHIFT_JIS	(WC_CES_E_PRIV2|WC_CES_N_SHIFT_JIS)
#define WC_CES_CP932		WC_CES_SHIFT_JIS
#define WC_CES_SHIFT_JISX0213	(WC_CES_E_PRIV2|WC_CES_N_SHIFT_JISX0213)
#define WC_CES_GBK		(WC_CES_E_PRIV2|WC_CES_N_GBK)
#define WC_CES_CP936		WC_CES_GBK
#define WC_CES_GB18030		(WC_CES_E_PRIV2|WC_CES_N_GB18030)
#define WC_CES_HZ_GB_2312	(WC_CES_T_PRIV2|WC_CES_T_MBYTE|WC_CES_N_HZ_GB_2312)
#define WC_CES_BIG5		(WC_CES_E_PRIV2|WC_CES_N_BIG5)
#define WC_CES_CP950		WC_CES_BIG5
#define WC_CES_HKSCS		(WC_CES_E_PRIV2|WC_CES_N_HKSCS)
#define WC_CES_UHC		(WC_CES_E_PRIV2|WC_CES_N_UHC)
#define WC_CES_CP949		WC_CES_UHC
#define WC_CES_JOHAB		(WC_CES_E_PRIV2|WC_CES_N_JOHAB)

#define WC_CES_CP1258		(WC_CES_E_PRIV1|WC_CES_N_CP1258)
#define WC_CES_TCVN_5712	(WC_CES_E_VIET|WC_CES_N_TCVN_5712)
#define WC_CES_VISCII_11	(WC_CES_E_VIET|WC_CES_N_VISCII_11)
#define WC_CES_VPS		(WC_CES_E_VIET|WC_CES_N_VPS)

#define WC_CES_UTF_8		(WC_CES_T_UTF|WC_CES_T_8BIT|WC_CES_T_MBYTE|WC_CES_N_UTF_8)
#define WC_CES_UTF_7		(WC_CES_T_UTF|WC_CES_T_MBYTE|WC_CES_N_UTF_7)

#define WC_CES_END		WC_CES_N_UTF_7

#endif
