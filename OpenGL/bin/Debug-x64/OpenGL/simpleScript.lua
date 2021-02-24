
--Player = {}
--Player["name"] = "Bogdan Strohonov"
--Player["health"] = 128
--Player["armor"] = 12
--Player["power"] = 99

--player = {
  --  name = "Bogdan Strohonov",
  --  health = 122,
  --  armor = 12,
  --  power = 832
--}

players = {}
players[0] = {name = "Bodgan", health = 92, armor = 23, power = 11}
players[1] = {name = "Katerina", health = 64, armor = 14, power = 91}


function add(x, y)
    print("[LUA] add("..x..", "..y..") called\n")
    return x + y
end

function getPlayer(index)
    return players[index]
end

function doSomething(a, b)
  print("[LUA] doSomething ("..a..","..b..") called\n")

  c = hostFunction(a*2, b)

  return c 
end