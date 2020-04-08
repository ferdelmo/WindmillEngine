
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

	local look = {}

	look[0] = deltaMouse[0] * angularSpeedX * delta
	look[1] = deltaMouse[1] * angularSpeedY * delta

	currentDeclination = math.acos(lookAt[2])
	requestedDeclination = currentDeclination-look[1]

	if requestedDeclination < zenitMin then
		look[1] = currentDeclination - zenitMin
	elseif requestedDeclination > zenitMax then
		look[1] = currentDeclination - zenitMax
	end


	local defaultUp = {}
	defaultUp[0] = 0.0
	defaultUp[1] = 0.0
	defaultUp[2] = 1.0
	local right = CrossProduct(lookAt, defaultUp)
	local up = CrossProduct(right, lookAt)

	lookAt = RotateVector(lookAt, right, look[1])
	lookAt = RotateVector(lookAt, up, look[0])

	SetFirstPersonLookAt(obj, lookAt);


	
	-- basic movement
	local pos = GetPosition(obj)

	local mov = {}
	mov[0] = 0.0
	mov[1] = 0.0
	mov[2] = 0.0

	local update = 0

	if IsKeyPressed('w') then
		mov[1] = mov[1] + 1
	end

	if IsKeyPressed('s') then
		mov[1] = mov[1] - 1
	end

	if IsKeyPressed('a') then
		mov[0] = mov[0] + 1
	end

	if IsKeyPressed('d') then
		mov[0] = mov[0] - 1
	end

	local frontMov = lookAt
	frontMov[2] = 0

	local rightMov = right
	rightMov[2] = 0

	if (not (mov[0] == 0.0)) or (not (mov[1] == 0.0)) then
		mov = VecMulEsc(NormalizeVector(mov),speed*delta)
	end
	pos = VecAddVec(pos, VecMulEsc(frontMov, mov[1]))
	pos = VecAddVec(pos, VecMulEsc(rightMov, mov[0]))

	SetPosition(obj, pos[0], pos[1], pos[2])
end