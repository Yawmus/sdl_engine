ids = {}

ids[99] = {
	name = "Player",
	blocking = true,
	asset = "Player",
	region = {
		x = 0,
		y = 0,
		width = 256,
		height = 256
	}
}

ids[0] = {
	name = "Grass",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 0,
		y = 0,
		width = 32,
		height = 32
	}
}

ids[1] = {
	name = "Water",
	blocking = true,
	asset = "Terrain_tilemap",
	region = {
		x = 32,
		y = 64,
		width = 32,
		height = 32
	}
}

ids[2] = {
	name = "Flowers",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 128,
		y = 64,
		width = 32,
		height = 32
	}
}

ids[50] = {
	name = "Tree",
	blocking = true,
	asset = "Obj_tilemap",
	region = {
		x = 0,
		y = 0,
		width = 32,
		height = 32
	}
}

assets = {}

assets["Player"] = "../assets/images/test2.png"
assets["Terrain_tilemap"] = "../assets/tilemaps/terrain.png"
assets["Obj_tilemap"] = "../assets/tilemaps/objects.png"

function GetEntityType(id)
	return ids[id]
end
