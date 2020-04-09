
print "Player.lua"

speed = 25

angularSpeedX = 1000.0
angularSpeedY = 500.0

zenitMin = math.rad(10)
zenitMax = math.rad(180-math.deg(zenitMin))


function player_Update(obj, delta)
	
	-- lookAt
	local lookAt = GetFirstPersonLookAt(obj)

	local deltaMouse = GetMousePosition()

	local look = { deltaMouse[0] * angularSpeedX * delta, 
			deltaMouse[1] * angularSpeedY * delta}

	currentDeclination = math.acos(lookAt[3])
	requestedDeclination = currentDeclination-look[2]

	if requestedDeclination < zenitMin then
		look[2] = currentDeclination - zenitMin
	elseif requestedDeclination > zenitMax then
		look[2] = currentDeclination - zenitMax
	end


	local defaultUp = { 0.0, 0.0, 1.0}
	local right = CrossProduct(lookAt, defaultUp)
	local up = CrossProduct(right, lookAt)

	lookAt = RotateVector(lookAt, right, look[2])
	lookAt = RotateVector(lookAt, up, look[1])

	SetFirstPersonLookAt(obj, lookAt);


	
	-- basic movement
	local pos = GetPosition(obj)

	local mov = { 0.0, 0.0, 0.0 }

	local update = 0

	if IsKeyPressed('w') then
		mov[2] = mov[2] + 1
	end

	if IsKeyPressed('s') then
		mov[2] = mov[2] - 1
	end

	if IsKeyPressed('a') then
		mov[1] = mov[1] + 1
	end

	if IsKeyPressed('d') then
		mov[1] = mov[1] - 1
	end

	local frontMov = lookAt
	frontMov[3] = 0

	local rightMov = right
	rightMov[3] = 0

	if (not (mov[1] == 0.0)) or (not (mov[2] == 0.0)) then
		mov = VecMulEsc(NormalizeVector(mov),speed*delta)
	end
	pos = VecAddVec(pos, VecMulEsc(frontMov, mov[2]))
	pos = VecAddVec(pos, VecMulEsc(rightMov, mov[1]))

	SetPosition(obj, pos)
end