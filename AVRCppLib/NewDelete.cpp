/**********************************************************************************************************************\

	C++ library for Atmel AVR microcontrollers
	Copyright (C) 2007 Lauri Kirikal, Mikk Leini, MTÜ TTÜ Robotiklubi

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

	See http://creativecommons.org/licenses/GPL/2.0/

	MTÜ TTÜ Robotiklubi  http://www.robotiklubi.ee robotiklubi@gmail.com
	Lauri Kirikal        laurikirikal@gmail.com
	Mikk Leini           mikk.leini@gmail.com

\**********************************************************************************************************************/

extern "C++" {

	namespace std
	{
		typedef unsigned int size_t;
		typedef void (*new_handler)();
		new_handler set_new_handler(new_handler) throw();
	}
	
	void operator delete(void*) throw();
	void operator delete[](void*) throw();
	void* operator new(std::size_t) throw();
	void* operator new[](std::size_t) throw();
	void operator delete(void*) throw();
	void operator delete[](void*) throw();

}

using std::new_handler;
extern "C" void *malloc (std::size_t);
extern "C" void free(void *);
extern "C" void abort() __attribute__((noreturn));
extern "C" void __cxa_pure_virtual();
new_handler __new_handler;

void * operator new (std::size_t sz) throw()
{
    void *p;
    if (sz == 0)
        sz = 1;
    p = (void*) malloc(sz);

    while (p == 0)
    {
        new_handler handler = __new_handler;
        if (! handler)
            return 0;
        handler();
        p = (void *) malloc(sz);
    }

    return p;
}

void * operator new[] (std::size_t sz) throw()
{
  return ::operator new(sz);
}

void operator delete (void *ptr) throw ()
{
  if (ptr)
    free (ptr);
}

void operator delete[] (void *ptr) throw ()
{
  ::operator delete (ptr);
}

void __cxa_pure_virtual()
{
    abort();
}


