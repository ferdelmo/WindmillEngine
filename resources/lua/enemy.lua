
print "Enemy.lua"

local speed = 10

function enemy_Update(obj, delta)
	local pos = GetPosition(obj)

	-- basic movement
	if IsKeyPressed('w') then
		pos[1] = pos[1] + speed*delta
	end

	if IsKeyPressed('s') then
		pos[1] = pos[1] - speed*delta
	end

	if IsKeyPressed('a') then
		pos[0] = pos[0] - speed*delta
	end

	if IsKeyPressed('d') then
		pos[0] = pos[0] + speed*delta
	end

	
	SetPosition(obj, pos[0], pos[1], pos[2])
end