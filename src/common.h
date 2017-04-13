#ifndef NFF_COMMON_H
#define NFF_COMMON_H
extern "C" {
	#include <nfc/nfc.h>
}

#define NFF_ERROR_OPEN_DEVICE 11
#define NFF_ERROR_INIT_LIBNFC 10

/* LibNFC errors binding */
#define NFF_ERROR_LIBNFC_UNKNOWN 100
#define NFF_ERROR_LIBNFC_EIO 101
#define NFF_ERROR_LIBNFC_EINVARG 102
#define NFF_ERROR_LIBNFC_EDEVNOTSUPP 103
#define NFF_ERROR_LIBNFC_ENOTSUCHDEV 104
#define NFF_ERROR_LIBNFC_EOVFLOW 105
#define NFF_ERROR_LIBNFC_ETIMEOUT 106
#define NFF_ERROR_LIBNFC_EOPABORTED 107
#define NFF_ERROR_LIBNFC_ENOTIMPL 108
#define NFF_ERROR_LIBNFC_ETGRELEASED 109
#define NFF_ERROR_LIBNFC_ERFTRANS 110
#define NFF_ERROR_LIBNFC_EMFCAUTHFAIL 111
#define NFF_ERROR_LIBNFC_ESOFT 112
#define NFF_ERROR_LIBNFC_ECHIP 113
#define LIBNFC_ERROR_TO_NFF(error) \
(error == NFC_EIO) ? NFF_ERROR_LIBNFC_EIO: \
(error == NFC_EINVARG) ? NFF_ERROR_LIBNFC_EINVARG: \
(error == NFC_EDEVNOTSUPP) ? NFF_ERROR_LIBNFC_EDEVNOTSUPP: \
(error == NFC_ENOTSUCHDEV) ? NFF_ERROR_LIBNFC_ENOTSUCHDEV: \
(error == NFC_EOVFLOW) ? NFF_ERROR_LIBNFC_EOVFLOW: \
(error == NFC_ETIMEOUT) ? NFF_ERROR_LIBNFC_ETIMEOUT: \
(error == NFC_EOPABORTED) ? NFF_ERROR_LIBNFC_EOPABORTED: \
(error == NFC_ENOTIMPL) ? NFF_ERROR_LIBNFC_ENOTIMPL: \
(error == NFC_ETGRELEASED) ? NFF_ERROR_LIBNFC_ETGRELEASED: \
(error == NFC_ERFTRANS) ? NFF_ERROR_LIBNFC_ERFTRANS: \
(error == NFC_EMFCAUTHFAIL) ? NFF_ERROR_LIBNFC_EMFCAUTHFAIL: \
(error == NFC_ESOFT) ? NFF_ERROR_LIBNFC_ESOFT: \
(error == NFC_ECHIP) ? NFF_ERROR_LIBNFC_ECHIP: \
NFF_ERROR_LIBNFC_UNKNOWN


#define NFF_MAX_DEVICES 10


// Dirty hack to be compatible with last git version of libfreefare
#ifdef FELICA_SC_RW
#define MifareTag FreefareTag
#define mifare_tag_type freefare_tag_type
#define CLASSIC_1K MIFARE_CLASSIC_1K
#define CLASSIC_4K MIFARE_CLASSIC_4K
#define DESFIRE MIFARE_DESFIRE
#define ULTRALIGHT MIFARE_ULTRALIGHT
#define ULTRALIGHT_C MIFARE_ULTRALIGHT_C
#define MifareTag FreefareTag
#endif




// Global vars
extern nfc_context* libnfc_context;



#endif /* NFF_COMMON_H */
