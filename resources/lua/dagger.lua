
--print "Dagger.lua"

linearSpeed = 1.0

angularSpeed = 1.0
-- center of the map, daggers will be around it
mapCenter = {0.0, 0.0, 0.0}

-- min distance from center of the map to dagger
distCenter = 30.0

-- respawn rate
respawnRate = 4.0

-- time since last skull was respanwed
local lastRespawn = 0.0

function dagger_Start(obj)
	
end

function dagger_Update(obj, delta)
	local dirVector = NormalizeVector(VecMinusVec(mapCenter, GetPosition(obj)))
	dirVector = {dirVector[1], dirVector[2], 0.0}
	if Magnitude(VecMinusVec(mapCenter, GetPosition(obj))) > distCenter then
		local aux = VecAddVec(GetPosition(obj), VecMulEsc(dirVector, delta * speed))
		SetPosition(obj, aux)
	end

	local rot = {0.0, 0.0, angularSpeed * delta} 
	rot = VecAddVec(GetRotation(obj), rot)
	SetRotation(obj, rot)

	lastRespawn = lastRespawn + delta
	if lastRespawn >= respawnRate then
		CreateSkull(obj)
		lastRespawn = 0.0
	end
end

function dagger_End(obj)

end


function dagger_Die(obj)
	DestroyGameObject(obj)
end
