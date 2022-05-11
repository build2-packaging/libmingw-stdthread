#include "throw.hxx"

e::
e (char c)
{
  data[0] = c;
}

void
t (char c)
{
  throw e (c);
}
