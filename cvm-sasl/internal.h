#ifndef CVM_SASL__INTERNAL__H__
#define CVM_SASL__INTERNAL__H__

extern const char* sasl_mech_cvm;

extern int sasl_login_start(const str*, str*);
extern int sasl_login_response(const str*, str*);

extern int sasl_plain_start(const str*, str*);
extern int sasl_plain_response(const str*, str*);

extern int sasl_cram_md5_start(const str*, str*);
extern int sasl_cram_md5_response(const str*, str*);

extern int sasl_authenticate(const char* account, const char** creds);

#endif
