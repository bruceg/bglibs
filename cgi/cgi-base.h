#ifndef CHECKVPW__CGI_BASE__H__
#define CHECKVPW__CGI_BASE__H__

#include "mystring/mystring.h"
#include "cgi/cgi-args.h"

extern void content_type(mystring ct);

class CGI
{
protected:
  const CGIArgs& args;

  const mystring redirect_url, error_redirect_url;
  const mystring vdomain, password;
  
  void error(mystring);
  void success(mystring);

  void must_be_defined(mystring);
  
public:
  CGI(const CGIArgs& a);
  
  void main();
};

#define CGI_MAIN void CGI::main()

#define CGI_INPUT(VAR) \
  const mystring VAR##__name__ = #VAR; \
  must_be_defined(VAR##__name__); \
  mystring VAR = args[VAR##__name__];

#define CGI_OPTINPUT(VAR) \
  const mystring VAR##__name__ = #VAR; \
  mystring VAR = args[VAR##__name__];

#endif
