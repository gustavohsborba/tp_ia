#include "magic.h"
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <algorithm>
#include "NeuralNetwork.h"
#include "Cor.h"
#include "Gui.h"
#include "RigidBody2D.h"
#include "NeuralTrainer.h"

#define _TIMESTEP 0.0045
#define _SPAWN_AREA_R 300

#define _MAX_NUM_UNITS 22

#define _UNIT_LENGTH 4

#define _WIDEVIEW_RADIUS_FACTOR 100
#define _LIMITEDVIEW_RADIUS_FACTOR 30
#define _CRITICAL_RADIUS_FACTOR 5

#define _SEPARATION_FACTOR 5
#define _BACK_VIEW_ANGLE_FACTOR 1

#define _MAXHITPOINTS 100.0

// Global Variables:
RigidBody2D Units[_MAX_NUM_UNITS];
enum actions{chase, flock, evade};

double damageRate = 0.4;
bool showVectors=false;
bool showField=true;

int GetRandomNumber(int min, int max) {
    int	number;
    number = (((abs(rand())%(max-min+1))+min));

    if(number>max) {
        number = max;
    } else if(number<min) {
        number = min;
    }
    return number;
}

Vector minWrapVector(Vector a, Vector b){
    return minWrapVector(a,b,_WINWIDTH, _WINHEIGHT);
}

void Initialize() {
    srand((unsigned) time(NULL));
	for(int i=0; i<_MAX_NUM_UNITS; i++) {
		Units[i].fMass = 10;
		Units[i].fInertia = 10;
		Units[i].vPosition.x = GetRandomNumber(_WINWIDTH/2-_SPAWN_AREA_R, _WINWIDTH/2+_SPAWN_AREA_R);
		Units[i].vPosition.y = GetRandomNumber(_WINHEIGHT/2-_SPAWN_AREA_R, _WINHEIGHT/2+_SPAWN_AREA_R);
		Units[i].fWidth = _UNIT_LENGTH/2;
		Units[i].fLength = _UNIT_LENGTH;
		Units[i].fHeight = _UNIT_LENGTH;
		Units[i].fOrientation = GetRandomNumber(0, 360);
		Units[i].CD.y = -0.12*Units[i].fLength; Units[i].CD.x = 0.0f;  // coordinates of the body center of drag
        Units[i].CT.y = -0.50*Units[i].fLength; Units[i].CT.x = 0.0f;  // coordinates of the propeller thrust vector
        Units[i].CPT.y = 0.5*Units[i].fLength; Units[i].CPT.x = -0.5*Units[i].fWidth; // coordinates of the port bow thruster
        Units[i].CST.y = 0.5*Units[i].fLength; Units[i].CST.x = 0.5*Units[i].fWidth; // coordinates of the starboard bow thruster

		Units[i].ProjectedArea = (Units[i].fLength + Units[i].fWidth) * Units[i].fHeight;

		Units[i].ThrustForce = _THRUSTFORCE;
		Units[i].HitPoints = _MAXHITPOINTS;
		Units[i].Chase = false;
		Units[i].Flock = false;
		Units[i].Evade = false;

	}

	Units[0].fInertia = 2;
	Units[0].fMass = 2;
	Units[0].vPosition.x = 0;
	Units[0].vPosition.y = 0;
	Units[0].fOrientation = 45;
	Units[0].HitPoints = _MAXHITPOINTS;
	Units[0].ThrustForce = _THRUSTFORCE*2;

    InitializeBrain();
	TrainTheBrain();
}

void Finalize() {
	EndBrain();
}

void DoUnitAI(int i) {
		int		nProximos;
		Vector	positionAvg;
		Vector	velocityAvg;
		Vector	steeringForce;
		Vector	d, u, v, w;
		double	m = 0.0;
		bool InView;
		int	RadiusFactor;

		// begin Flock AI
		steeringForce.x = steeringForce.y = steeringForce.z = 0;
		positionAvg.x = positionAvg.y = positionAvg.z = 0;
		velocityAvg.x = velocityAvg.y = velocityAvg.z = 0;
		nProximos = 0;
        Units[i].NumFriends = 0;
		
		for(int j=1; j<_MAX_NUM_UNITS; j++) {
			if(i!=j) {
				d = minWrapVector(Units[j].vPosition, Units[i].vPosition);
				w = VRotate2D(-Units[i].fOrientation, d);

				if(d.Magnitude() <= (Units[i].fLength * _LIMITEDVIEW_RADIUS_FACTOR))
                    Units[i].NumFriends++;

				if(Units[i].Flock) {
					InView = (w.y > 0);
					RadiusFactor = _LIMITEDVIEW_RADIUS_FACTOR;
				} else {
                    InView = ((w.y > 0) || ((w.y < 0) && (fabs(w.x) > fabs(w.y)*_BACK_VIEW_ANGLE_FACTOR)));
                    RadiusFactor = _WIDEVIEW_RADIUS_FACTOR;
                }

				if(InView) {
					if(d.Magnitude() <= (Units[i].fLength * RadiusFactor)) {
						positionAvg += Units[j].vPosition;
						velocityAvg += Units[j].vVelocity;
						nProximos++;
					}

				    // Separation Rule:
					if(d.Magnitude() <= (Units[i].fLength * _SEPARATION_FACTOR)) {
						if(w.x < 0){
                            m = 1;
                        } else if(w.x > 0){
                            m = -1;
                        }
						
						steeringForce.x += m*_STEERINGFORCE * (Units[i].fLength * _SEPARATION_FACTOR) / d.Magnitude();
					}
				}
			}
		}

		// Cohesion Rule: 
		if(Units[i].Flock && (nProximos > 0)) {
			positionAvg = positionAvg / nProximos;
			v = Units[i].vVelocity;
			v.Normalize();
			u = minWrapVector(positionAvg, Units[i].vPosition);
			u.Normalize();
			w = VRotate2D(-Units[i].fOrientation, u);
			if(w.x < 0) m = -1;
			if(w.x > 0) m = 1;
			if(fabs(v*u) < 1.0f)
				steeringForce.x += m * _STEERINGFORCE * acos(v * u) / pi;
		}

		// Alignment Rule:
		if(Units[i].Flock && (nProximos > 0)) {
			velocityAvg = velocityAvg / nProximos;
			u = velocityAvg;
			u.Normalize();
			v = Units[i].vVelocity;
			v.Normalize();			
			w = VRotate2D(-Units[i].fOrientation, u);
			if(w.x < 0) m = -1;
			if(w.x > 0) m = 1;
			if(fabs(v*u) < 1)
				steeringForce.x += m * _STEERINGFORCE * acos(v * u) / pi;
		}

		// Chase
		if(Units[i].Chase) {
			u = Units[0].vPosition;
			d = minWrapVector(u,Units[i].vPosition);
			w = VRotate2D(-Units[i].fOrientation, d);
			if(w.x < 0) m = -1;
			if(w.x > 0) m = 1;
			steeringForce.x += m*_STEERINGFORCE;
		}

		// Evade
		if(Units[i].Evade) {
			u = Units[0].vPosition;
			d = minWrapVector(u,Units[i].vPosition);
			w = VRotate2D(-Units[i].fOrientation, d);
			if(w.x < 0) m = 1;
			if(w.x > 0) m = -1;
			steeringForce.x += m*_STEERINGFORCE;
		}

		// apply accumulated steering force
		Units[i].Fa = steeringForce;
		Units[i].Pa = Vector(0.0f, Units[i].fLength / 2.0f, 0.0f);
		// end Flock AI

}

void UpdateSimulation(int _) {
    glutTimerFunc(20, UpdateSimulation, 0);
	double dt = _TIMESTEP;
	int i;
	Vector u;
	bool kill = false;
	
	// Update player controlled unit:	
	Units[0].SetThrusters(false, false, 1);
	Units[0].SetThrusters(false, false, 1);

    if(IsKeyDownNow(27)) { //esc
        Finalize();
        exit(0);
    }
    if (IsKeyDownNow('v')) {
        showVectors = !showVectors;
    }
	if (IsKeyDownNow('f')) {
		showField = !showField;
	}

	if (IsKeyDown('d'))
		Units[0].SetThrusters(true, false, 0.5);

	if (IsKeyDown('a'))
		Units[0].SetThrusters(false, true, 0.5);

	if (IsKeyDown('0')) {
		damageRate = 0.0;
	} else if (IsKeyDown('1')) {
		damageRate = 0.2;
	} else if (IsKeyDown('2')) {
		damageRate = 0.4;
	} else if (IsKeyDown('3')) {
		damageRate = 0.6;
	} else if (IsKeyDown('4')) {
		damageRate = 0.8;
	} else if (IsKeyDown('5')) {
		damageRate = 1.0;
	} else if (IsKeyDown('6')) {
		damageRate = 1.2;
	} else if (IsKeyDown('7')) {
		damageRate = 1.4;
	} else if (IsKeyDown('8')) {
		damageRate = 1.6;
	} else if (IsKeyDown('9')) {
		damageRate = 1000.0;
	}


	Units[0].UpdateBodyEuler(dt);

	if(Units[0].vPosition.x > _WINWIDTH) Units[0].vPosition.x = 0;
	if(Units[0].vPosition.x < 0) Units[0].vPosition.x = _WINWIDTH;
	if(Units[0].vPosition.y > _WINHEIGHT) Units[0].vPosition.y = 0;
	if(Units[0].vPosition.y < 0) Units[0].vPosition.y = _WINHEIGHT;

	// calc number of enemy units currently engaging the target
	Vector d;
	Units[0].NumFriends = 0;
	for(i=1; i<_MAX_NUM_UNITS; i++) {	
		d = minWrapVector(Units[i].vPosition, Units[0].vPosition);
		if(d.Magnitude() <= (Units[0].fLength * _CRITICAL_RADIUS_FACTOR)) {
            Units[0].NumFriends++;
        }
	}

	// deduct hit points from target
	if(Units[0].NumFriends > 0) {
		Units[0].HitPoints -= 0.2 * Units[0].NumFriends;
		if(Units[0].HitPoints < 0) {
			Units[0].vPosition.x = _WINWIDTH/2;
			Units[0].vPosition.y = _WINHEIGHT/2;
			Units[0].HitPoints = _MAXHITPOINTS;
			kill = true;
		}
	} else {
		//Units[0].HitPoints += 1;
		//if(Units[0].HitPoints > _MAXHITPOINTS) Units[0].HitPoints = _MAXHITPOINTS;
	}

    /*static double lastHitpoints = -1;
    if(lastHitpoints != Units[0].HitPoints){
        lastHitpoints = Units[0].HitPoints;
        printf("player hitpoints: %.0lf\n", Units[0].HitPoints);
    }*/

	
	// update computer controlled units:	
	for(i=1; i<_MAX_NUM_UNITS; i++) {		
		u = minWrapVector(Units[0].vPosition, Units[i].vPosition);
		if(kill) {				
			if((u.Magnitude() <= (Units[0].fLength * _CRITICAL_RADIUS_FACTOR)) /*&& (Units[i].Command != 2)*/) {
				ReTrainTheBrain(Units[i], 0.9, 0.1, 0.1);
				Units[i].HitPoints = std::min(Units[i].HitPoints+_MAXHITPOINTS/4.0f, _MAXHITPOINTS);
				if(Units[i].HitPoints > _MAXHITPOINTS) Units[i].HitPoints = _MAXHITPOINTS;
			}
		}				

		// handle enemy hitpoints, and learning if required
		if(u.Magnitude() <= (Units[0].fLength * _CRITICAL_RADIUS_FACTOR)) {
			Units[i].HitPoints -= damageRate;
			if((Units[i].HitPoints < 0)) {
                printf("player matou inimigo\n");
				Units[i].vPosition.x = GetRandomNumber(_WINWIDTH/2-_SPAWN_AREA_R, _WINWIDTH/2+_SPAWN_AREA_R);
				Units[i].vPosition.y = GetRandomNumber(_WINHEIGHT/2-_SPAWN_AREA_R, _WINHEIGHT/2+_SPAWN_AREA_R);
				Units[i].HitPoints = _MAXHITPOINTS/2.0;
//				if(Units[i].Command == chase)
					ReTrainTheBrain(Units[i], 0.1, 0.1, 0.9);
			}
		} else {
			Units[i].HitPoints = std::min(Units[i].HitPoints+0.01, _MAXHITPOINTS);
		}

		// get a new command
		Units[i].Inputs[0] = Units[i].NumFriends/_MAX_NUM_UNITS;
		Units[i].Inputs[1] = (double) (Units[i].HitPoints/_MAXHITPOINTS);
		Units[i].Inputs[2] = (Units[0].NumFriends>0 ? 1:0);
		Units[i].Inputs[3] = (u.Magnitude()/800.0f);
		
			
		TheBrain.SetInput(0, Units[i].Inputs[0]);	
		TheBrain.SetInput(1, Units[i].Inputs[1]);		
		TheBrain.SetInput(2, Units[i].Inputs[2]);
		TheBrain.SetInput(3, Units[i].Inputs[3]);			
		TheBrain.FeedForward();

		Units[i].Command = TheBrain.GetMaxOutputID();
		switch(Units[i].Command) {
			case chase:
				Units[i].Chase = true;
				Units[i].Flock = false;
				Units[i].Evade = false;
				break;

			case flock:
				Units[i].Chase = false;
				Units[i].Flock = true;
				Units[i].Evade = false;
				break;

			case evade:
				Units[i].Chase = false;
				Units[i].Flock = false;
				Units[i].Evade = true;
				break;

		}

		DoUnitAI(i);
		
		Units[i].UpdateBodyEuler(dt);

		if(Units[i].vPosition.x > _WINWIDTH) Units[i].vPosition.x = 0;
		if(Units[i].vPosition.x < 0) Units[i].vPosition.x = _WINWIDTH;
		if(Units[i].vPosition.y > _WINHEIGHT) Units[i].vPosition.y = 0;
		if(Units[i].vPosition.y < 0) Units[i].vPosition.y = _WINHEIGHT;	
		
	}
    glutPostRedisplay();
}

void DrawLine(float a, float b, float c, float d) {
    glVertex2f(a,b);
    glVertex2f(c,d);
}

void drawCircle(float x, float y, float r){
    const int nvertex = 15;
    glBegin(GL_LINES);
    for(double i = 0; i < 2 * M_PI; i += M_PI / nvertex)
        glVertex2f(x+cos(i) * r, y+sin(i) * r);
    glEnd();
}

void DrawCraft(RigidBody2D craft, Cor clr) {
    Vector vList[5];
    float wd, lg;
    int	i;
    Vector v1;

    wd = craft.fWidth;
    lg = craft.fLength;
    vList[0].y = lg/2;	vList[0].x = wd/2;
    vList[1].y = -lg/2;	vList[1].x = wd/2;
    vList[2].y = -lg/2;	vList[2].x = -wd/2;
    vList[3].y = lg/2;	vList[3].x = -wd/2;
    vList[4].y = lg/2*1.5f; vList[4].x = 0;
    for(i=0; i<5; i++) {
        v1 = VRotate2D(craft.fOrientation, vList[i]);
        vList[i] = v1 + craft.vPosition;
    }

    glBegin(GL_LINES);
    glColor3ub(clr.r,clr.g,clr.b);
    DrawLine(vList[0].x, vList[0].y, vList[1].x, vList[1].y);
    DrawLine(vList[1].x, vList[1].y, vList[2].x, vList[2].y);
    DrawLine(vList[2].x, vList[2].y, vList[3].x, vList[3].y);
    DrawLine(vList[3].x, vList[3].y, vList[4].x, vList[4].y);
    DrawLine(vList[4].x, vList[4].y, vList[0].x, vList[0].y);
    glEnd();

	if(showField) {
		drawCircle(craft.vPosition.x, craft.vPosition.y, _CRITICAL_RADIUS_FACTOR * Units[0].fLength);
	}

    if(showVectors) {
        glBegin(GL_LINES);
        Vector v, u;
        float f = 0.025f;

        // Show velocity vectors in green
        DrawLine(craft.vPosition.x, craft.vPosition.y, craft.vPosition.x+craft.vVelocity.x, craft.vPosition.y+craft.vVelocity.y);

        // Show force vectors in black
        // thrust vector
        v.x = 0;
        v.y = craft.ThrustForce*f;
        v = VRotate2D(craft.fOrientation, v);
        u.x = craft.CT.x;
        u.y = craft.CT.y;
        u = VRotate2D(craft.fOrientation, u);
        DrawLine(craft.vPosition.x+u.x, craft.vPosition.y+u.y, craft.vPosition.x + u.x + v.x, craft.vPosition.y + u.y + v.y);

        // port steering force
        v.x = craft.PThrust.x*f;
        v.y = craft.PThrust.y*f;
        v = VRotate2D(craft.fOrientation, v);
        u.x = craft.CPT.x;
        u.y = craft.CPT.y;
        u = VRotate2D(craft.fOrientation, u);
        DrawLine(craft.vPosition.x+u.x, craft.vPosition.y+u.y, craft.vPosition.x + u.x + v.x, craft.vPosition.y + u.y + v.y);

        // stbd steering force
        v.x = craft.SThrust.x*f;
        v.y = craft.SThrust.y*f;
        v = VRotate2D(craft.fOrientation, v);
        u.x = craft.CST.x;
        u.y = craft.CST.y;
        u = VRotate2D(craft.fOrientation, u);
        DrawLine(craft.vPosition.x+u.x, craft.vPosition.y+u.y, craft.vPosition.x + u.x + v.x, craft.vPosition.y + u.y + v.y);

        // applied force
        v.x = craft.Fa.x*f;
        v.y = craft.Fa.y*f;
        v = VRotate2D(craft.fOrientation, v);
        u.x = craft.Pa.x;
        u.y = craft.Pa.y;
        u = VRotate2D(craft.fOrientation, u);
        DrawLine(craft.vPosition.x+u.x, craft.vPosition.y+u.y, craft.vPosition.x + u.x + v.x, craft.vPosition.y + u.y + v.y);
        glEnd();
    }
}

void drawCrafts() {
	glClearColor(.1,.1,.1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < _MAX_NUM_UNITS; i++) {
        if (i == 0) {
            DrawCraft(Units[i], Cor(255, 255, 255));
        } else if (Units[i].Command == chase)
            DrawCraft(Units[i], Cor(255, 0, 0));
        else if (Units[i].Command == flock)
            DrawCraft(Units[i], Cor(0, 0, 255));
        else if (Units[i].Command == evade)
            DrawCraft(Units[i], Cor(128, 128, 0));
    }

    glutSwapBuffers();
}