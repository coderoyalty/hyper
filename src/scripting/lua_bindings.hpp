#ifndef EXPOSE_HPP
#define EXPOSE_HPP

#include <sol/sol.hpp>

namespace wow
{
  namespace binding
  {
    void bindEngineClasses(sol::state &lua);
  };
};

#endif