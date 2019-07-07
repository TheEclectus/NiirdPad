--[[
	Dialogue Functions
	==================
	- Each validation function is a the name of a TU function
	- Each takes a table that is a list of string arguments in sequential order (not including the function name)
	- Returns a boolean for validity, and a list of connection keys (or nil for a single connection/non-index-modifying functions/failure), and an error string (nil on success)
	- Multiple index-modifying functions in the same Option 
	- On generation, keys will be passed back in the exact same order as they were output in
--]]

-- TOMORROW: Ask meanie about what functions can go where
-- TODO: Maybe just make a list of index-modifying functions? Maybe as another table?

-- Maybe disconnects should only happen when a function is changed?
-- Otherwise maintain connections based on position

-- function CurrentCharacter() -> string			character that the script is running under
-- function ReferenceExists(ref) -> bool			checks if a reference exists yet
-- function CharacterExists(char) -> bool			checks if a character exists
-- function CharacterHasFile(char, file) -> bool	checks if current character has a file named 'file'
-- function ItemExists(item) -> bool				checks if an item exists
-- function CardExists(card) -> bool				checks if a card exists
-- function EnvironmentExists(env) -> bool			checks if an environment exists
-- function LocationExists(loc) -> bool				checks if a location exists

function IsCuratedListType(type)
	if(type == "prioritized" or type == "random" or type == "weighted") then
		return true
	else
		return false
end

function IsMoneyType(type)
	if(type == "krats" or type == "adats") then
		return true
	else
		return false
end

-- Shamelessly copy-pasted from http://lua-users.org/wiki/SplitJoin
-- I've written so many fucking string split functions I can't be bothered to do it anymore
function SplitString(str, pat)
   local t = {}  -- NOTE: use {n = 0} in Lua-5.0
   local fpat = "(.-)" .. pat
   local last_end = 1
   local s, e, cap = str:find(fpat, 1)
   while s do
      if s ~= 1 or cap ~= "" then
         table.insert(t,cap)
      end
      last_end = e+1
      s, e, cap = str:find(fpat, last_end)
   end
   if last_end <= #str then
      cap = str:sub(last_end)
      table.insert(t, cap)
   end
   return t
end

local Funcs = {}

Funcs.Dialogue = {}

-- Shorthand, D for Dialogue function
local D = Funcs.Dialogue
function MakeFail(msg)
	return false, nil, msg
end



-- pic (image name)
function D.pic(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local image_path = args[1]

	return true, nil, nil
end

-- remove_pic
function D.remove_pic(args)
	if #args ~= 0 then return MakeFail("Expected 0 argument, received " .. tostring(#args) .. ".") end
	
	return true, nil, nil
end

-- hide_character
-- hide_sprite
function D.hide_character(args)
	if #args ~= 0 then return MakeFail("Expected 0 argument, received " .. tostring(#args) .. ".") end
	
	return true, nil, nil
end
D.hide_sprite = D.hide_character

-- character_leave
function D.character_leave(args)
	if #args ~= 0 then return MakeFail("Expected 0 argument, received " .. tostring(#args) .. ".") end
	
	return true, nil, nil
end

-- character_return (sprite)
function D.character_return(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local sprite_name = args[1]

	return true, nil, nil
end

-- change_character (character)
function D.change_character(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local character = args[1]

	return true, nil, nil
end

-- change_environment (env)
function D.change_environment(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local environment = args[1]
	if(not EnvironmentExists(environment)) then return MakeFail("Environment defined in argument 1 ['" .. environment .. "'] doesn't exist.") end

	return true, nil, nil
end

-- change_sprite (sprite)
function D.change_sprite(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local sprite = args[1]

	return true, nil, nil
end

-- change_default_sprite (char, new sprite)
function D.change_default_sprite(args)
	if #args ~= 2 then return MakeFail("Expected 2 argument, received " .. tostring(#args) .. ".") end
	
	local character = args[1]
	local new_sprite = args[2]

	return true, nil, nil
end

-- change_default_env_background (env, bkg img filename)
function D.change_default_env_background(args)
	if #args ~= 2 then return MakeFail("Expected 2 argument, received " .. tostring(#args) .. ".") end
	
	local environment = args[1]
	if(not EnvironmentExists(environment)) then return MakeFail("Environment defined in argument 1 ['" .. environment .. "'] doesn't exist.") end
	local bkg_image_filename = args[2]

	return true, nil, nil
end

-- add_default_env_layer (bkg img filename)
function D.change_default_env_background(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local bkg_image_filename = args[1]

	return true, nil, nil
end

-- save_index (saved index)
function D.save_index(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local saved_index = args[1]

	return true, nil, nil
end

-- save_index_by_character (character, saved index)
function D.save_index_by_character(args)
	if #args ~= 2 then return MakeFail("Expected 2 argument, received " .. tostring(#args) .. ".") end
	
	local character = args[1]
	local saved_index = args[2]

	return true, nil, nil
end

-- change_default_diag_file (character, filename)
function D.change_default_diag_file(args)
	if #args ~= 2 then return MakeFail("Expected 2 argument, received " .. tostring(#args) .. ".") end
	
	local character = args[1]
	local filename = args[2]

	return true, nil, nil
end

-- add_flag (flag name)
function D.add_flag(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local flag_name = args[1]

	return true, nil, nil
end
D.set_flag = D.add_flag

-- remove_flag (flag name)
function D.remove_flag(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local flag_name = args[1]

	return true, nil, nil
end

-- counter (counter name, +/-, number)
function D.counter(args)
	if #args ~= 3 then return MakeFail("Expected 3 argument, received " .. tostring(#args) .. ".") end
	
	local counter_name = args[1]
	local plus_minus = args[2]
	local number = args[3]

	if(plus_minus ~= "+" and plus_minus ~= "-") then
		return MakeFail("Argument 2 must be '+' or '-'.")
	end

	return true, nil, nil
end

-- set_counter (counter name, set number)
function D.set_counter(args)
	if #args ~= 2 then return MakeFail("Expected 2 argument, received " .. tostring(#args) .. ".") end
	
	local counter_name = args[1]
	local set_number = args[2]

	return true, nil, nil
end

-- remove_counter (counter name)
function D.remove_counter(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local counter_name = args[1]

	return true, nil, nil
end

-- curated_list (type) [IM, no connection points]
function D.curated_list(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local type = args[1]
	if(not IsCuratedListType(type)) then return MakeFail("Argument 1 must be a valid curated_list type.") end

	return true, nil, nil
end

-- change_diag_file (filename, ptr) [IM, no connection points]
function D.change_diag_file(args)
	if #args ~= 2 then return MakeFail("Expected 2 argument, received " .. tostring(#args) .. ".") end
	
	local filename = args[1]
	if(not CharacterHasFile(filename)) then return MakeFail("Argument 1 must be the name of one of the character's dialogue files.") end

	local ptr = args[2]

	return true, nil, nil
end

-- change_to_default_diag (ptr) [IM, no connection points]
function D.change_to_default_diag(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local ptr = args[1]

	return true, nil, nil
end

-- check_flag (flag name, [doesn't have flag], [has flag]) [IM]
function D.check_flag(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end
	
	local flag_name = args[1]

	return true, {"Doesn't have flag '" .. flag_name .. "'", "Has Flag '" .. flag_name .. "'"}, nil
end

-- check_counter (counter name, limit, [under], [over]) [IM]
function D.check_counter(args)
	if #args ~= 2 then return MakeFail("Expected 2 arguments, received " .. tostring(#args) .. ".") end

	local counter_name = args[1]
	local target = args[2]

	return true, {"Counter < " .. tostring(target), "Counter >= " .. tostring(target)}, nil
end

-- check_discovery (place name, [not discovered], [discovered]) [IM]
function D.check_discovery(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local place_name = args[1]

	return true, {tostring(place_name) .. " not discovered", tostring(place_name) .. " discovered"}, nil
end

-- This is going to need a special renderer
-- check_stat (attribute, val1-val2-...-valN, [fail]-[pass1]-[pass2]-...-[passN]) [IM]
function D.check_stat(args)
	if #args ~= 2 then return MakeFail("Expected 2 arguments, received " .. tostring(#args) .. ".") end

	local attribute = args[1]
	local values = SplitString(args[2], "-")

	local keys = {"Check Failed"}
	for i=1,#values do
		keys[#keys + 1] = attribute .. " >= " .. values[i]
	end

	return true, keys, nil
end

-- Meanis why
-- sex_gate ([male], [female]) [IM]
-- sex_branch ([male], [female]) [IM]
-- gender_gate ([male], [female]) [IM]
function D.sex_gate(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, {tostring(place_name) .. " not discovered", tostring(place_name) .. " discovered"}, nil
end
D.sex_branch = D.sex_gate
D.gender_gate = D.sex_gate

-- rape_filter ([on], [off]) [IM]
function D.sex_gate(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, {"Rape filter on", "Rape filter off"}, nil
end

-- feral_filter ([on], [off]) [IM]
function D.feral_filter(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, {"Feral filter on", "Feral filter off"}, nil
end

-- gore_filter ([on], [off]) [IM]
function D.gore_filter(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, {"Gore filter on", "Gore filter off"}, nil
end

-- chance (win %, [lost], [win]) [IM]
function D.chance(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local win_percent = tonumber(args[1])
	if(win_percent == nil or win_percent < 1 or win_percent > 100) then return MakeFail("Argument 1 must be a number in range [1-100].") end

	return true, {"Lose", "Win"}, nil
end

-- Is also gonna need a special renderer
-- Also, fuckem, they get 10 pointers to work with
-- random_ptr ([ptr1]-[ptr2]-...[ptrN]) [IM]
function D.random_ptr(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, nil
end

-- day_night_gate ([day], [night]) [IM]
function D.day_night_gate(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, {"Day", "Night"}, nil
end

-- morn_day_eve_night_gate ([morning], [day], [evening], [night]) [IM]
function D.morn_day_eve_night_gate(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, {"Morning", "Day", "Evening", "Night"}, nil
end

-- give_money ("krats"|"adats", amount)
function D.give_money(args)
	if #args ~= 2 then return MakeFail("Expected 2 arguments, received " .. tostring(#args) .. ".") end

	local money_type = args[1]
	if(not IsMoneyType(money_type)) then return MakeFail("Argument 1 must be either 'krats' or 'adats'.") end

	local amount = tonumber(args[2])
	if(amount == nil or amount < 0) then return MakeFail("Argument 2 must be a positive number.") end

	return true, nil, nil
end

-- take_money ("krats"|"adats", amount)
function D.take_money(args)
	if #args ~= 2 then return MakeFail("Expected 2 arguments, received " .. tostring(#args) .. ".") end

	local money_type = args[1]
	if(not IsMoneyType(money_type)) then return MakeFail("Argument 1 must be either 'krats' or 'adats'.") end

	local amount = tonumber(args[2])
	if(amount == nil or amount < 0) then return MakeFail("Argument 2 must be a positive number.") end

	return true, nil, nil
end

-- give_item (item, ?number)
function D.give_item(args)
	if #args == 0 or #args > 2 then return MakeFail("Expected 1 or 2 arguments, received " .. tostring(#args) .. ".") end

	local item = args[1]
	if(not ItemExists(item)) then return MakeFail("Argument 1: item '" .. item .." doesn't exist.") end

	local amount = tonumber(args[2]) or 1
	if(amount < 1) then return MakeFail("Argument 2 cannot be less than 1.")

	return true, nil, nil
end

-- remove_item (item, ?number)
function D.remove_item(args)
	if #args == 0 or #args > 2 then return MakeFail("Expected 1 or 2 arguments, received " .. tostring(#args) .. ".") end

	local item = args[1]
	if(not ItemExists(item)) then return MakeFail("Argument 1: item '" .. item .." doesn't exist.") end

	local amount = tonumber(args[2]) or 1
	if(amount < 1) then return MakeFail("Argument 2 cannot be less than 1.")

	return true, nil, nil
end

-- give_card (card name)
function D.give_card(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local card = args[1]
	if(not CardExists(card)) then return MakeFail("Argument 1: card '" .. card .." doesn't exist.") end

	return true, nil, nil
end

-- remove_card (card name)
function D.remove_card(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local card = args[1]
	if(not CardExists(card)) then return MakeFail("Argument 1: card '" .. card .." doesn't exist.") end

	return true, nil, nil
end

-- auto_continue (ptr) [IM, no connection points]
function D.auto_continue(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local ptr = args[1]

	return true, nil, nil
end

-- start_encounter (environment, character, ptr) [IM, no connection points]
function D.start_encounter(args)
	if #args ~= 3 then return MakeFail("Expected 3 arguments, received " .. tostring(#args) .. ".") end

	local environment = args[1]
	if(not EnvironmentExists(environment)) then return MakeFail("Environment defined in argument 1 ['" .. environment .. "'] doesn't exist.") end

	local character = args[2]
	if(not CharacterExists(environment)) then return MakeFail("Character defined in argument 2 ['" .. character .. "'] doesn't exist.") end

	local ptr = args[3]

	return true, nil, nil
end

-- set_map_location (location)
function D.set_map_location(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local location = args[1]
	if(not EnvironmentExists(environment)) then return MakeFail("Environment defined in argument 1 ['" .. location .. "'] doesn't exist.") end

	return true, nil, nil
end

-- start_combat ([lose], [win])
function D.start_combat(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, {"Combat Lost", "Combat Won"}, nil
end

-- combat_damage ("character"|"player", hitpoints)
function D.combat_damage(args)
	if #args ~= 2 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	local target = args[1]
	if(target ~= "character" and target ~= "player") then return MakeFail("Argument 1 must be one of ['character', 'player'].") end

	local hitpoints = tonumber(args[2])
	if(hitpoints == nil) then return MakeFail("Argument 2 must be a number.")

	return true, nil, nil
end

-- combat_heal ("character"|"player", hitpoints)
function D.combat_heal(args)
	if #args ~= 2 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	local target = args[1]
	if(target ~= "character" and target ~= "player") then return MakeFail("Argument 1 must be one of ['character', 'player'].") end

	local hitpoints = tonumber(args[2])
	if(hitpoints == nil) then return MakeFail("Argument 2 must be a number.")

	return true, nil, nil
end

-- //showif.index.ate_narbs
-- player_death
-- [laughs in narb]
function D.player_death(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, nil, nil
end

-- advance_time (time units)
function D.advance_time(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local time_units = tonumber(args[1]) or 1
	if(hitpoints < 1) then return MakeFail("Argument 2 cannot be less than 1.")

	return true, nil, nil
end

-- advance_time (time units or textual time)
function D.advance_time_to(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local time_units = tonumber(args[1])
	if(time_units == nil) then
		time_units = args[1]
		if(time_units ~= "morning" and time_units ~= "noon" and time_units ~= "evening" and time_units ~= "night") then
			return MakeFail("Argument 1 must be a number or one of ['morning', 'noon', 'evening', 'night'].") end
		end
	elseif time_units < 1 then
		return MakeFail("Argument 1 must be greater than 0.") end
	end

	return true, nil, nil
end

-- start_music (filename, volume, fadeout secs, fadein secs)
function D.start_music(args)
	if #args ~= 4 then return MakeFail("Expected 4 arguments, received " .. tostring(#args) .. ".") end

	local filename = args[1]
	local volume = tonumber(args[2]) or 0
	local fadeout_seconds = tonumber(args[3]) or 0
	local fadein_seconds = tonumber(args[4]) or 0

	return true, nil, nil
end

-- stop_music (fadeout secs)
function D.stop_music(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local fadeout_seconds = tonumber(args[1]) or 0

	return true, nil, nil
end

-- play_contsound (filename, 1|2, fadein, fadeout)
function D.play_contsound(args)
	if #args ~= 4 then return MakeFail("Expected 4 arguments, received " .. tostring(#args) .. ".") end

	local filename = args[1]
	local channel = tonumber(args[2])
	if(channel == nil and channel ~= 1 and channel ~= 2) then return MakeFail("Argument 2 must be one of [1, 2].") end

	local fadein_seconds = tonumber(args[3]) or 0
	local fadeout_seconds = tonumber(args[4]) or 0

	return true, nil, nil
end

-- stop_contsound (1|2, fadeout)
function D.stop_contsound(args)
	if #args ~= 4 then return MakeFail("Expected 4 arguments, received " .. tostring(#args) .. ".") end

	local channel = tonumber(args[1])
	if(channel == nil and channel ~= 1 and channel ~= 2) then return MakeFail("Argument 2 must be one of [1, 2].") end

	local fadeout_seconds = tonumber(args[2]) or 0

	return true, nil, nil
end

-- start_shop (return ptr)
function D.start_shop(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local return_ptr = args[1]

	return true, nil, nil
end

-- revert_last_shop_item
function D.revert_last_shop_item(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, nil, nil
end

-- disable_shop
function D.disable_shop(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, nil, nil
end

-- enable_shop
function D.enable_shop(args)
	if #args ~= 0 then return MakeFail("Expected 0 arguments, received " .. tostring(#args) .. ".") end

	return true, nil, nil
end

-- discover_location (location)
function D.discover_location(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local location = args[1]
	if(not LocationExists(location)) then return MakeFail("Location defined in argument 1 ['" .. location .. "'] doesn't exist.") end

	return true, nil, nil
end

-- remove_location (location)
function D.remove_location(args)
	if #args ~= 1 then return MakeFail("Expected 1 argument, received " .. tostring(#args) .. ".") end

	local location = args[1]
	if(not LocationExists(location)) then return MakeFail("Location defined in argument 1 ['" .. location .. "'] doesn't exist.") end

	return true, nil, nil
end