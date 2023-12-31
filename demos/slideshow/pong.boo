string reset_game_name
= reset_game_name "secret.boo"
include "slideshow_start.inc"

number score1
= score1 0
number score2
= score2 0

function rand_speed
{
	number f
	rand f 0 1000
	/ f 1000.0
	* f 5.0
	return f
}

number next_cpu1_speed_update
rand next_cpu1_speed_update 0 100
number cpu1_speed
call_result cpu1_speed rand_speed
number next_cpu2_speed_update
rand next_cpu2_speed_update 0 100
number cpu2_speed
call_result cpu2_speed rand_speed

number cfg
cfg_load cfg "com.b1stable.pong"
cfg_get_number cfg score1 "score1"
cfg_get_number cfg score2 "score2"

number font
font_load font "font.ttf" 48 1

number paddle1
number paddle2
number ball_x
number ball_y
number prev_ball_x
number prev_ball_y
number ball_vx
number ball_vy
number ball_radius
number paddle_w
number paddle_h

= ball_radius 10
= paddle1 180
= paddle2 180
= paddle_w 20
= paddle_h 50

number half
= half paddle_h
/ half 2

number wall_sfx
number point_sfx
mml_load wall_sfx "sfx/wall.mml"
mml_load point_sfx "sfx/point.mml"

function reset_ball
{
	= ball_x 320
	= ball_y 180
	= prev_ball_x 320
	= prev_ball_y 180

	number r
	rand r 0 1
	? r 0
	je left
	= ball_vx 2.5
	goto done_horiz
:left
	= ball_vx -2.5
:done_horiz

	rand r 0 1
	? r 0
	je up 
	= ball_vy 2.5
	goto done_vert
:up
	= ball_vy -2.5
:done_vert
}

function run
{
	number nj
	joystick_count nj

	include "poll_joystick.inc"

	? nj 1
	jl cpu1

	? joy_y1 0
	jl below
	? joy_y1 0.1
	jl done_joy
	* joy_y1 5.0
	+ paddle1 joy_y1
	number bot
	= bot 360
	- bot half
	? paddle1 bot
	jl done_joy
	= paddle1 bot

:below
	? joy_y1 -0.1
	jg done_joy
	* joy_y1 5.0
	+ paddle1 joy_y1
	? paddle1 half
	jg done_joy
	= paddle1 half

:done_joy
	goto done_cpu1

:cpu1
	? ball_vx 0
	jge done_cpu1

	number diff
	= diff paddle1
	- diff ball_y
	? diff 0
	jge not_neg1
	neg diff
:not_neg1
	? diff 10
	jle done_cpu1

	? paddle1 ball_y
	jl move_down1

	- paddle1 cpu1_speed
	? paddle1 half
	jge done_cpu1
	= paddle1 half
	goto done_cpu1

:move_down1

	+ paddle1 cpu1_speed
	number bott
	= bott 360
	- bott half
	? paddle1 bott
	jle done_cpu1
	= paddle1 bott

:done_cpu1

	? nj 2
	jl cpu

:player2
	joystick_poll 1 joy_x1 joy_y1 joy_x2 joy_y2 joy_x3 joy_y3 joy_l joy_r joy_u joy_d joy_a joy_b joy_x joy_y joy_lb joy_rb joy_ls joy_rs joy_back joy_start

	? joy_y1 0
	jl below2
	? joy_y1 0.1
	jl done_joy2
	* joy_y1 5.0
	+ paddle2 joy_y1
	number bot
	= bot 360
	- bot half
	? paddle2 bot
	jl done_joy2
	= paddle2 bot

:below2
	? joy_y1 -0.1
	jg done_joy2
	* joy_y1 5.0
	+ paddle2 joy_y1
	? paddle2 half
	jg done_joy2
	= paddle2 half

:done_joy2

	goto done_cpu

:cpu
	? ball_vx 0
	jle done_cpu

	number diff
	= diff paddle2
	- diff ball_y
	? diff 0
	jge not_neg
	neg diff
:not_neg
	? diff 10
	jle done_cpu

	? paddle2 ball_y
	jl move_down

	- paddle2 cpu2_speed
	? paddle2 half
	jge done_cpu
	= paddle2 half
	goto done_cpu

:move_down

	+ paddle2 cpu2_speed
	number bott
	= bott 360
	- bott half
	? paddle2 bott
	jle done_cpu
	= paddle2 bott

:done_cpu

	+ ball_x ball_vx
	? ball_x ball_radius
	jge no_point2

	call reset_ball
	mml_play point_sfx 1.0 0
	+ score2 1

:no_point2

	number right
	= right 640
	- right ball_radius
	? ball_x right
	jle no_point1

	call reset_ball
	mml_play point_sfx 1.0 0
	+ score1 1

:no_point1
	
	+ ball_y ball_vy
	? ball_y ball_radius
	jge no_bounce_top

	- ball_y ball_vy
	neg ball_vy
	mml_play wall_sfx 1.0 0

:no_bounce_top

	number bottom
	= bottom 360
	- bottom ball_radius
	? ball_y bottom
	jle no_bounce_bottom

	- ball_y ball_vy
	neg ball_vy
	mml_play wall_sfx 1.0 0

:no_bounce_bottom

	number paddle_top
	number paddle_bottom
	number paddle_x

	= paddle_top paddle1
	- paddle_top half
	- paddle_top ball_radius
	= paddle_bottom paddle_top
	+ paddle_bottom paddle_h
	+ paddle_bottom ball_radius
	+ paddle_bottom ball_radius

	? ball_y paddle_top
	jl done_hit_paddle1

	? ball_y paddle_bottom
	jg done_hit_paddle1

	= paddle_x paddle_w
	* paddle_x 2
	+ paddle_x ball_radius
	? ball_x paddle_x
	jg done_hit_paddle1

	? prev_ball_x paddle_x
	jl done_hit_paddle1

	neg ball_vx
	+ ball_vx 0.25
	+ ball_x ball_vx
	mml_play wall_sfx 1.0 0

:done_hit_paddle1

	= paddle_top paddle2
	- paddle_top half
	- paddle_top ball_radius
	= paddle_bottom paddle_top
	+ paddle_bottom paddle_h
	+ paddle_bottom ball_radius
	+ paddle_bottom ball_radius

	? ball_y paddle_top
	jl done_hit_paddle2

	? ball_y paddle_bottom
	jg done_hit_paddle2

	= paddle_x 640
	- paddle_x paddle_w
	- paddle_x paddle_w
	- paddle_x ball_radius
	? ball_x paddle_x
	jl done_hit_paddle2

	? prev_ball_x paddle_x
	jg done_hit_paddle2

	neg ball_vx
	- ball_vx 0.25
	+ ball_x ball_vx
	mml_play wall_sfx 1.0 0

:done_hit_paddle2

	= prev_ball_x ball_x
	= prev_ball_y ball_y

	- next_cpu1_speed_update 1
	? next_cpu1_speed_update 0
	jge test2
	rand next_cpu1_speed_update 0 100
	call_result cpu1_speed rand_speed

:test2
	- next_cpu2_speed_update 1
	? next_cpu2_speed_update 0
	jge slideshow
	rand next_cpu2_speed_update 0 100
	call_result cpu2_speed rand_speed

:slideshow
	include "slideshow_logic.inc"
}

function draw
{
	clear 0 0 0

	number y
	= y paddle1
	- y half

	filled_rectangle 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 paddle_w y paddle_w paddle_h

	number x
	= x 640
	- x paddle_w
	- x paddle_w

	= y paddle2
	- y half

	filled_rectangle 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 x y paddle_w paddle_h

	number bx
	number by
	number bw
	number bh
	= bx ball_x
	- bx ball_radius
	= by ball_y
	- by ball_radius
	= bw ball_radius
	* bw 2

	filled_rectangle 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 bx by bw bw

	number x1
	number x2
	= x1 320
	- x1 32
	= x2 320
	+ x2 32
	
	string text1
	string text2
	string_format text1 "%" score1
	string_format text2 "%" score2

	number w1
	number w2

	font_width font w1 text1
	font_width font w2 text2

	- x1 w1

	number y
	= y 360
	number fh
	font_height font fh
	- y fh
	- y 8

	font_draw font 255 255 255 255 text1 x1 y
	font_draw font 255 255 255 255 text2 x2 y
}

function end
{
	cfg_set_number cfg "score1" score1
	cfg_set_number cfg "score2" score2

	number save_result
	cfg_save cfg save_result "com.b1stable.pong"
}

call reset_ball
