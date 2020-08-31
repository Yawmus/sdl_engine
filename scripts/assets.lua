ids = {}

ids[100] = {
	name = "unknown",
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
	name = "player",
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
	name = "monster",
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
	name = "grass",
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
	name = "grass.flat.top",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 32,
		y = 0,
		width = 32,
		height = 32
	}
}

ids[2] = {
	name = "grass.flat.topright",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 64,
		y = 0,
		width = 32,
		height = 32
	}
}

ids[3] = {
	name = "grass.flat.right",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 96,
		y = 0,
		width = 32,
		height = 32
	}
}

ids[4] = {
	name = "grass.flat.bottomright",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 128,
		y = 0,
		width = 32,
		height = 32
	}
}
ids[5] = {
	name = "grass.flat.bottom",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 160,
		y = 0,
		width = 32,
		height = 32
	}
}
ids[6] = {
	name = "grass.flat.bottomleft",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 192,
		y = 0,
		width = 32,
		height = 32
	}
}
ids[7] = {
	name = "grass.flat.left",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 224,
		y = 0,
		width = 32,
		height = 32
	}
}
ids[8] = {
	name = "grass.flat.topleft",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 0,
		y = 64,
		width = 32,
		height = 32
	}
}

ids[9] = {
	name = "flat",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 256,
		y = 0,
		width = 32,
		height = 32
	}
}
ids[10] = {
	name = "water.flat.top",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 288,
		y = 0,
		width = 32,
		height = 32
	}
}
ids[11] = {
	name = "water.flat.topright",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 0,
		y = 32,
		width = 32,
		height = 32
	}
}
ids[12] = {
	name = "water.flat.right",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 32,
		y = 32,
		width = 32,
		height = 32
	}
}
ids[13] = {
	name = "water.flat.bottomright",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 64,
		y = 32,
		width = 32,
		height = 32
	}
}
ids[14] = {
	name = "water.flat.bottom",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 96,
		y = 32,
		width = 32,
		height = 32
	}
}
ids[15] = {
	name = "water.flat.bottomleft",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 128,
		y = 32,
		width = 32,
		height = 32
	}
}
ids[16] = {
	name = "water.flat.left",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 64,
		y = 64,
		width = 32,
		height = 32
	}
}
ids[17] = {
	name = "water.flat.topleft",
	blocking = false,
	asset = "Terrain_tilemap",
	region = {
		x = 160,
		y = 32,
		width = 32,
		height = 32
	}
}

ids[18] = {
	name = "water",
	blocking = true,
	asset = "Terrain_tilemap",
	region = {
		x = 32,
		y = 64,
		width = 32,
		height = 32
	}
}

ids[19] = {
	name = "water.flat.topright2",
	blocking = true,
	asset = "Terrain_tilemap",
	region = {
		x = 192,
		y = 32,
		width = 32,
		height = 32
	}
}

ids[20] = {
	name = "water.flat.bottomright2",
	blocking = true,
	asset = "Terrain_tilemap",
	region = {
		x = 224,
		y = 32,
		width = 32,
		height = 32
	}
}
ids[21] = {
	name = "water.flat.bottomleft2",
	blocking = true,
	asset = "Terrain_tilemap",
	region = {
		x = 256,
		y = 32,
		width = 32,
		height = 32
	}
}
ids[22] = {
	name = "water.flat.topleft2",
	blocking = true,
	asset = "Terrain_tilemap",
	region = {
		x = 288,
		y = 32,
		width = 32,
		height = 32
	}
}
--ids[50] = {
	--name = "Tree",
	--blocking = true,
	--asset = "Obj_tilemap",
	--region = {
		--x = 0,
		--y = 0,
		--width = 32,
		--height = 32
	--}
--}

assets = {}

assets["Unknown"] = "../assets/images/unknown.png"
assets["Monster"] = "../assets/images/test3.png"
assets["Player"] = "../assets/images/test2.png"
assets["Terrain_tilemap"] = "../assets/tilemaps/terrain.png"
assets["Obj_tilemap"] = "../assets/tilemaps/objects.png"

function tablelength(T)
	local count = 0
	for _ in pairs(T) do count = count + 1 end
	return count
end

function GetAllIds()
	local keyset={}
	local n=1
	keyset[1] = tablelength(ids);

	for k,v in pairs(ids) do
		n=n+1
		keyset[n]=k
	end
	return keyset;
end

function GetEntityType(id)
	return ids[id]
end
