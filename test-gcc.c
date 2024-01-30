#include "stdio.h"

int main()
{

	int (*build_me())()
	{

		int i = 42;
		int f()
		{
			return i;
		}
		return f;

	};

	__auto_type g = build_me();
	printf("%i", g());

}
