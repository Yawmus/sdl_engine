ids = {}

ids[100] = {
	name = "Unknown",
	blocking = false,
	asset = "Unknown",
	region = {
		x = 0,
		y = 0,
		width = 32,
		height = 32
	}
}
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

ids[98] = {
	name = "Monster",
	blocking = true,
	asset = "Monster",
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
	name = "Flat",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 256,
		y = 0,
		width = 32,
		height = 32
	}
}

ids[2] = {
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

assets["Unknown"] = "../assets/images/unknown.png"
assets["Monster"] = "../assets/images/test3.png"
assets["Player"] = "../assets/images/test2.png"
assets["Terrain_tilemap"] = "../assets/tilemaps/terrain.png"
assets["Obj_tilemap"] = "../assets/tilemaps/objects.png"

function GetEntityType(id)
	return ids[id]
end
