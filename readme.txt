
-------------------------- Quick Start Guide --------------------------

1) Place the files in your 'svencoop' folder. That folder is located here:
   /Steam/steamapps/common/Half-Life/svencoop/
   Do not place the files in 'svencoop/maps' or any other subdirectory.
   
2) Open gsrand_config.txt and scroll all the way to the bottom. You will
   see "[maps]". This is where you specify which maps you want randomized.
  
3) Start gsrand.exe. You should see a console pop up with a text menu.
   Use the number keys to change various options. To get the most random
   output, press each of these keys once: 1, 2, 3, 4, and 5.
   
4) Press 8 to begin the randomization process. The program will search
   for usable content (this may take a long time) and then ask you if
   you want to proceed with the randomization. Press 0 when that happens.

5) After the process is finished, you can launch Sven Co-op and start 
   playing the new maps! By default, each map is prefixed with "gsrand_"
   so the randomized version of "c1a0" would be named "gsrand_c1a0".
   
6) If you want to delete the new maps, Select the "Undo all changes" option
   in the program menu. You don't have to do this before running the randomizer
   again (unless you're not using a map prefix, which is dangerous).
   
   
-------------------------- Multiplayer Setup --------------------------   

Disable singleplayer in gsrand_config.txt (singleplayer = 0). This will
turn on RES file generation and enable a safety setting to prevent crashes. 
You might also want to take a look at the sentence_mode setting if you plan 
to distribute your files manually (not via the RES file).

It's a good idea to use the timestamp map prefix so that players won't get
a "Your map differs" error after you re-randomize your maps. Once you know
people have downloaded your maps, you should never use that map prefix again.

After the maps are generated, you are asked if you want to copy the
randomized maps to a folder. Select the 'copy' option so that you can easily
pack up the files to send to your buddies or your fast-dl server. If you have
7za.exe in the same folder as gsrand.exe then you will get additional options 
for compressing everything into a 7zip archive.

You can download 7za.exe here: http://www.7-zip.org/download.html
(Make sure you download the "7-Zip Command Line Version")

You might also want to limit non-default content so that players don't have
to spend a lot of time downloading on every level change. With my 10Mbps
upload speed, it took about 3-5 minutes for a single player to join my server. 
Each of my map RES files had about 200 to 300 items in it.


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
You only need to add content for the randomizations you want.
Note: Skyboxes are used only when Texture randomization is enabled.


-------------------------- Troubleshooting -------------------------- 

The game keeps crashing! What can I do?
   This is how unstable each randomization option is (on a scale from 1-10):
       Texture = 0
       Entity  = 3
       Corrupt = 1
       Model   = 47
       Sound   = 0
   Basically, by using model randomization, you are guaranteed to crash
   at some point. The other options are pretty safe to use but they still
   might cause a crash on occasion. If you really want to keep using model
   randomization, then set model_safety to "2" in gsrand_config.txt. 
   You will still crash, but it should happen less often. "Appearances only"
   is also much safer to use than "Super random" as an Entity mode.
   
Why don't I have any of the Half-Life/Blue Shift/Opposing Force maps?
   These maps are installed with Sven Co-op 4.8, but you have to have these
   games installed before you install Sven Co-op. It's not enough to copy
   the maps over manually after you've installed Sven Co-op. The SC installer 
   makes adjustments to the maps so that they work properly in SC.
   
It's not working :<
   Sorry about that. Ask a question on the forum and I'll try to help:
   http://forums.svencoop.com/showthread.php/42021-Glitch-Life-program?p=505531#post505531


----------------------------- Changelog -----------------------------

 version 4
-----------

New Features:
+ Corruption settings reworked (see gsrand_config.txt)
+ More fog randomization (change densities and randomly add fog to maps)
+ ear_rape_safety stops looping and halves volume of some sounds
+ Rotating entities and trains have a higher minimum random speed

Fixes:
- Fixed model replacement crash: player model with T submodel used for p_weapon
- Fixed model replacement crash: model with missing animation submodels used for p_weapon
- Fixed invalid models being used for replacement (e.g. valve/models/doctor.mdl)
- Fixed unicode characters in program path preventing gsrand.exe from finding files
- Fixed WADs with "gsrand" in the name being used for randomization
- Fixed custom prefixes being cutoff in shared WAD names


 version 3
-----------

New Features:
+ Added ear_rape_safety setting (for impulse 101)

Fixes:
- Fixed the "common wad" texture mode
- Fixed memory usage spikes and leaks
- Possibly fixed RNG's crash with texture modes
- Fixed grapple not being provided with corruption modes
- Fixed gsrand_ maps being included in map list
- Timestamp map prefixes are lowercase now (fixes server issues)
- Fixed spore ammo and impact noises not being randomized

Other changes:
- Real-time status updates when finding content
- Removed "Write WAD for each map" texture mode


 version 2
-----------

New Features:
+ Model randomization finished (unstable)
+ Sprite randomization
+ Skybox randomization
+ New entity randomizations
+ BSP corruptions (geometry + lighting + textures)
+ Sound effect randomization (reverb, pitch, etc.)
+ BSP model randomization (off by default)
+ Skill file generation (weapon/monster damage modifiers)
+ MOTD file generation (Helpful tips for new players)
+ RES file generation (for server hosters)
+ Non-default content is usable
+ Textures embedded in maps are usable (yay porn)
+ Cheats via entities (no restrictions!)
+ Grapple weapon can attach to most walls
+ Configuration file added for advanced settings and map selection
+ Custom and timestamp map prefixes supported
+ Randomizations can be zipped if 7za.exe is present
+ HLSP "Kill all monsters" restriction is bypassed

Fixes:
- Map prefixes work properly now
- Visual C++ Runtime no longer required
