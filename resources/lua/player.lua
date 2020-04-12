
-- print "Player.lua"

playerPos = {0.0, 0.0, 0.0}
speed = 25

angularSpeedX = 1000.0
angularSpeedY = 500.0

zenitMin = math.rad(10)
zenitMax = math.rad(180-math.deg(zenitMin))

-- Needs to be the same as in GLFW
shoot_button = '\0'
cadence = 0.1
timeSinceLastShoot = 0.0

lookAt = {0.0, 0.0, 0.0}

-- Jump
gravity = 9.8 * 5.0
velocityUpInitial = 25
velocityUp = 0
jump_button = ' '
isJumping = false

shotgun = false

function player_Start(obj)
	-- body
	RegisterMouseButtonRightPressedFunction(obj,"player_shotgun")
end

function player_Update(obj, delta)
	
	-- lookAt
	lookAt = GetFirstPersonLookAt(obj)

	local deltaMouse = GetMousePosition()

	local look = { -deltaMouse[0] * angularSpeedX * delta, 
			-deltaMouse[1] * angularSpeedY * delta}

	local currentDeclination = math.acos(lookAt[3])
	local requestedDeclination = currentDeclination-look[2]

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

	if IsKeyPressed('w') then
		mov[2] = mov[2] + 1
	end

	if IsKeyPressed('s') then
		mov[2] = mov[2] - 1
	end

	if IsKeyPressed('a') then
		mov[1] = mov[1] - 1
	end

	if IsKeyPressed('d') then
		mov[1] = mov[1] + 1
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

	-- shoot 
	if timeSinceLastShoot <= 0 then
		if IsKeyPressed(shoot_button) then
			shotgun = false
			CreateBullet(obj)
			timeSinceLastShoot = cadence
		end
	else 
		timeSinceLastShoot = timeSinceLastShoot - delta
	end

	--Jump
	if IsKeyPressed(jump_button) and (not isJumping) then
		velocityUp = velocityUpInitial
		isJumping = true
	end

	if pos[3] >= 0 and isJumping then
		velocityUp = velocityUp - gravity * delta
		pos[3] = pos[3] + velocityUp * delta
	else
		pos[3] = 0
		isJumping = false
	end

	SetPosition(obj, pos)


end

function player_shotgun(obj)
	if timeSinceLastShoot > 0 then
		return
	end
	timeSinceLastShoot = 0.5
	shotgun = true
	for i=1,5 do
		CreateBullet(obj)
	end

end