function readip(fn)
    open(fn) do f
        readlines(f)
    end
end

function numsonly(s,nums)
    result = ""
    for i = 1:length(s)
        subs = s[i:end]
        for (s1,s2) in nums
            if startswith(subs,s1)
                result *= s2
            end
        end
    end
    result
end

nums1 = begin
    d = Dict{String,String}()
    for i in 0:9
        d[string(i)] = string(i)
    end
    d
end

nums2 = begin
    d = copy(nums1)
    d["one"] = "1"
    d["two"] = "2"
    d["three"] = "3"
    d["four"] = "4"
    d["five"] = "5"
    d["six"] = "6"
    d["seven"] = "7"
    d["eight"] = "8"
    d["nine"] = "9"
    d["zero"] = "0"
    d
end

function part1(fn)
    ip = readip(fn)
    snos = map((s)->numsonly(s,nums1),ip)
    nos = map((s)->parse(Int64,s[1]*s[end]),snos)
    println("part 1 ",fn, " ans = ",sum(nos))
end


function part2(fn)
    ip = readip(fn)
    snos = map((s)->numsonly(s,nums2),ip)
    nos = map((s)->10*parse(Int64,s[1]) + parse(Int64,s[end]),snos)
    println("part 2 ",fn, " ans = ",sum(nos))
end

part1("test_input")
part1("input")
part2("test_input2")
part2("input")
