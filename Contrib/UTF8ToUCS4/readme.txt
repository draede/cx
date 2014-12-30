UTF-8 <-> Unicode convertion library

It's an implementation of RFC3629. It can convert UCS4 (wchar_t) strings into UTF-8 string and vice versa. I plan to add support of streaming when converting from UTF-8 and support of specifying different endians (not one that local machine has).