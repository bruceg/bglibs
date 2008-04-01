int test_short(char* ptr);
int test_int(char* ptr);
int test_long(char* ptr);
int test_longlong(char* ptr);
int main(void)
{
  long buf[2] = {0,0};		/* will be aligned */
  char* ptr = ((char*)buf) + 1; /* will be unaligned */
  return (test_short(ptr) +
	  test_int(ptr) +
	  test_long(ptr) +
	  test_longlong(ptr)) == 10 ? 0 : 1;
}
int test_short(char* ptr) { return ++*((unsigned short*)ptr); }
int test_int(char* ptr) { return ++*((unsigned int*)ptr); }
int test_long(char* ptr) { return ++*((unsigned long*)ptr); }
int test_longlong(char* ptr) { return ++*((unsigned long long*)ptr); }
