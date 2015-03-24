#include "bsp_util.h"
#include "gsrand.h"

#define PLANE_X 0     // Plane is perpendicular to given axis
#define PLANE_Y 1
#define PLANE_Z 2
#define PLANE_ANYX 3  // Non-axial plane is snapped to the nearest
#define PLANE_ANYY 4
#define PLANE_ANYZ 5

struct BSPPLANE
{
    vec3 vNormal; // The planes normal vector
    float fDist;  // Plane equation is: vNormal * X = fDist
    int nType;    // Plane type, see #defines
};

struct BSPMODEL
{
    float nMins[3], nMaxs[3];      // Defines bounding box
    vec3 vOrigin;                  // Coordinates to move the // coordinate system
    int iHeadnodes[4];             // Index into nodes array
    int nVisLeafs;                 // ???
    int iFirstFace, nFaces;        // Index and count into faces
};

struct BSPNODE
{
    int iPlane;            // Index into Planes lump
    short iChildren[2];       // If > 0, then indices into Nodes // otherwise bitwise inverse indices into Leafs
    short nMins[3], nMaxs[3]; // Defines bounding box
    unsigned short firstFace, nFaces; // Index and count into Faces
};

struct BSPCLIPNODE
{
    int iPlane;       // Index into planes
    short iChildren[2]; // negative numbers are contents
};

struct BSPLEAF
{
    int nContents;                         // Contents enumeration
    int nVisOffset;                        // Offset into the visibility lump
    short nMins[3], nMaxs[3];                // Defines bounding box
    unsigned short iFirstMarkSurface, nMarkSurfaces; // Index and count into marksurfaces array
    byte nAmbientLevels[4];                 // Ambient sound levels
};

struct BSPFACE
{
    unsigned short iPlane;          // Plane the face is parallel to
    unsigned short nPlaneSide;      // Set if different normals orientation
    unsigned int iFirstEdge;      // Index of the first surfedge
    unsigned short nEdges;          // Number of consecutive surfedges
    unsigned short iTextureInfo;    // Index of the texture info structure
    byte nStyles[4];       // Specify lighting styles
    unsigned int nLightmapOffset; // Offsets into the raw lightmap data
};

struct BSPTEXTUREINFO
{
    vec3 vS; 
    float fSShift;    // Texture shift in s direction
    vec3 vT; 
    float fTShift;    // Texture shift in t direction
    uint iMiptex; // Index into textures array
    uint nFlags;  // Texture flags, seem to always be 0
};

bool print_clip = false;
bool print_face = false;
bool print_before = false;

// return true if also child flip
bool do_plane_flip(BSPPLANE& p, int idx, bool is_clip)
{
	bool flip_contents = false; // flip BSP nodes in tree
	if (is_clip && print_clip && print_before)
		println("CLIP " + str(idx) + ": " + str(p.vNormal.x) + " " + str(p.vNormal.y) + " " + str(p.vNormal.z) + "\t" + str(p.nType) + " " + str(p.fDist));
	else if (!is_clip && print_face && print_before)
		println("FACE " + str(idx) + ": " + str(p.vNormal.x) + " " + str(p.vNormal.y) + " " + str(p.vNormal.z) + "\t" + str(p.nType) + " " + str(p.fDist));

	if ((p.vNormal.x > 0 && p.vNormal.y < 0 && p.vNormal.z > 0))         // + - +
	{
		p.vNormal.x *= -1;
	}
	else if (//(p.vNormal.x == 0 && p.vNormal.y < 0 && p.vNormal.z < 0) || // (negative fDist only?) 0 - - 
			 (p.vNormal.x < 0 && p.vNormal.y == 0 && p.vNormal.z < 0) || // - 0 -
			 (p.vNormal.x > 0 && p.vNormal.y == 0 && p.vNormal.z < 0))   // + 0 -
	{
		p.vNormal.z *= -1;
	}
	else if ((p.vNormal.x != 0 && p.vNormal.y < 0 && p.vNormal.z < 0) || // + - -   - - -
			 (p.vNormal.x > 0 && p.vNormal.y > 0 && p.vNormal.z < 0) ||  // + + -
			 (p.vNormal.x == 0 && p.vNormal.y > 0 && p.vNormal.z < 0) ||
			 (p.vNormal.x == 0 && p.vNormal.y < 0 && p.vNormal.z < 0))   // 0 - - (positive fDist only?)
	{			
		p.vNormal.y *= -1;															     
		p.vNormal.z *= -1;		
	}
	else if (//(p.vNormal.x == 0 && p.vNormal.y > 0 && p.vNormal.z < 0) || // (negative fDist only?) 0 + - 
			 (p.vNormal.x == 0 && p.vNormal.y < 0 && p.vNormal.z > 0) ||   // 0 - + 
			 (p.vNormal.x == 0 && p.vNormal.y == 0 && p.vNormal.z > 0) ||  // 0 0 +
			 (p.vNormal.x == 0 && p.vNormal.y > 0 && p.vNormal.z == 0))    // 0 + 0
	{
		p.fDist *= -1.0f;	
		flip_contents = true;		
	}
	else if ((p.vNormal.x > 0 && p.vNormal.y == 0 && p.vNormal.z != 0) ||   // + 0 +   + 0 0   + 0 -
				(p.vNormal.x == 0 && p.vNormal.y > 0 && p.vNormal.z > 0) || // 0 + +
				(p.vNormal.x < 0 && p.vNormal.y <= 0 && p.vNormal.z > 0) || // - 0 +   - - +
				(p.vNormal.x < 0 && p.vNormal.y > 0 && p.vNormal.z <= 0))   // - + 0   - + -
	{
		p.fDist *= -1.0f;
		p.vNormal.x *= -1;
		flip_contents = true;
	}
	else if (p.vNormal.x != 0 && p.vNormal.y > 0 && p.vNormal.z > 0) // 0 + +    + + +    - + +
	{
		p.vNormal.x *= -1;
		p.fDist *= -1.0f;	
		flip_contents = true;			
	}
	else if ((p.vNormal.x > 0 && p.vNormal.y == 0 && p.vNormal.z == 0)) // + 0 0
	{
		// do nothing, x isn't affected
	}
	else if (p.vNormal.x > 0 && p.vNormal.z <= 0 ||                     // + ? -    + ? 0
		(p.vNormal.x < 0 && p.vNormal.y < 0 && p.vNormal.z == 0))       // - - 0
	{
		p.vNormal.y *= -1;
	}
	else
	{
		string sign_x = "0";
		string sign_y = "0";
		string sign_z = "0";
		if (p.vNormal.x > 0) sign_x = "+";
		if (p.vNormal.y > 0) sign_y = "+";
		if (p.vNormal.z > 0) sign_z = "+";
		if (p.vNormal.x < 0) sign_x = "-";
		if (p.vNormal.y < 0) sign_y = "-";
		if (p.vNormal.z < 0) sign_z = "-";
		println("UNKNOWN COMBO: " + sign_x + " " + sign_y + " " + sign_z);
	}
	if (is_clip && print_clip && !print_before)	
		println("AFTE " + str(idx) + ": " + str(p.vNormal.x) + " " + str(p.vNormal.y) + " " + str(p.vNormal.z) + "\t" + str(p.nType) + " " + str(p.fDist));		
	else if (!is_clip && print_face && !print_before)
		println("AFTF " + str(idx) + ": " + str(p.vNormal.x) + " " + str(p.vNormal.y) + " " + str(p.vNormal.z) + "\t" + str(p.nType) + " " + str(p.fDist));
	return flip_contents;
}

// return true if axis alignment changes
bool do_plane_stretch(BSPPLANE& plane, float scaleX, float scaleY, float scaleZ)
{
	int old_axis = plane.nType;
	float x = plane.vNormal.x * fabs(plane.fDist) * scaleX;
	float y = plane.vNormal.y * fabs(plane.fDist) * scaleY;
	float z = plane.vNormal.z * fabs(plane.fDist) * scaleZ;
	float dist = sqrt( (x*x) + (y*y) + (z*z) );
	float d = 1.0f / dist;
	plane.vNormal.x = x * d;
	plane.vNormal.y = y * d;
	plane.vNormal.z = z * d;
	plane.fDist = plane.fDist > 0 ? dist : -dist;
	if (plane.nType > PLANE_Z) // angular plane axis might have changed
	{
		x = fabs(plane.vNormal.x);
		y = fabs(plane.vNormal.y);
		z = fabs(plane.vNormal.z);
		if (x > y && x > z) plane.nType = PLANE_ANYX;
		if (y > x && y > z) plane.nType = PLANE_ANYY;
		if (z > x && z > y) plane.nType = PLANE_ANYZ;
	}
	return old_axis != plane.nType;
}

void corrupt_map_verts(BSP * map, Entity ** ents)
{
	byte * vert_lump = map->lumps[LUMP_VERTICES];
	byte * planes_lump = map->lumps[LUMP_PLANES];
	byte * surf_lump = map->lumps[LUMP_SURFEDGES];
	byte * models_lump = map->lumps[LUMP_MODELS];
	byte * leaf_lump = map->lumps[LUMP_LEAVES];
	byte * node_lump = map->lumps[LUMP_NODES];
	byte * faces_lump = map->lumps[LUMP_FACES];
	byte * clip_lump = map->lumps[LUMP_CLIPNODES];
	byte * tex_lump = map->lumps[LUMP_TEXINFO];

	int verts = map->header.lump[LUMP_VERTICES].nLength / (3*4);
	int planes = map->header.lump[LUMP_PLANES].nLength / sizeof(BSPPLANE);
	int surfs = map->header.lump[LUMP_SURFEDGES].nLength / 4;
	int models = map->header.lump[LUMP_MODELS].nLength / sizeof(BSPMODEL);
	int leaves = map->header.lump[LUMP_LEAVES].nLength / sizeof(BSPLEAF);
	int nodes = map->header.lump[LUMP_NODES].nLength / sizeof(BSPNODE);
	int faces = map->header.lump[LUMP_FACES].nLength / sizeof(BSPFACE);
	int clips = map->header.lump[LUMP_CLIPNODES].nLength / sizeof(BSPCLIPNODE);
	int texs = map->header.lump[LUMP_TEXINFO].nLength / sizeof(BSPTEXTUREINFO);

	float vertScaleX = 0.5f;
	float vertScaleY = 0.5f;
	float vertScaleZ = 0.5f;
	int vertDistort = 4;
	println("");
	if (vertMode & VERT_FLIP)
	{
		for (int i = 0; i < MAX_MAP_ENTITIES; i++)
		{
			if (ents[i] == NULL)
				break;
			string cname = ents[i]->keyvalues["classname"];
			if (matchStr(cname, "squadmaker") || matchStr(cname, "monstermaker") || matchStr(cname, "env_xenmaker"))
				cname = ents[i]->keyvalues["monstertype"];

			if (ents[i]->keyvalues.find("origin") != ents[i]->keyvalues.end())
			{
				vector<string> coords = splitString(ents[i]->keyvalues["origin"], " ");
				if (coords.size() >= 3)
				{
					coords[1] = str( atoi(coords[1].c_str()) * -1 );
					coords[2] = str( atoi(coords[2].c_str()) * -1 );
					ents[i]->keyvalues["origin"] = coords[0] + " " + coords[1] + " " + coords[2];
				}
			}

			if (matchStr(cname, "func_door") || matchStr(cname, "func_water"))
			{
				if (ents[i]->keyvalues.find("angles") != ents[i]->keyvalues.end())
				{
					vector<string> angles = splitString(ents[i]->keyvalues["angles"], " ");
					if (angles.size() >= 3)
					{
						angles[0] = str( atoi(angles[0].c_str()) * -1 );
						//angles[1] = str( atoi(angles[1].c_str()) * -1 );
						//angles[2] = str( atoi(angles[2].c_str()) * -1 );
						ents[i]->keyvalues["angles"] = angles[0] + " " + angles[1] + " " + angles[2];
					} 
				}

				if (ents[i]->keyvalues.find("angle") != ents[i]->keyvalues.end())
				{
					int a = atoi(ents[i]->keyvalues["angle"].c_str());
					if      (a == 270) a = 90;
					else if (a == 90) a = 270;
					else if (a == -2) a = -1;
					else if (a == -1) a = -2;

					ents[i]->keyvalues["angle"] = str(a);
				}
			}
			if (matchStr(cname, "func_plat") || matchStr(cname, "func_platrot"))
			{
				if (ents[i]->keyvalues.find("height") != ents[i]->keyvalues.end())
					ents[i]->keyvalues["height"] = str( atoi(ents[i]->keyvalues["height"].c_str()) * -1 );
			}
			if (cname.find("info_player_") == 0)
			{
				vector<string> coords = splitString(ents[i]->keyvalues["origin"], " ");
				if (coords.size() == 3)
				{
					coords[2] = str( atoi(coords[2].c_str()) - 8 );
					ents[i]->keyvalues["origin"] = coords[0] + " " + coords[1] + " " + coords[2];
				}
				else
					ents[i]->keyvalues["origin"] = "0 0 -8";
			}
			if (matchStr(cname, "info_teleport_destination") || matchStr(cname, "scripted_sequence") || 
			    matchStr(cname, "aiscripted_sequence"))
			{
				vector<string> coords = splitString(ents[i]->keyvalues["origin"], " ");
				if (coords.size() == 3)
				{
					coords[2] = str( atoi(coords[2].c_str()) - 80 );
					ents[i]->keyvalues["origin"] = coords[0] + " " + coords[1] + " " + coords[2];
				}
				else
					ents[i]->keyvalues["origin"] = "0 0 -80";
			}
			if (matchStr(cname, "monster_tentacle") || matchStr(cname, "monster_tripmine") ||
				matchStr(cname, "cycler") || matchStr(cname, "cycler_weapon") || matchStr(cname, "monster_generic") || 
				matchStr(cname, "monster_furniture") || cname.find("item_") == 0 || cname.find("weapon_") == 0 ||
				cname.find("ammo_") == 0 || matchStr(cname, "evn_sprite") || matchStr(cname, "env_shooter") ||
				cname.find("xen_") == 0 || matchStr(cname, "monster_sitting_scientist") ||
				matchStr(cname, "monster_nihilanth") || matchStr(cname, "monster_alien_controller"))
			{
				vector<string> angles = splitString(ents[i]->keyvalues["angles"], " ");
				if (angles.size() == 3)
				{
					angles[2] = str( atoi(angles[0].c_str()) + 180 );
					ents[i]->keyvalues["angles"] = angles[0] + " " + angles[1] + " " + angles[2];
				}
				else
					ents[i]->keyvalues["angles"] = "0 0 180";
			}
			if (matchStr(cname, "monster_nihilanth"))
			{
				vector<string> coords = splitString(ents[i]->keyvalues["origin"], " ");
				if (coords.size() == 3)
				{
					coords[2] = str( atoi(coords[2].c_str()) - 3000 );
					ents[i]->keyvalues["origin"] = coords[0] + " " + coords[1] + " " + coords[2];
				}
				else
					ents[i]->keyvalues["origin"] = "0 0 -3000";
				vector<string> angles = splitString(ents[i]->keyvalues["angles"], " ");
				if (angles.size() == 3)
				{
					angles[2] = str( atoi(angles[0].c_str()) + 25 );
					ents[i]->keyvalues["angles"] = angles[0] + " " + angles[1] + " " + angles[2];
				}
				else
					ents[i]->keyvalues["angles"] = "0 0 25";
			}
			if (matchStr(cname, "monster_turret") || 
				matchStr(cname, "monster_miniturret"))
			{
				// TODO: side mounts can still break
				ents[i]->keyvalues["orientation"] = str( !atoi(ents[i]->keyvalues["orientation"].c_str()) );
			}
		}
		for (int i = 0; i < verts; i++)
		{
			vec3& v = ((vec3*)vert_lump)[i];
			v.z *= -1.0f;
			v.y *= -1.0f;
		}

		int * flips = new int[planes]; // don't flip planes more than once!
		for (int i = 0; i < planes; i++) flips[i] = 0;
		//println("PLANES: " + str(planes));

		for (int i = 0; i < clips; i++)
		{
			BSPCLIPNODE& m = ((BSPCLIPNODE*)clip_lump)[i];
			BSPPLANE& p = ((BSPPLANE*)planes_lump)[m.iPlane];
			
			if (!flips[m.iPlane])
			{
				flips[m.iPlane] = 1;
				if (do_plane_flip(p, m.iPlane, true))
					flips[m.iPlane] = 2;
			}
			if (flips[m.iPlane] == 2)
			{
				short temp = m.iChildren[0];
				m.iChildren[0] = m.iChildren[1];
				m.iChildren[1] = temp;
			}
		}
		for (int i = 0; i < nodes; i++)
		{
			BSPNODE& m = ((BSPNODE*)node_lump)[i];
			BSPPLANE& p = ((BSPPLANE*)planes_lump)[m.iPlane];
			short tempy = m.nMins[1];
			short tempz = m.nMins[2];
			m.nMins[1] = -m.nMaxs[1];
			m.nMins[2] = -m.nMaxs[2];
			m.nMaxs[1] = -tempy;
			m.nMaxs[2] = -tempz;

			if (!flips[m.iPlane])
			{
				flips[m.iPlane] = 1;
				if (do_plane_flip(p, m.iPlane, false))
					flips[m.iPlane] = 2;
			}
			if (flips[m.iPlane] == 2)
			{
				short temp = m.iChildren[0];
				m.iChildren[0] = m.iChildren[1];
				m.iChildren[1] = temp;
			}
		}
		
		for (int i = 0; i < faces; i++)
		{
			BSPFACE& m = ((BSPFACE*)faces_lump)[i];
			BSPPLANE& p = ((BSPPLANE*)planes_lump)[m.iPlane];

			if (!flips[m.iPlane])
			{
				flips[m.iPlane] = 1;
				if (do_plane_flip(p, m.iPlane, false))
					flips[m.iPlane] = 2;
			}
			if (flips[m.iPlane] == 2)
				m.nPlaneSide = m.nPlaneSide == 0;
		}
		delete [] flips;

		for (int i = 0; i < texs; i++)
		{
			BSPTEXTUREINFO& t = ((BSPTEXTUREINFO*)tex_lump)[i];
			t.vS.y *= -1;
			t.vS.z *= -1;

			t.vT.y *= -1;
			t.vT.z *= -1;
		}

		for (int i = 0; i < models; i++)
		{
			BSPMODEL& m = ((BSPMODEL*)models_lump)[i];
			m.vOrigin.z *= -1.0f;
			m.vOrigin.y *= -1.0f;
			float tempy = m.nMins[1];
			float tempz = m.nMins[2];
			m.nMins[1] = -m.nMaxs[1];
			m.nMins[2] = -m.nMaxs[2];
			m.nMaxs[1] = -tempy;
			m.nMaxs[2] = -tempz;
		}
		
		for (int i = 0; i < leaves; i++)
		{
			BSPLEAF& m = ((BSPLEAF*)leaf_lump)[i];
			short tempy = m.nMins[1];
			short tempz = m.nMins[2];
			m.nMins[1] = -m.nMaxs[1];
			m.nMins[2] = -m.nMaxs[2];
			m.nMaxs[1] = -tempy;
			m.nMaxs[2] = -tempz;
		}
	}

	if (vertMode & VERT_SCALE)
	{
		for (int i = 0; i < MAX_MAP_ENTITIES; i++)
		{
			if (ents[i] == NULL)
				break;
			string cname = ents[i]->keyvalues["classname"];

			if (ents[i]->keyvalues.find("origin") != ents[i]->keyvalues.end())
			{
				vector<string> coords = splitString(ents[i]->keyvalues["origin"], " ");
				if (coords.size() >= 3)
				{
					coords[0] = str( atoi(coords[0].c_str()) * vertScaleX );
					coords[1] = str( atoi(coords[1].c_str()) * vertScaleY );
					coords[2] = str( atoi(coords[2].c_str()) * vertScaleZ );
					ents[i]->keyvalues["origin"] = coords[0] + " " + coords[1] + " " + coords[2];
				}
			}

			if (matchStr(cname, "func_plat") || matchStr(cname, "func_platrot"))
			{
				if (ents[i]->keyvalues.find("height") != ents[i]->keyvalues.end())
					ents[i]->keyvalues["height"] = str( atoi(ents[i]->keyvalues["height"].c_str()) * vertScaleZ );
			}
			if (matchStr(cname, "func_door") || matchStr(cname, "func_button") || matchStr(cname, "func_water"))
			{
				if (ents[i]->keyvalues.find("lip") != ents[i]->keyvalues.end())
					ents[i]->keyvalues["lip"] = str( atoi(ents[i]->keyvalues["lip"].c_str()) * vertScaleX ); // todo: find dir
			}
		}
		for (int i = 0; i < verts; i++)
		{
			vec3& v = ((vec3*)vert_lump)[i];
			v.x *= vertScaleX;
			v.y *= vertScaleY;
			v.z *= vertScaleZ;
		}
		
		for (int i = 0; i < planes; i++)
		{
			BSPPLANE& plane = ((BSPPLANE*)planes_lump)[i];
			//println("\nBEFORE: " + str(plane.vNormal.x) + " " +  str(plane.vNormal.y) + " " +  str(plane.vNormal.z) + " " +  str(plane.fDist));
			do_plane_stretch(plane, vertScaleX, vertScaleY, vertScaleZ);
			//println("AFTERR: " + str(plane.vNormal.x) + " " +  str(plane.vNormal.y) + " " +  str(plane.vNormal.z) + " " +  str(plane.fDist));	
		}

		for (int i = 0; i < nodes; i++)
		{
			BSPNODE& m = ((BSPNODE*)node_lump)[i];
			m.nMins[0] *= vertScaleX;
			m.nMins[1] *= vertScaleY;
			m.nMins[2] *= vertScaleZ;
			m.nMaxs[0] *= vertScaleX;
			m.nMaxs[1] *= vertScaleY;
			m.nMaxs[2] *= vertScaleZ;
		}
		for (int i = 0; i < texs; i++)
		{
			BSPTEXTUREINFO& t = ((BSPTEXTUREINFO*)tex_lump)[i];
			t.vS.x *= 1.0f/vertScaleX;
			t.vS.y *= 1.0f/vertScaleY;
			t.vS.z *= 1.0f/vertScaleZ;
			t.vT.x *= 1.0f/vertScaleX;
			t.vT.y *= 1.0f/vertScaleY;
			t.vT.z *= 1.0f/vertScaleZ;
		}

		for (int i = 0; i < models; i++)
		{
			BSPMODEL& m = ((BSPMODEL*)models_lump)[i];
			m.vOrigin.x *= vertScaleX;
			m.vOrigin.y *= vertScaleY;
			m.vOrigin.z *= vertScaleZ;
			m.nMins[0] *= vertScaleX;
			m.nMins[1] *= vertScaleY;
			m.nMins[2] *= vertScaleZ;
			m.nMaxs[0] *= vertScaleX;
			m.nMaxs[1] *= vertScaleY;
			m.nMaxs[2] *= vertScaleZ;
		}
		
		for (int i = 0; i < leaves; i++)
		{
			BSPLEAF& m = ((BSPLEAF*)leaf_lump)[i];
			m.nMins[0] *= vertScaleX;
			m.nMins[1] *= vertScaleY;
			m.nMins[2] *= vertScaleZ;
			m.nMaxs[0] *= vertScaleX;
			m.nMaxs[1] *= vertScaleY;
			m.nMaxs[2] *= vertScaleZ;
		}
	}

	if (vertMode & VERT_DISTORT)
	{
		for (int i = 0; i < verts; i++)
		{
			vec3& v = ((vec3*)vert_lump)[i];
		
			v.x += (rand() % (vertDistort*2 + 1)) - vertDistort;
			v.y += (rand() % (vertDistort*2 + 1)) - vertDistort;
			v.z += (rand() % (vertDistort*2 + 1)) - vertDistort;
		}
	}
}


void corrupt_map_lightmap(BSP * map)
{
	byte * lump = map->lumps[LUMP_LIGHTING];
	int colors = (map->header.lump[LUMP_LIGHTING].nLength) / 3;

	for (int i = 0; i < colors; i++)
	{
		COLOR3& c = ((COLOR3*)lump)[i];
		byte r = c.r;
		byte g = c.g;
		byte b = c.b;

		if (lightMode == LIGHT_SHIFTED)
		{
			c.r = 255 - g;
			c.g = b;
			if (g > 128)
				c.b = 255 - g;
			else
				c.b = 128 - g; 
		}
		else if (lightMode == LIGHT_DISCO)
		{
			c.r = rand() % 256;
			c.g = rand() % 256;
			c.b = rand() % 256;
		}
		else if (lightMode == LIGHT_INVERTED)
		{
			c.r = 255 - c.r;
			c.g = 255 - c.g;
			c.b = 255 - c.b;
		}
		else if (lightMode == LIGHT_DARK)
		{
			c.r = 0;
			c.g = 0;
			c.b = 0;
		}
	}
}