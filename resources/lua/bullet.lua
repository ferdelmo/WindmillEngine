
print "Bullet.lua"

bullets = {}

max_bullet_time = 7.0
bullet_speed = 50.0


function bullet_Start(obj)
	bullets[obj] = {}
	bullets[obj]["time"] = 0.0
	-- get the direction that the player is looking at at the start
	bullets[obj]["direction"] = GetFirstPersonLookAt(GetPlayer(obj))
end

function bullet_Update(obj, delta)
	-- check life time
	bullets[obj]["time"] = bullets[obj]["time"]+delta

	if bullets[obj]["time"] > max_bullet_time then
		bullets[obj] = nil
		DestroyGameObject(obj)
		return
	end

	-- Move the bullet in its direction

	local bullet_pos = GetPosition(obj)

	local bullet_mov = VecMulEsc(NormalizeVector(bullets[obj]["direction"]), bullet_speed*delta)

	bullet_pos = VecAddVec(bullet_pos, bullet_mov)

	SetPosition(obj, bullet_pos)

end