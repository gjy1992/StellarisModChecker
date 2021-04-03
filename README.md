# StellarisModChecker
To subscribe, update and calculate the hash of Stellaris Mods.

This program currently is used to subscribe, download/update the Stellaris mods in current playset. Then the program calculates the hash of all the steam mods in the playset and generate a hash.json file. This file is used to compare the content of mods between multiplayer, to imply which one is different and prevent a Stellaris multiplayer game.

To start a Stellaris multiplayer game, one of the person should decide all the mods to use, and generate a new playset in Stellaris launcher, then set it as active one. Then, copy the launcher-v2.sqlite file (usually in MyDocuments/Paradox Interactive/Stellaris folder) to other people. Then close the launcher and run this program to download and calculate the hash. If any person cannot see the host in multiplayer game page, or the hash in Stellaris is different from the host's one, they should compare the generated hash.json file to find which mod is different. Then they need to delete the whole mod folder (such as steamapps/workshop/content/281990/xxxxxxxxx) and trigger a integrity verification in Steam menu.

# To do
- Generate a string to represent the mods in current playset, and generate a playset from the given string.