#!/usr/bin/python
import math
import sys
import pyglet
from pyglet.gl import *
from pyglet.window import mouse
from pyglet.image.codecs.png import PNGImageDecoder

###############################################################################
## Constants and global variables
###############################################################################
TILE_SIZE = 32

if sys.argv and len(sys.argv) > 1:
    TILE_IMAGE = pyglet.image.load('../assets/tilemaps/' + sys.argv[1] + '.png')
else:
    TILE_IMAGE = pyglet.image.load('../assets/tilemaps/jungle.png')

HR_IMAGE = pyglet.image.load('./assets/images/hr.png')
W = 25;
H = 16;
selected_tile_x = 0
selected_tile_y = 2

###############################################################################
## Define the 2D array that stores each tile coordinate
###############################################################################
class Point2D:
    x = 0
    y = 0

Matrix = [[Point2D() for x in range(H)] for y in range(W)]

###############################################################################
## Populate the 2D array with empty tile values
###############################################################################
for i in range(W):
    for j in range(H):
        Matrix[i][j].x = -1
        Matrix[i][j].y = -1

###############################################################################
## Renders the main program window
###############################################################################
window = pyglet.window.Window(caption='Tilemap Editor', width=800, height=640)
window.set_icon(pyglet.image.load('./assets/images/icon.png'))
cursor = window.get_system_mouse_cursor(window.CURSOR_HAND)
window.set_mouse_cursor(cursor)
pyglet.gl.glClearColor(0.1, 0.1, 0.1, 1)

###############################################################################
## Display the tiles that can be clicked to change the current selected tile
###############################################################################
def draw_tile_map_selector():
    HR_IMAGE.blit(0, 0)
    TILE_IMAGE.blit(0, 0)
    selected_tile_part = TILE_IMAGE.get_region(
        x=selected_tile_x * TILE_SIZE,
        y=selected_tile_y * TILE_SIZE,
        width=TILE_SIZE,
        height=TILE_SIZE
    )
    selected_tile_part.blit(365, 37)

###############################################################################
## Draw UI labels and text
###############################################################################
def draw_ui_label():
    label_title = pyglet.text.Label(
        'TILEMAP EDITOR',
        font_name='Hermit',
        font_size=10,
        x=460,
        y=85,
        anchor_x='left',
        anchor_y='bottom'
    )
    label_1 = pyglet.text.Label(
        '1. click one of the tiles to select',
        font_name='Hermit',
        font_size=10,
        x=460,
        y=60,
        anchor_x='left',
        anchor_y='bottom'
    )
    label_2 = pyglet.text.Label(
        '2. use the mouse to draw on the map',
        font_name='Hermit',
        font_size=10,
        x=460,
        y=45,
        anchor_x='left',
        anchor_y='bottom'
    )
    label_3 = pyglet.text.Label(
        '3. press "w" to write the map file',
        font_name='Hermit',
        font_size=10,
        x=460,
        y=30,
        anchor_x='left',
        anchor_y='bottom'
    )
    label_4 = pyglet.text.Label(
        '4. press "q" to quit the editor',
        font_name='Hermit',
        font_size=10,
        x=460,
        y=15,
        anchor_x='left',
        anchor_y='bottom'
    )
    label_title.draw()
    label_1.draw()
    label_2.draw()
    label_3.draw()
    label_4.draw()

###############################################################################
## Saves the currently selected tile in the 2D array
###############################################################################
def save_tile_in_array(x, y):
    global selected_tile_x
    global selected_tile_y
    # Select tile
    if x >= 0 and x <= 320 and y >= 0 and y < 96:
        selected_tile_x = math.floor(x / 32);
        selected_tile_y = math.floor(y / 32);
    # Draw tile
    if x >= 0 and x < 800 and y >= 128 and y < 640:
        target_x = math.floor(x / 32)
        target_y = math.floor((640 - y) / 32)
        p_2d = Point2D()
        p_2d.x = selected_tile_x
        p_2d.y = selected_tile_y
        Matrix[target_x][target_y] = p_2d

###############################################################################
## Loop the array and draw each selected tile in the screen
###############################################################################
def draw_array_of_selected_tiles():
    for i in range(W):
        for j in range(H):
            target_tile_x = Matrix[i][j].x
            target_tile_y = Matrix[i][j].y
            target_pixel_x = i * TILE_SIZE
            target_pixel_y = 640 - j * TILE_SIZE
            if target_tile_x != -1 and target_tile_y != -1:
                current_tile_part = TILE_IMAGE.get_region(
                    x=target_tile_x * TILE_SIZE,
                    y=target_tile_y * TILE_SIZE,
                    width=TILE_SIZE,
                    height=TILE_SIZE
                )
                current_tile_part.blit(target_pixel_x, target_pixel_y - 32)

###############################################################################
## Save the selected tiles in a file
###############################################################################
def save_array_to_file():
    with open('../assets/maps/./saved.map', 'w') as file:
        file.truncate(0)
        for j in range(H):
            for i in range(W):
                x = Matrix[i][j].x
                y = Matrix[i][j].y
                if x == -1 and y == -1:
                    x = y = 0
                val = str(y) + str(x)
                if i < W - 1:
                    #val += ","
                    val += " "

                file.write(val)
            file.write('\n')

###############################################################################
## Event listener on key press
###############################################################################
@window.event
def on_key_press(symbol, modifiers):
    if symbol == 119:
        save_array_to_file()
    if symbol == 113:
        exit()

###############################################################################
## Event listener on mouse press
###############################################################################
@window.event
def on_mouse_press(x, y, buttons, modifiers):
    save_tile_in_array(x, y)

###############################################################################
## Event listener on mouse drag
###############################################################################
@window.event
def on_mouse_drag(x, y, dx, dy, buttons, modifiers):
    save_tile_in_array(x, y)

###############################################################################
## Render loop
###############################################################################
@window.event
def on_draw():
    window.clear()
    draw_tile_map_selector()
    draw_array_of_selected_tiles()
    draw_ui_label()

###############################################################################
## Triggers graphics app execution
###############################################################################
pyglet.app.run()
