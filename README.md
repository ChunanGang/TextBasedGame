# JUST LIKE ANIMAL

### Author: 

Chunan(Oscar) Huang

### Design: 

A question-based test that tells you what animal you could be, if not human.

### Text Drawing: 

The text drawing pipeline relies on the following 4 files: <br>

    TextRenderer.hpp / TextRenderer.cpp 
        These 2 files define a TextRenderer class which takes in a ttf font file and construct a renderer object.
        You can then call renderer.draw(...) to display text.
    TextRenderProgram.hpp / TextRenderProgram.cpp 
        These 2 files define the shader program that is used by the TextRenderer class. 

Some details about the TextRenderer class: <br>
It uses FreeType to read the ttf font file, and extracts the glyphs from the face (font). <br>
It uses harfbuzz to shape a user-input text so that we know how to render the glyphs. <br>
And finally it uses OpenGL to render the glyphs. <br>
For better performance, durting init, i read all common-used glpyphs (ie. english letters) <br>
from the face and convert these glyphs into bitmap and store in the buffer. <br>
A map will store the links between char and the textureID, so that during runtime it directly renders the <br>
bitmap in buffer using the textureID.  <br>

### Screen Shot:

![Screen Shot](screenshot.png)

### How To Play:

Press buttons A,B,C on kayboard to select answer. Press Enter to move on.

### Sources: 

This game was built with [NEST](NEST.md). <br>
The code is developed from https://github.com/15-466/15-466-f21-base4 <br>
And the text rendering pipeline is based on : <br>
https://github.com/harfbuzz/harfbuzz-tutorial/blob/master/hello-harfbuzz-freetype.c
https://www.freetype.org/freetype2/docs/tutorial/step1.html
https://learnopengl.com/In-Practice/Text-Rendering
Font assets:
https://www.fontspace.com/open-sans-font-f22353 <br>
https://www.fontspace.com/serat-font-f57208<br>
https://www.fontspace.com/broadway-font-f57397<br>
https://www.fontspace.com/really-font-f45186<br>
