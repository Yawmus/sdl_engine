-- Global def
Entity = {}

-- Called automatically at end of entity creation: CreateEntity(x, y, id)
function Entity:Start()
	print("[LUA] Start called")
	self:Test()
end

-- Called automatically once per update loop
function Entity:Update (delta)
	--print("[LUA] Update called")

	-- Random Movement
	x = 0
	y = 0
	dir = math.random(1, 7)
	if dir == 1 then
		x = 1
	elseif dir == 2 then
		x = -1
	elseif dir == 3 then
		y = 1
	elseif dir == 4 then
		y = -1
	end
	self:Move(x, y);
	--self:Move(1, 0);
end

function Entity:Test2()
end

function Entity:Test()
	print("[LUA] x is " .. self.x)
	print("[LUA] y is " .. self.y)
end
