#include "lua_bindings.hpp"
#include "shader.hpp"

namespace lua_bindings
{
  void exposeToLua(sol::state &lua);
}

void wow::binding::bindEngineClasses(sol::state &lua)
{
  lua_bindings::exposeToLua(lua);
}

namespace sol
{
  template <>
  struct lua_type_of<Ref<Shader>> : std::integral_constant<sol::type, sol::type::userdata>
  {
  };

  template <>
  struct usertype_traits<std::shared_ptr<Shader>>
  {
    static bool get(lua_State *L, int index, std::shared_ptr<Shader> &value)
    {
      if (sol::type_of(L, index) == sol::type::userdata)
      {
        auto userdata = sol::stack::get<sol::userdata>(L, index);
        if (userdata.is<Shader>())
        {
          value = CreateRef<Shader>(userdata.as<Shader>());
          return true;
        }
      }
      return false;
    }

    static void push(lua_State *L, const std::shared_ptr<Shader> &value)
    {
      sol::make_object(L, *value);
    }
  };
}

namespace lua_bindings
{
  void bindShaderClasses(sol::state &lua)
  {
    lua.new_enum<SHADER_TYPE>(
        "ShaderType",
        {{"VERTEX", SHADER_TYPE::VERTEX}, {"FRAGMENT", SHADER_TYPE::FRAGMENT}});

    // shader
    sol::usertype<Shader>
        shader_type = lua.new_usertype<Shader>("Shader", sol::constructors<Shader(SHADER_TYPE)>());
    shader_type["compile"] = &Shader::compile;
    shader_type["getShader"] = &Shader::getShader;
    shader_type["getType"] = &Shader::getType;
    shader_type["equal"] = &Shader::operator==;

    // shader program
    sol::usertype<ShaderProgram> program_type = lua.new_usertype<ShaderProgram>("ShaderProgram", sol::constructors<ShaderProgram()>());
    program_type["attachShader"] = &ShaderProgram::attachShader;
    program_type["link"] = &ShaderProgram::link;
    program_type["use"] = &ShaderProgram::use;
  }

  void exposeToLua(sol::state &lua)
  {
    bindShaderClasses(lua);
  }
}
