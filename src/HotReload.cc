#include "pub/HotReload.hh"

#include <cstdio>
#include <thread>

void* Load(const char* filepath)
{
  return dlopen(filepath, RTLD_NOW | RTLD_LOCAL);
}

void* LoadSymbol(void* library, const char* symbol)
{
  return dlsym(library, symbol);
}

void Reload(void*& library, const char* filepath)
{
  if(library)
  {
    dlclose(library);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  library = Load(filepath);
}

void PrintError()
{
  printf("Error: %s\n", dlerror());
}
