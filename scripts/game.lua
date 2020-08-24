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

local entities = {}

function Start ()
	x = 5
	y = 5
	id = 99 -- Player


    for i=0, 1 do
		entities[i] = CreateEntity(x + i * 2, y, id)
		entities[i].test = i
		print("[LUA] Test x & y " .. entities[i].x .. " " .. entities[i].y)
		entities[i].Start(entities[i])
    end
end

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
