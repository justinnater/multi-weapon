# Description
Allows players to switch through weapons with the same weapon slot instead of loosing their previous weapon with the same weapon slot.

# How to install
Place the multi_weapon.inc inside your include folder. ('pawno\include')
You have to include the file in your script and you're good to go! ('#include <multi_weapon>')
No changes are required in your script for this to work.
Although, if you are using 'GetPlayerWeaponData' you will have to use it differently now.

Before:
```
	new weapons[12][2];
	for (new i = 0; i <= 12; i++)
	{
	    GetPlayerWeaponData(playerid, i, weapons[i][0], weapons[i][1]);
	}
  ```
Now:
```
	new weapons[47][2];
	for (new i = 1; i <= 47; i++)
	{
	    GetPlayerWeaponData(playerid, i, weapons[i][0], weapons[i][1]);
	}
  ```
# How to switch weapon
By default you can switch weapons with the 'ALT' key.
Incase you feel like changing this: 
-> open 'multi_weapon.inc'
-> go to line '9' (#define MULTI_WEAPON_SWITCH_KEY KEY_WALK)
-> change 'KEY_WALK' to any SAMP KEY you like.
(info about keys: https://wiki.sa-mp.com/wiki/Keys)

# Dependencies
y_hooks
