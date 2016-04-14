macro(fix_default_compiler_settings_)
  if (MSVC)
    # For MSVC, CMake sets certain flags to defaults we want to override.
    # This replacement code is taken from sample in the CMake Wiki at
    # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if (NOT BUILD_SHARED_LIBS AND NOT cpp_note_force_shared_crt)
        # When Google Test is built as a shared library, it should also use
        # shared runtime libraries.  Otherwise, it may end up with multiple
        # copies of runtime library data in different modules, resulting in
        # hard-to-find crashes. When it is built as a static library, it is
        # preferable to use CRT as static libraries, as we don't have to rely
        # on CRT DLLs being available. CMake always defaults to using shared
        # CRT libraries, so we override that default here.
        string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
      endif()

      # We prefer more strict warning checking for building Google Test.
      # Replaces /W3 with /W4 in defaults.
      string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
    endforeach()
  endif()
endmacro()

macro(add_src_libs_ target)
  ################################
  # Include all .cpp/.h files
  ################################
  unset(the_SRC)
  file(GLOB_RECURSE the_SRC
    "src/*.h"
    "src/*.cpp"
    )
  add_executable(${target} ${the_SRC})

  if(WIN32)
    TARGET_LINK_LIBRARIES(${target} optimized ${GTEST_RELEASE_LIB})
    TARGET_LINK_LIBRARIES(${target} debug ${GTEST_DEBUG_LIB})
    TARGET_LINK_LIBRARIES(${target} optimized ${GTEST_RELEASE_LIB_MAIN})
    TARGET_LINK_LIBRARIES(${target} debug ${GTEST_DEBUG_LIB_MAIN})
  else(WIN32)
    TARGET_LINK_LIBRARIES(${target} ${GTEST_BOTH_LIBRARIES})  
  endif(WIN32)

  target_link_libraries(${target} ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES} ${EXTRA_LIB})
endmacro()
