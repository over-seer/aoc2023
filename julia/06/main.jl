function f(T::BigFloat, D::BigFloat)
    (- T + sqrt(T^2 - 4D))÷2 - (- T - sqrt(T^2 - 4D))÷2
end

println(f(big(44.),big(202.)))
println(f(big(44826981.),big(202107611381458.)))

