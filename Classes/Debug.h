// debug.h

#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define DEBUG_PRINT(...) \
  do { \
      std::cout << __VA_ARGS__ << std::endl; \
  } while(0)
#else
#define DEBUG_PRINT(...) do { } while(0)
#endif

#endif // DEBUG_H
