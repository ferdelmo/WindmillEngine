
print "Enemy.lua"

local speed = 10

function enemy_Update(obj, delta)
	local pos = GetPosition(obj)
	
	SetPosition(obj, pos[0], pos[1], pos[2])
end