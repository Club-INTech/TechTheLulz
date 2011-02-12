cmake_minimum_required(VERSION 2.4)
cmake_policy(SET CMP0015 OLD)

project(SIMULOL)

set(EXECUTABLE_OUTPUT_PATH ${SIMULOL_SOURCE_DIR})

set(IRRLICHT_INC ${SIMULOL_SOURCE_DIR}/irrlicht-1.7.2/include)
set(IRRBP_INC ${SIMULOL_SOURCE_DIR}/irrBP/include)
set(BULLET_INC ${SIMULOL_SOURCE_DIR}/bullet-read-only/src)

set(IRRLICHT_LIB ${SIMULOL_SOURCE_DIR}/irrlicht-1.7.2/lib/Linux)
set(IRRBP_LIB ${SIMULOL_SOURCE_DIR}/irrBP/build)
set(BULLET_LIBDIR ${SIMULOL_SOURCE_DIR}/bullet-read-only/build/src)


include_directories (${IRRLICHT_INC} ${IRRBP_INC} ${BULLET_INC})

link_directories(${IRRLICHT_LIB})
link_directories(${IRRBP_LIB})
link_directories(${BULLET_LIBDIR}/BulletMultiThreaded)
link_directories(${BULLET_LIBDIR}/BulletSoftBody)
link_directories(${BULLET_LIBDIR}/BulletDynamics)
link_directories(${BULLET_LIBDIR}/BulletCollision)
link_directories(${BULLET_LIBDIR}/LinearMath)

link_libraries (
GL
Xxf86vm
irrBP
BulletMultiThreaded 
BulletSoftBody 
BulletDynamics 
BulletCollision 
LinearMath 
pthread 
Irrlicht)

add_executable(SimuLOL-preprepreprealpha main.cpp)

