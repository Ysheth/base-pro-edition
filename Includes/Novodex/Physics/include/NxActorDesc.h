#ifndef NX_PHYSICS_NXACTORDESC
#define NX_PHYSICS_NXACTORDESC
/*----------------------------------------------------------------------------*\
|
|						Public Interface to NovodeX Technology
|
|							     www.novodex.com
|
\*----------------------------------------------------------------------------*/

#include "NxBodyDesc.h"
#include "NxShapeDesc.h"

	enum ActorDescType
		{
		NX_ADT_SHAPELESS,
		NX_ADT_DEFAULT,
		NX_ADT_ALLOCATOR,
		NX_ADT_LIST,
		NX_ADT_POINTER
		};

/**
	Actor Descriptor. This structure is used to save and load the state of NxActor objects.

	if the body descriptor contains a null mass but the actor descriptor contains a non-null density,
	we compute a new mass automatically from the density and the shapes.

	Static or dynamic actors:

	- To create a static actor, use a null body descriptor pointer. Do not create a body with zero mass.
	  If you want to create a temporarily static actor that can be made dynamic at runtime, create your
	  dynamic actor as usual and use BF_FROZEN flags in its body descriptor.

	- To create a dynamic actor, provide a valid body descriptor with or without shape descriptors. The
	  shapes are not mandatory.

	Mass or density:
	
		To simulate a dynamic actor, the SDK needs a mass and an inertia tensor. 
		(The inertia tensor is the combination of bodyDesc.massLocalPose and bodyDesc.massSpaceInertia)

		These can be specified in several different ways:

		1) actorDesc.density == 0,  bodyDesc.mass > 0, bodyDesc.massSpaceInertia.magnitude() > 0

			Here the mass properties are specified explicitly, there is nothing to compute.

		2) actorDesc.density > 0,	actorDesc.shapes.size() > 0, bodyDesc.mass == 0, bodyDesc.massSpaceInertia.magnitude() == 0

			Here a density and the shapes are given. From this both the mass and the inertia tensor is computed.

		3) actorDesc.density == 0,	actorDesc.shapes.size() > 0, bodyDesc.mass > 0, bodyDesc.massSpaceInertia.magnitude() == 0

			Here a mass and shapes are given. From this the inertia tensor is computed.

		Other combinations of settings are illegal.
*/

class NxActorDescBase
	{
	public:
	NxMat34					globalPose;		//!< The pose of the actor in the world.
	const NxBodyDesc*		body;			//!< Body descriptor, null for static actors
	NxReal					density;		//!< We can compute the mass from a density and the shapes, see above notes.
	NxU32					flags;			//!< Combination of ::NxActorFlag flags
	NxActorGroup			group;			//!< Actor group. See NxActor::setGroup().
	void*					userData;		//!< Will be copied to NxActor::userData
	const char*				name;			//!< Possible debug name. The string is not copied by the SDK, only the pointer is stored.

	/**
	constructor sets to default.
	*/
	NX_INLINE NxActorDescBase();	
	/**
	(re)sets the structure to the default.	
	*/
	NX_INLINE void setToDefault();
	/**
	returns true if the current settings are valid
	*/
	NX_INLINE bool isValid() const;

	/**
	type info.
	*/
	NX_INLINE ActorDescType getType() const;
	protected:
	NX_INLINE bool isValidInternal(bool hasShape) const;

	ActorDescType			type;
	};

/**
Legacy implementation that works with existing code but does not permit the user
to supply his own allocator for NxArray<NxShapeDesc*>	shapes.
*/
class NxActorDesc : public NxActorDescBase
	{
	public:
	NxArray<NxShapeDesc*>	shapes;			//!< Shapes composing the actor

	/**
	constructor sets to default.
	*/
	NX_INLINE NxActorDesc();	
	/**
	(re)sets the structure to the default.	
	*/
	NX_INLINE void setToDefault();
	/**
	returns true if the current settings are valid
	*/
	NX_INLINE bool isValid() const;
	};

/**
Implementation that permits the user to supply his own allocator
*/
template<class AllocType = NxAllocatorDefault> class NxActorDesc_Template : public NxActorDescBase
	{
	public:
	NxArray<NxShapeDesc*, AllocType>	shapes;			//!< Shapes composing the actor



	NX_INLINE NxActorDesc_Template()
		{
		setToDefault();
		type = NX_ADT_ALLOCATOR;
		}

	NX_INLINE void setToDefault()
		{
		NxActorDescBase::setToDefault();
		shapes.clear();
		}

	NX_INLINE bool isValid() const
		{
		if (!NxActorDescBase::isValid())
			return false;
		for (unsigned i = 0; i < shapes.size(); i++)
			if (!shapes[i]->isValid())
				return false;

		if (!NxActorDescBase::isValidInternal(shapes.size() > 0))
			return false;

		return true;
		}


	};
	


NX_INLINE NxActorDescBase::NxActorDescBase()
	{
	//nothing!  Don't call setToDefault() here!
	}


NX_INLINE void NxActorDescBase::setToDefault()
	{
	body		= NULL;
	density		= 0.0f;
	globalPose	.id();
	flags		= 0;
	userData	= NULL;
	name		= NULL;
	group		= 0;
	}

NX_INLINE bool NxActorDescBase::isValid() const
	{
	if (density < 0)
		return false;

	if (body && !body->isValid())
		return false;
	if(!globalPose.isFinite())
		return false;

	return true;
	}

NX_INLINE ActorDescType NxActorDescBase::getType() const			
	{	
	return type; 
	}

NX_INLINE bool NxActorDescBase::isValidInternal(bool haveShape) const
	{
	bool haveDensity = density!=0.0f;
	bool haveMass = body && body->mass != 0.0f;
	bool haveTensor = body && !(body->massSpaceInertia.isZero() > 0.0f);
	
	if (haveShape && !body)
		return true;
	if      (haveShape && haveDensity && !haveMass && !haveTensor) return true;
	else if (haveShape && !haveDensity && haveMass && !haveTensor) return true;
	else if (!haveDensity && haveMass && haveTensor) return true;
	else return false;
	}




NX_INLINE NxActorDesc::NxActorDesc()
	{
	setToDefault();
	type = NX_ADT_DEFAULT;
	}

NX_INLINE void NxActorDesc::setToDefault()
	{
	NxActorDescBase::setToDefault();
	shapes		.clear();
	}

NX_INLINE bool NxActorDesc::isValid() const
	{
	if (!NxActorDescBase::isValid())
		return false;

	for (unsigned i = 0; i < shapes.size(); i++)
		if (!shapes[i]->isValid())
			return false;


	if (!NxActorDescBase::isValidInternal(shapes.size() > 0))
		return false;

	return true;
	}

#endif
