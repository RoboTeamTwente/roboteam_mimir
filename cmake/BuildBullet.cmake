include(ExternalProject)

ExternalProject_Add(project_bullet
        URL http://github.com/bulletphysics/bullet3/archive/2.89.tar.gz
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/bullet
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
        -DCMAKE_MAKE_PROGRAM:PATH=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_DEBUG_POSTFIX:STRING=
        -DBUILD_BULLET2_DEMOS:BOOL=OFF
        -DBUILD_BULLET3:BOOL=OFF
        -DBUILD_EXTRAS:BOOL=OFF
        -DBUILD_OPENGL3_DEMOS:BOOL=OFF
        -DUSE_GRAPHICAL_BENCHMARK:BOOL=OFF
        -DBUILD_UNIT_TESTS=OFF
        -DUSE_DOUBLE_PRECISION=OFF
        -DUSE_SOFT_BODY_MULTI_BODY_DYNAMICS_WORLD=OFF
        -DUSE_GLUT=OFF
        -DBUILD_ENET=OFF
        -DBUILD_CLSOCKET=OFF
        -DINSTALL_LIBS:BOOL=ON
        BUILD_BYPRODUCTS
        "<INSTALL_DIR>/lib/${CMAKE_STATIC_LIBRARY_PREFIX}BulletDynamics${CMAKE_STATIC_LIBRARY_SUFFIX}"
        "<INSTALL_DIR>/lib/${CMAKE_STATIC_LIBRARY_PREFIX}BulletCollision${CMAKE_STATIC_LIBRARY_SUFFIX}"
        "<INSTALL_DIR>/lib/${CMAKE_STATIC_LIBRARY_PREFIX}LinearMath${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
ExternalProject_get_property(project_bullet install_dir)

set_target_properties(project_bullet PROPERTIES EXCLUDE_FROM_ALL true)
add_library(lib::bullet STATIC IMPORTED)
add_dependencies(lib::bullet project_bullet)

set(BULLET_INCLUDE_DIRS ${install_dir}/include/bullet)
file(MAKE_DIRECTORY "${install_dir}/include/bullet")

set_property(TARGET lib::bullet PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${install_dir}/include/bullet")
set_property(TARGET lib::bullet PROPERTY IMPORTED_LOCATION
        "${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}BulletDynamics${CMAKE_STATIC_LIBRARY_SUFFIX}"
        )
set_property(TARGET lib::bullet PROPERTY INTERFACE_LINK_LIBRARIES
        "${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}BulletDynamics${CMAKE_STATIC_LIBRARY_SUFFIX}"
        "${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}BulletCollision${CMAKE_STATIC_LIBRARY_SUFFIX}"
        "${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}LinearMath${CMAKE_STATIC_LIBRARY_SUFFIX}"
        )
