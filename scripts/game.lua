-- Entity metatable
--
-- entity.lua start/update automatically called on creation
--
-- + Create => Entity.Create(x, y, id)
-- + Update
-- + Move
--
-- Supports new user values (not provided by API):
-- 		entity.zzz = 10

lunajson = require 'lunajson'
package.path = package.path .. ";?.lua" -- includes all /scripts
local open = io.open

local entities = {}
local width
local height

-- Entry-point from C++
function Main (file)
	if file ~= nil and file ~= '' then -- Starting from existing state
		local file = open(file, "rb")
		local content = file:read "*a"
		file:close()

		local jsonraw = content
		local jsonparse = lunajson.decode( jsonraw )
		height = jsonparse["height"]
		width = jsonparse["width"]

		local i = 1
		while jsonparse["entities"][i] do
			raw_e = jsonparse["lua_entities"][i]
			entity = CreateEntity(raw_e["x"], raw_e["y"], raw_e["id"])
			i = i + 1
		end
	else -- Fresh start
		x = 5
		y = 5
		id = 98 -- Monster
		entityIdx = 0

		for i=0, 6 do
			entity = CreateEntity(x + i * 2, y, id)
			if entity ~= nil then
				entities[entityIdx] = entity
				entities[entityIdx].test = i
				entityIdx = entityIdx + 1
			end
			--print("[LUA] Test x & y " .. entities[i].x .. " " .. entities[i].y)
			--entities[i].Start()
		end
	end
end

-- TODO: Generalize bindings
function Update()
end

function CreateEntity (x, y, id)
	print("[LUA] Creating entity at x:" .. x .. " y:" .. y .. " with id:" .. id);
	return Entity.Create(x, y, id);
end

-- Helper func to get the size of the table
function tablelength(T)
	local count = 0
	for _ in pairs(T) do count = count + 1 end
	return count
end
