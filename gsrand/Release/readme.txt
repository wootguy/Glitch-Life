
-------------------------- Quick Start Guide --------------------------

1) Place the files in your 'svencoop' folder. That folder is located here:
   Steam/steamapps/common/Half-Life/svencoop
   Do not place the files in 'svencoop/maps' or any other sub-directory
   because then the program will not be able to find any of your files.
   
2) Open gsrand_config.txt and scroll all the way to the bottom. You will
   see "[maps]". This is where you specify which maps you want randomized.
   Read the comments above "[maps]" for more info.
  
3) Start gsrand.exe. You should see a console pop up with a text menu.

3) Use the number keys to change various options. To get the most random
   output, press each of these keys once: 1, 2, 3, 4, and 5.
   
4) Press 8 to begin the randomization process. The program will search
   for usable content (this may take a long time) and then ask you if
   you want to proceed with the randomization.

5) After the process is finished, you will be asked about what you want
   to do with the randomized maps. Just say "Nothing" to this unless
   you want to share your maps.
   
6) Launch Sven Co-op and play through the new maps! By default, each map
   is prefixed with "gsrand_", so the randomized version of "c1a0" would
   be named "gsrand_c1a0".
   
7) If you want to delete these maps, Select the "Undo all changes" option
   in the program. "Undo all changes" only works if the map prefix is set
   to "gsrand_" or "No prefix".
   
   
-------------------------- Multiplayer Setup --------------------------   

Disable singleplayer in gsrand_config.txt (singleplayer = 0). This will
turn on RES file generation and enable a safety setting to prevent crashes. 
You might also want to take a look at the sentence_mode setting if you plan 
to distribute your files manually.

After the maps are generated, you are asked if you want to copy the
randomized maps to a folder. Select the 'copy' option so that you can easily
pack up the files to send to your buddies or your fast-dl server. If you have
7za.exe in the same folder as gsrand.exe then the program will give you
additional options to 7zip everything instead of just copying to a folder.

You can download 7za.exe here: http://www.7-zip.org/download.html
Make sure you download the "7-Zip Command Line Version".


-------------------------- Content Selection -------------------------- 

You have the power to control what content gets used in the randomizations.
Simply create a new folder and place the gsrand files inside. This new
folder has to follow the same structure as your svencoop folder:

   your_gsrand_folder
    |-- gsrand.exe
    |-- gsrand_config.txt
    |-- gsrand_models.txt
	|-- gfx
    |    |-- env
    |    |    |-- sky_up.tga
    |    |    |-- sky_dn.tga
    |    |    |-- sky_lf.tga
    |    |    |-- sky_rt.tga
    |    |    |-- sky_ft.tga
    |    |    |-- sky_bk.tga
    |-- maps
    |    |-- stadium3.bsp
    |    |-- uni2.bsp
	|    |-- horsecock7.bsp
    |-- models
    |    |-- zombie.mdl
    |    |-- p_crowbar.mdl
	|    |-- v_crowbar.mdl
	|    |-- w_crowbar.mdl
    |-- sound
    |    |-- thunder.wav
    |    |-- beepboop.wav
    |-- sprites
    |    |-- fire.spr
    |    |-- teleport.spr

The program will only use content that it finds in this new folder.
If you're using model randomization, then make sure you have at least
one monster model and one set of weapon models. For example,
zombie.mdl + w_crowbar.mdl + p_crowbar.mdl + v_crowbar.mdl


-------------------------- Troubleshooting -------------------------- 

The game keeps crashing! What can I do?
   This is how dangerous each randomization option is (on a scale of 1-10):
       Texture = 0
       Entity  = 3 ("Appearances only" mode is safer)
	   Corrupt = 1
	   Model   = 47 (chubby code - write it down)
	   Sound   = 0
   Basically, by using model randomization, you are guaranteed to crash
   at some point. The other options are pretty safe to use but they still
   might cause a crash on occasion. If you really want to keep using model
   randomization, then set model_safety to "2" in gsrand_config.txt. 
   You will still crash, but it should happen less often.
   
It's not working :<
   Sorry about that. Ask a question on the forum and I'll try to help:
   *INSERT LINK TO FORUM POST*
   
