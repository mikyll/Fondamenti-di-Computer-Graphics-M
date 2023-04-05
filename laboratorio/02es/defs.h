#pragma once

#define WINDOW_WIDTH	1280
#define WINDOW_HEIGHT	720

#define UPDATE_DELAY	0

#define PI	3.141592653589793

// SPACESHIP ==============================================
#define SPACESHIP_SCALE	10.0f

#define SPACESHIP_MAX_FORWARD_SPEED	250.0f
#define SPACESHIP_MAX_ANGULAR_SPEED	(PI * 1.5)
#define SPACESHIP_MIN_ANGULAR_SPEED	(PI / 1.5)

#define SPACESHIP_FORWARD_DECELERATION	0.95f
#define SPACESHIP_ANGULAR_DECELERATION	0.8f

enum {
	SPACESHIP_HULL,
	SPACESHIP_NOSE,
	SPACESHIP_FIN,
	SPACESHIP_PROPULSOR,
	SPACESHIP_ASTRONAUT,
	SPACESHIP_PORTHOLE,
};

// STARS ==================================================
#define NUM_STARS_NEAR		50
#define NUM_STARS_MIDDLE	100
#define NUM_STARS_FAR		150
#define NUM_TOT_STARS		(NUM_STARS_NEAR + NUM_STARS_MIDDLE + NUM_STARS_FAR)

#define STAR_OFFSET_NEAR	0
#define	STAR_OFFSET_MIDDLE	(0 + NUM_STARS_NEAR)
#define STAR_OFFSET_FAR		(NUM_STARS_NEAR + NUM_STARS_MIDDLE)

// FIRETRAIL ==============================================
#define SPEED_DECREASE_FACTOR	SPACESHIP_FORWARD_DECELERATION
#define ALPHA_DECREASE_FACTOR	2.5f

// ASTEROIDS ==============================================
enum {
	ASTEROID_1 =	1,
	ASTEROID_2 =	2,
	ASTEROID_3 =	3,
};

#define ASTEROID_RADIUS_BASE	1.9f
#define ASTEROID_SCALE_BASE		20.0f

#define ASTEROID_BIG_FACTOR		1.5f
#define ASTEROID_MEDIUM_FACTOR	1.0f
#define ASTEROID_SMALL_FACTOR	0.5f

#define NUM_ASTEROIDS	5

// BULLETS ================================================
#define BULLET_RADIUS		5.0f
#define BULLET_MAX_SPEED	650.0f

#define BULLET_RELOAD_COOLDOWN	2.0f

// COLLIDERS ==============================================
#define COLLIDER_COLOR			{ 1.0f, 1.0f, 0.0f, 1.0f }
#define COLLIDER_NUM_SEGMENTS	15