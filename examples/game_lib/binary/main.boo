number f
font_load f "vga.ttf" 32 0

number fw
number fh

font_height f fh
font_width f fw "0"

function draw
{
	clear 0 0 0

	number cols
	number rows

	= cols 640
	/ cols fw

	= rows 360
	/ rows fh

	+ cols 1
	+ rows 1

	number x
	number y

	= y 0
:next_y
	= x 0
:next_x
	number dx
	number dy
	= dx x
	* dx fw
	= dy y
	* dy fh
	number r
	string c
	rand r 0 1
	? r 0
	jne one
	= c "0"
	goto draw_it
:one
	= c "1"
:draw_it
	font_draw f 32 32 32 255 c dx dy
	+ x 1
	? x cols
	jl next_x
	+ y 1
	? y rows
	jl next_y
}
