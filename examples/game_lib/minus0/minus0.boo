number count
= count 0
number size
= size 300

function my_rect c x1 y1 x2 y2
{
	number w
	number h
	= w x2
	- w x1
	= h y2
	- h y1
	filled_rectangle c c c 255 c c c 255 c c c 255 c c c 255 x1 y1 w h
}

function draw_zero w x1 y1 x2 y2
{
	number xx1
	number yy1
	number xx2
	number yy2

	; left
	= xx1 x1
	= xx2 xx1
	+ xx2 w
	= yy1 y1
	= yy2 y2
	call my_rect 0 xx1 yy1 xx2 yy2

	; right
	= xx1 x2
	- xx1 w
	= xx2 x2
	= yy1 y1
	= yy2 y2
	call my_rect 0 xx1 yy1 xx2 yy2

	; top
	= xx1 x1
	= xx2 x2
	= yy1 y1
	= yy2 yy1
	+ yy2 w
	call my_rect 0 xx1 yy1 xx2 yy2

	; bottom
	= xx1 x1
	= xx2 x2
	= yy1 y2
	- yy1 w
	= yy2 y2
	call my_rect 0 xx1 yy1 xx2 yy2
}

function draw_slash w x1 y1 x2 y2
{
	number xx1
	number yy1
	number xx2
	number yy2

	= xx1 x2
	- xx1 w
	- xx1 w
	= xx2 xx1
	+ xx2 w
	= yy1 y1
	+ yy1 w
	= yy2 yy1
	+ yy2 w
	call my_rect 0 xx1 yy1 xx2 yy2

	= xx1 x1
	+ xx1 w
	= xx2 xx1
	+ xx2 w
	= yy1 y2
	- yy1 w
	- yy1 w
	= yy2 yy1
	+ yy2 w
	call my_rect 0 xx1 yy1 xx2 yy2

	number half
	= half w
	/ half 2

	= xx1 320
	- xx1 half
	= yy1 180
	- yy1 half
	= xx2 320
	+ xx2 half
	= yy2 180
	+ yy2 half
	call my_rect 0 xx1 yy1 xx2 yy2
}

function draw_minus w x1 y1 x2 y2
{
	number mid
	= mid y1
	+ mid y2
	/ mid 2

	number half
	= half w
	/ half 2

	number yy1
	number yy2
	= yy1 mid
	- yy1 half
	= yy2 mid
	+ yy2 half

	call my_rect 0 x1 yy1 x2 yy2
}

function fix_zero x1 y1 x2 y2
{
	; left
	call my_rect 255 0 0 x1 360
	; right
	call my_rect 255 x2 0 640 y2
	; top
	call my_rect 255 0 0 640 y1
	; bottom
	call my_rect 255 0 y2 640 360
}

function draw_all w
{
	clear 255 255 255

	number half
	= half size
	/ half 2

	number x1
	number y1
	number x2
	number y2
	= x1 320
	- x1 half
	= x2 320
	+ x2 half
	= y1 180
	- y1 half
	= y2 180
	+ y2 half

	call draw_zero w x1 y1 x2 y2
	call draw_slash w x1 y1 x2 y2
	call draw_minus w x1 y1 x2 y2
	call fix_zero x1 y1 x2 y2
}

function draw
{
	number fifth
	= fifth size
	/ fifth 5

	number w
	= w count
	fmod w 64.0
	/ w 64.0
	? w 0.5
	jge pingpong
	/ w 0.5
	goto ok

:pingpong
	- w 0.5
	/ w 0.5
	number tmp
	= tmp 1.0
	- tmp w
	= w tmp

:ok
	* w fifth

	number close
	= close fifth
	- close 2.5
	? w close
	jle ok2
	= w fifth
	call draw_all w
	flip
	delay 5000
	= count 0

:ok2

	call draw_all w
}

function run
{
	+ count 1
}
