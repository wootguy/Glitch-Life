Place gsrand.exe in your SvenCoop folderand run it!

Or,

You can put gsrand in it's own folder with only the
maps you want changed in a 'maps' folder next to gsrand. 
You will also need to copy over the wads from your Sven Co-op 
directory or else no textures can be replaced.


----------- Known Issues ---------------

Game crashes with this error: "Texture Overflow: MAX_GLTEXTURES"

This happens consistently when using model replacement. Just restart the game
and continue where you left off (the beginning of the next level, since this
crash happens on level changes).



Game crashes with this error: "SZ_GetSpace: overflow without FSB_ALLOWOVERFLOW set on Server Reliable Datagram"

Caused by entity randomizations. Unless you think you know what triggered it 
(e.g. elevator button) you'll have to skip the map that gave you this error.
Running the randomizer again probably won't fix the issue.


Game crashes with no error:

Probably caused by entity or model randomization. Re-running the randomizer
might fix the issue.


