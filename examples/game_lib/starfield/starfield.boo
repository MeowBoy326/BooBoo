number num_stars
= num_stars 128
number max_trail
= max_trail 64
number velocity
= velocity 1

vector stars

number i
= i 0
:add_another_star
vector v
number r
number x
number y
rand r 0 639
= x r
vector_add v r
rand r 0 359
= y r
vector_add v r
number dx
number dy
= dx x
- dx 320
= dy y
- dy 180
number angle
atan2 angle dy dx
number vx
cos vx angle
* vx velocity
number vy
sin vy angle
* vy velocity
vector_add v vx
vector_add v vy
vector_add stars v
+ i 1
? i num_stars
jl add_another_star

function run
{
	number sz
	vector_size stars sz

	number i
	= i 0
:next_update
	vector v
	vector_get stars v i
	number x
	number y
	number vx
	number vy
	vector_get v x 0
	vector_get v y 1
	vector_get v vx 2
	vector_get v vy 3
	+ x vx
	+ y vy
	? x 0
	jl regen
	? y 0
	jl regen
	? x 640
	jge regen
	? y 360
	jge regen
	goto done_regen

:regen
	= x 320
	= y 180
	goto done_regen

: done_regen
	vector_set v 0 x
	vector_set v 1 y
	vector_set stars i v
	+ i 1
	? i sz
	jl next_update
}

function draw
{
	clear 0 0 0

	number sz
	vector_size stars sz

	number i
	= i 0
:next_draw
	vector v
	vector_get stars v i
	number x
	number y
	number vx
	number vy
	vector_get v x 0
	vector_get v y 1
	vector_get v vx 2
	vector_get v vx 3
	number cx
	number cy
	= cx 320
	= cy 180
	number dx
	number dy
	= dx cx
	- dx x
	= dy cy
	- dy y
	* dx dx
	* dy dy
	number dist
	= dist dx
	+ dist dy
	sqrt dist dist
	; 410px is about the max it can get from diagonal to center
	/ dist 410
	* dist max_trail
	number dx
	number dy
	= dx 320
	- dx x
	= dy 180
	- dy y
	number angle
	atan2 angle dy dx
	cos dx angle
	* dx dist
	sin dy angle
	* dy dist
	number trail_x
	number trail_y
	= trail_x x
	- trail_x dx
	= trail_y y
	- trail_y dy
	line 255 255 255 255 x y trail_x trail_y 1
	+ i 1
	? i sz
	jl next_draw
}
