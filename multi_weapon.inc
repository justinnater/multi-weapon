
#if defined _multi_weapon_included
	#endinput
#endif
#define _multi_weapon_included

#include <YSI_Coding\y_hooks>

#define MULTI_WEAPON_SWITCH_KEY KEY_WALK

enum
    e_WepInfo
{
    bool:wepId[47],
    wepAmmo[47]
};

new pWeps[MAX_PLAYERS][e_WepInfo];

stock GetPlayerWeaponDataEx(playerid, weaponid, &mwep, &mammo)
{
	mwep = pWeps[playerid][wepId][weaponid];
	mammo = pWeps[playerid][wepAmmo][weaponid];
}

#if defined _ALS_GetPlayerWeaponData
  #undef GetPlayerWeaponData
#else
#define _ALS_GetPlayerWeaponData
#endif
#define GetPlayerWeaponData GetPlayerWeaponDataEx 

stock ResetPlayerWeaponsEx(playerid)
{
	for( new i=1; i < 47; i++ )
	{
	    pWeps[playerid][wepId][i] = false;
	    pWeps[playerid][wepAmmo][i] = 0;
	}	
	pWeps[playerid][wepId][0] = true;
	ResetPlayerWeapons(playerid);
	return 1;
}

#if defined _ALS_ResetPlayerWeapons
  #undef ResetPlayerWeapons
#else
#define _ALS_ResetPlayerWeapons
#endif
#define ResetPlayerWeapons ResetPlayerWeaponsEx 

stock GivePlayerWeaponEx(playerid, weaponid, ammo, bool:switching=false)
{
	new prevWeapon = GetPlayerWeapon(playerid);
	if(prevWeapon == weaponid)
	{
		pWeps[playerid][wepAmmo][weaponid] += ammo;
		GivePlayerWeapon(playerid, weaponid, ammo);
		return 1;
	}
	pWeps[playerid][wepId][weaponid] = true;
	if(switching==true)
	{
		GivePlayerWeapon(playerid, weaponid, pWeps[playerid][wepAmmo][weaponid]-pWeps[playerid][wepAmmo][prevWeapon]);
		SetPlayerAmmo(playerid, weaponid, pWeps[playerid][wepAmmo][weaponid]);
	}
	else 
	{
		pWeps[playerid][wepAmmo][weaponid] += ammo;
		if(SwitchWeaponAvailable(playerid, weaponid)) 
		{
			GivePlayerWeapon(playerid, weaponid, pWeps[playerid][wepAmmo][weaponid]-pWeps[playerid][wepAmmo][prevWeapon]);
			SetPlayerAmmo(playerid, weaponid, pWeps[playerid][wepAmmo][weaponid]);
		}
		else GivePlayerWeapon(playerid, weaponid, pWeps[playerid][wepAmmo][weaponid]);
	}
	return 1;
}

#if defined _ALS_GivePlayerWeapon 
  #undef GivePlayerWeapon
#else
#define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon GivePlayerWeaponEx 

stock GetWeaponSlot(weaponid)
{
	new slot;
	switch(weaponid){
		case 0, 1: slot = 0;
		case 2 .. 9: slot = 1;
		case 22 .. 24: slot = 2;
		case 25 .. 27: slot = 3;
		case 28, 29, 32: slot = 4;
		case 30, 31: slot = 5;
		case 33, 34: slot = 6;
		case 35 .. 38: slot = 7;
		case 16, 18, 39: slot = 8;
		case 42, 43: slot = 9;
		case 44 .. 46: slot = 11;
		default: slot = -1;
	}
	return slot;
}

stock SwitchWeaponAvailable(playerid, weapon)
{
	new weaponslot = GetWeaponSlot(weapon);
	if(weaponslot == -1) return false;
	for( new i; i < 47; i++ )
	{
		if(GetWeaponSlot(i) == weaponslot && i != weapon)
		{
			if(pWeps[playerid][wepId][i] == true) return true;
		}
	}	
	return false;
}

stock getNextWeapon(playerid, weapon)
{
	new nextwep = -1;
	new weaponslot = GetWeaponSlot(weapon);

	for( new i=weapon; i < 47; i++ )
	{
		if(GetWeaponSlot(i) == weaponslot && i != weapon)
		{
			if(pWeps[playerid][wepId][i] == true)
			{
				nextwep = i;
				break;
			}
		}
	}
	if(nextwep == -1)
	{
		for( new i; i < weapon; i++ )
		{
			if(GetWeaponSlot(i) == weaponslot && i)
			{
				if(pWeps[playerid][wepId][i] == true)
				{
					nextwep = i;
					break;
				}
			}
		}		
	}	
	return nextwep;
}

stock SwitchPlayerWeapon(playerid)
{
	new weapon = GetPlayerWeapon(playerid);

	if(SwitchWeaponAvailable(playerid,weapon))
	{
		new ammo = GetPlayerAmmo(playerid);
		pWeps[playerid][wepAmmo][weapon] = ammo;
		new nextWeapon = getNextWeapon(playerid, weapon);
		GivePlayerWeapon(playerid, nextWeapon, 0, true);
	}
	return 1;
}

hook OnPlayerConnect(playerid)
{
	ResetPlayerWeapons(playerid);
	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys & MULTI_WEAPON_SWITCH_KEY && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		SwitchPlayerWeapon(playerid);
	}
	return 1;
}

hook OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	pWeps[playerid][wepAmmo][weaponid] = GetPlayerAmmo(playerid)-1;
	if(GetPlayerAmmo(playerid) == 1)
	{
		SwitchPlayerWeapon(playerid);
		pWeps[playerid][wepId][weaponid] = false;
		pWeps[playerid][wepAmmo][weaponid] = 0;
	}
    return 1;
}
