#ifndef _RIGIDBODY2DHEADER
#define _RIGIDBODY2DHEADER

#include "Vector.h"

#define CONSTANTE_MAROTA 4.0F
#define	 _THRUSTFORCE				(500.0f*CONSTANTE_MAROTA)
#define  _STEERINGFORCE				(500.0f*CONSTANTE_MAROTA)
#define  _LINEARDRAGCOEFFICIENT		10.5f
#define  _ANGULARDRAGCOEFFICIENT	2000.0f

#define NEURAL_INPUTS 6

//------------------------------------------------------------------------//
// Rigid body structure (2D only)
//------------------------------------------------------------------------//
class RigidBody2D {
public:
	float	fMass;				// total mass (constant)
	float	fInertia;			// mass moment of inertia in body coordinates (constant)
	
	Vector	vPosition;			// position in earth coordinates
	Vector	vVelocity;			// velocity in earth coordinates
	Vector	vVelocityBody;		// velocity in body coordinates
	Vector	vAngularVelocity;	// angular velocity in body coordinates

	float	fOrientation;		// orientation 	

	Vector	vForces;			// total force on body
	Vector	vMoment;			// total moment (torque) on body (2D: about z-axis only)

	float	ThrustForce;		// Magnitude of the thrust force
	Vector	PThrust, SThrust;	// bow thruster forces

	float	fWidth;				// bounding dimensions
	float	fLength;
	float	fHeight;

	Vector	CD;
	Vector	CT;
	Vector	CPT;
	Vector	CST;

	float	ProjectedArea;

	Vector	Fa;// other applied force
	Vector  Pa;// location of other applied force

	RigidBody2D();
	void	CalcLoads();
	void	UpdateBodyEuler(double dt);
	void	SetThrusters(bool p, bool s, double f);

	

	double	HitPoints;
	int		NumFriends;

	int		Command;
	bool	Chase;
	bool	Flock;
	bool	Evade;

	double	Inputs[NEURAL_INPUTS];

};

#endif