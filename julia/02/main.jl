function readip(fn)
    open(fn) do f
        readlines(f)
    end
end

mutable struct RGB
    r
    g
    b
end

function parsecubes(cubes)
    rgb = RGB(0,0,0)
    for s in eachsplit(cubes,",")
        #starts with space so first element empty string
        #use second element of split
        n = parse(Int64,split(s," ")[2])
        colour = split(s," ")[3]
        if colour == "red"
            rgb.r = n
        elseif colour == "green"
            rgb.g = n
        elseif colour == "blue"
            rgb.b = n
        else error("bad colour")
        end
    end
    rgb
end

function parseline(line)
    map(parsecubes,split(split(line,":")[2],";"))
end

function parseip(fn)
    map(parseline,readip(fn))
end

function possible(game)
    for rgb in game
        if rgb.r > 12 || rgb.g > 13 || rgb.b > 14
            return false
        end
    end
    true
end

function part1(fn)
    games = parseip(fn)
    n = 0
    for i in 1:length(games)
        if possible(games[i])
            n += i
        end
    end
    println(fn," part 1 ans = ",n)
end

function minset(game)
    m = RGB(0,0,0)
    for rgb in game
        m.r = max(rgb.r,m.r)
        m.g = max(rgb.g,m.g)
        m.b = max(rgb.b,m.b)
    end
    m
end

function part2(fn)
    games = parseip(fn)
    n = 0
    for game in games
        rgb = minset(game)
        n += rgb.r * rgb.g * rgb.b
    end
    println(fn," part 2 ans = ",n)
end

part1("test_input")
part1("input")
part2("test_input")
part2("input")
