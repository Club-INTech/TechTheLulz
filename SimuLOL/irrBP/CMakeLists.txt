
include_directories (include
../irrlicht-1.7.2/include
../bullet-read-only/src)

link_libraries ( 
BulletMultiThreaded  
BulletSoftBody 
BulletDynamics 
BulletCollision 
LinearMath 
pthread 
Irrlicht
)

add_library (irrBP
src/CIrrBPCollisionObject.cpp
src/CIrrBPManager.cpp
src/CMotionState.cpp
src/CIrrBPDebugDrawer.cpp
src/CIrrBPWorld.cpp
src/actions/CIrrBPRaycastVehicle.cpp
src/animator/CIrrBPAnimator.cpp
src/animator/CIrrBPCollisionCallbackAnimator.cpp
src/animator/CIrrBPCollisionDeleteAnimator.cpp
src/animator/CIrrBPDeleteAnimator.cpp
src/animator/CIrrBPTimeCallBackAnimator.cpp
src/body/CIrrBPBoxBody.cpp
src/body/CIrrBPCapsuleBody.cpp
src/body/CIrrBPConeBody.cpp
src/body/CIrrBPConvexHullBody.cpp
src/body/CIrrBPCylinderBody.cpp
src/body/CIrrBPRigidBody.cpp
src/body/CIrrBPSphereBody.cpp
src/body/CIrrBPTrimeshBody.cpp
src/constraint/CIrrBP6DOFConstraint.cpp
src/constraint/CIrrBPConeTwistConstraint.cpp
src/constraint/CIrrBPConstraint.cpp
src/constraint/CIrrBPHingeConstraint.cpp
src/constraint/CIrrBPP2PConstraint.cpp
src/constraint/CIrrBPSlideConstraint.cpp
#src/constraint/CIrrBPSpringConstraint.cpp
src/softbody/CIrrBPPatchSoftBody.cpp
src/softbody/CIrrBPRopeSoftBody.cpp
src/softbody/CIrrBPSoftBody.cpp
)
