
print "Enemy.lua"

-- All enemies in the scene
enemies = {}

-- Area of effect of separation from other enemies
separationArea = 2.0

playerPos = {0.0, 0.0, 0.0}

local speed = 10


function enemy_Start(obj)
	enemies[obj] = {}
	enemies[obj]["velocity"] = {0.0, 0.0, 0.0}
	enemies[obj]["health"] = 1.0
end

function enemy_Update(obj, delta)
	
	playerPos = GetPosition(GetPlayer(obj))
	
	steering(obj, delta)
	
	local aux = VecAddVec(GetPosition(obj), VecMulEsc(enemies[obj]["velocity"], delta))
	SetPosition(obj, aux)
	SetRotation(obj, aux)
end

function steering(obj, delta)
	local currentPos = GetPosition(obj)
	local currentSpeed = enemies[obj]["velocity"]
	-- player to skull
	local distVector = NormalizeVector(VecMinusVec(playerPos, currentPos))
	local desiredVelocity = VecMulEsc(distVector, speed)

	-- the others skulls positions
	local otherPos = {0.0, 0.0, 0.0}

	local separationDir = {0.0, 0.0, 0.0}

	for key in pairs(enemies) do
		otherPos = GetPosition(key)
		aux = VecMinusVec(currentPos, otherPos)
		local distance = Magnitude(aux) 
		if distance < separationArea then
			separationDir = VecAddVec(separationDir, aux)
		end
	end

	currentSpeed = VecAddVec(currentSpeed, separationDir)

	local steering = VecMinusVec(desiredVelocity, currentSpeed)
	enemies[obj]["velocity"] = VecAddVec(currentSpeed, 
						VecMulEsc(steering, delta))
end


function printVector(v)
	print("{" .. v[1] .. ", " .. v[2] .. ", " .. v[3] .."}")
end
