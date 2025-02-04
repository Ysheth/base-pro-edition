#ifndef NX_COLLISION_NXPLANESHAPE
#define NX_COLLISION_NXPLANESHAPE
/*----------------------------------------------------------------------------*\
|
|						Public Interface to NovodeX Technology
|
|							     www.novodex.com
|
\*----------------------------------------------------------------------------*/
#include "Nxp.h"
#include "NxShape.h"

class NxPlane;
class NxPlaneShapeDesc;

/**
A plane collision detection primitive.
By default it is configured to be the y == 0 plane. You can then set a normal and a d to specify
an arbitrary plane. d is the distance of the plane from the origin along the normal, assuming
the normal is normalized. Thus the plane equation is: 
normal.x * X + normal.y * Y + normal.z * Z  = d

Note: the plane does not represent an infinitely thin object, but rather a completely solid negative
half space (all points p for which normal.dot(p) - d < 0 are inside the solid region.)


Each shape is owned by an actor that it is attached to.

An instance can be created by calling the createShape() method of the NxActor object
that should own it, with a NxPlaneShapeDesc object as the parameter, or by adding the 
shape descriptor into the NxActorDesc class before creating the actor.

The shape is deleted by calling NxActor::releaseShape() on the owning actor.
*/

class NxPlaneShape: public NxShape
	{
	public:

	/**
	sets the plane equation. See above.
	*/
	virtual void setPlane(const NxVec3 & normal, NxReal d) = 0;

	/*
	Saves the state of the shape object to a descriptor.
	*/
	virtual	void	saveToDesc(NxPlaneShapeDesc&)	const = 0;

	/**
	retrieves the plane
	*/
	virtual NxPlane	getPlane() const = 0;

	};
#endif
