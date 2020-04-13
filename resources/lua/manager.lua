local daggerRespawnTime = 5.0

local respawnNumber = 1

local totalTime = 0.0

local lastRespawnTime = 0.0

function manager_Start(obj)

end

function manager_Update(obj, delta)
	totalTime = totalTime + delta
	lastRespawnTime = lastRespawnTime + delta

	if totalTime <= 20.0 then
		respawnNumber = 1
		daggerRespawnTime = 5.0
	elseif totalTime > 20.0 then
		respawnNumber = 2
		daggerRespawnTime = 4.0
	elseif totalTime > 40.0 then
		respawnNumber = 3
		daggerRespawnTime = 3.0
	elseif totalTime > 60.0 then
		respawnNumber = 4
		daggerRespawnTime = 2.0
	else
		daggerRespawnTime = 1.0
	end

	if lastRespawnTime >= daggerRespawnTime then
		for i = 1,respawnNumber do
			CreateDagger(obj)
		end
		lastRespawnTime = 0.0
	end
end

function manager_End(obj)

end
